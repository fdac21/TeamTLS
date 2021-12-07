#!/usr/bin/env python
import requests
import datetime
from time import sleep
import json
from pathlib import Path
from pymongo import MongoClient
from configparser import ConfigParser


def nextURL(f):
    for line in f.readlines():
        yield line.strip()


def scrape():
    with open("ten_thousand_com.txt", "r") as rf, open("website_data.json", 'w') as wf:

        website_queue = []
        current_queue = []

        website_number = 0
        current_connections = 0
        max_connections = 25

        for url in nextURL(rf):

            website_number += 1

            if len(website_queue) < max_connections or len(url) == 0:
                website_queue.append(url)
            else:
                current_queue = website_queue

            for website in current_queue:

                try:
                    print(
                        f"Fetching website: {website_number}: {website} at {datetime.datetime.now().strftime('%I:%M:%S')}")

                    response = requests.get(
                        f"https://api.ssllabs.com/api/v3/analyze?host=https://{website}&all=on&fromCache=on")

                    if (response.json()["status"] == "READY"):
                        wf.write(response.text + '\n')
                        website_queue.remove(website)

                    if (response.json()["status"] == "ERROR"):
                        website_queue.remove(website)

                    max_connections = int(
                        response.headers['X-Max-Assessments']) - 1
                    sleep(2)

                except Exception as e:
                    print(e, website, response.text)
                    website_queue.remove(website)

            current_queue = []


# def clean_list():
#     with open("website_data.json") as d, open("ten_thousand_com.txt") as lst, open("filtered_list.txt", "w+") as nlst:
#         lines = [line.strip() for line in lst.readlines()]
#         for line in d.readlines():
#             data = json.loads(line)
#             host = data["host"].split("https://")[1]
#             res = data["endpoints"][0]["statusMessage"]
#             if host in lines and "Unable to connect to the server" in res:
#                 print(host)
#                 bad_hosts.append(host)

#         lines = [line.strip() for line in lst.readlines()]

#         for line in lines:
#             if line not in hosts and line not in bad_hosts:
#                 print(line)
#                 nlst.write(line + "\n")

def get_json_path(directory, data_type=""):
    path = Path(directory)
    exts = [".json"]

    # Search within **/*
    for p in path.rglob('*'):
        if '.git' not in str(p.parent) and p.suffix in exts and data_type in p.name:
            yield p.resolve()


def getCertInfo(collection, data, website):
    certs = data["info"]["certs"]

    for cert in certs:

        document = {"domain": website}

        if website in cert["subject"]:
            document["root"] = False
        elif cert["subject"] == cert["issuerSubject"]:
            document["root"] = True
        else:
            continue

        document["issuerSubject"] = cert["issuerSubject"]
        document["sigAlg"] = cert["sigAlg"]
        document["keyAlg"] = cert["keyAlg"]
        document["keySize"] = cert["keySize"]
        document["keyStrength"] = cert["keyStrength"]

        if cert["revocationStatus"] == 0:
            document["revocationStatus"] = "not checked"
        elif cert["revocationStatus"] == 1:
            document["revocationStatus"] = "certificate revoked"
        elif cert["revocationStatus"] == 2:
            document["revocationStatus"] = "certificate not revoked"
        elif cert["revocationStatus"] == 3:
            document["revocationStatus"] = "revocation check error"
        elif cert["revocationStatus"] == 4:
            document["revocationStatus"] = "no revocation information"
        elif cert["revocationStatus"] == 5:
            document["revocationStatus"] = "internal error"

        collection.insert_one(document)


def getEndpointInfo(collection, data, website):
    endpoints = data["info"]["endpoints"]

    for endpoint in endpoints:
        document = {"domain": website}
        try:
            document["grade"] = endpoint["grade"]
        except:
            document["grade"] = "N/A"
        document["gradeTrustIgnored"] = endpoint["gradeTrustIgnored"]
        details = endpoint["details"]
        for protocol in details["protocols"]:
            document[f"{protocol['name']}_{protocol['version'].replace('.', '_')}"] = next((
                suite for suite in details["suites"] if suite["protocol"] == protocol["id"]), {})

        document["heartbleed"] = details["heartbleed"]
        document["vulnBeast"] = details["vulnBeast"]
        document["freak"] = details["freak"]
        document["drownVulnerable"] = details["drownVulnerable"]
        document["logjam"] = details["logjam"]
        document["poodle"] = details["poodle"]
        document["ocspStapling"] = details["ocspStapling"]

        collection.insert_one(document)


def insertOneToClient(client, data, website):
    web_collection = client["tls_data"]["websites"]
    certs_collection = client["tls_data"]["certificates"]
    endpoints_collection = client["tls_data"]["endpoints"]

    if certs_collection.count_documents({"domain": website}) == 0:
        getCertInfo(certs_collection, data[website], website)

    if endpoints_collection.count_documents({"domain": website}) == 0:
        getEndpointInfo(endpoints_collection, data[website], website)

    if web_collection.find_one({"domain": website}) == None:
        web_collection.insert_one(
            {"domain": website, "rank": data[website]["rank"]})


def checkIfFound(client, website):
    web_collection = client["tls_data"]["websites"]
    certs_collection = client["tls_data"]["certificates"]
    endpoints_collection = client["tls_data"]["endpoints"]

    if web_collection.find_one({"domain": website}) != None:
        if endpoints_collection.count_documents({"domain": website}) != 0:
            if certs_collection.count_documents({"domain": website}) != 0:
                return True
        if certs_collection.count_documents({"domain": website}) != 0:
            if endpoints_collection.count_documents({"domain": website}) != 0:
                return True


def aggregate_data(client):
    website_found = []
    with open("ten_thousand_com.txt") as wf:
        web_collection = client["tls_data"]["websites"]
        certs_collection = client["tls_data"]["certificates"]
        endpoints_collection = client["tls_data"]["endpoints"]
        websites = [[w, False] for w in wf.read().splitlines()]

        # print(websites[:2])
        # return
        with open("website_data.json") as df, open("missing.txt", "w") as m:
            lines = df.read().splitlines()
            for index, entry in enumerate(websites[:2500]):
                print(index)
                website, found = entry

                if checkIfFound(client, website):
                    continue

                for line in lines:
                    data = json.loads(line)
                    if website in data.keys() and not found:
                        data[website]["rank"] = index + 1
                        websites[index][1] = True
                        try:
                            insertOneToClient(client, data, website)
                        except Exception as e:
                            print(e)

                        # agg.write(json.dumps(data) + "\n")
                    if websites[index][1]:
                        break
            for index, entry in enumerate(websites[2500:5000]):
                print(index + 2501)
                website, found = entry

                if checkIfFound(client, website):
                    continue

                for line in lines[2000:]:
                    data = json.loads(line)
                    if website in data.keys() and not found:
                        data[website]["rank"] = index + 2501
                        websites[index + 2500][1] = True
                        try:
                            insertOneToClient(client, data, website)
                        except:
                            print("Error entering information for ", website)

                    if websites[index+2500][1]:
                        break
            for index, entry in enumerate(websites[5000:7500]):
                print(index+5001)
                website, found = entry

                if checkIfFound(client, website):
                    continue
                for line in lines[4000:]:
                    data = json.loads(line)
                    if website in data.keys() and not found:
                        data[website]["rank"] = index + 5001
                        websites[index+5000][1] = True
                        try:
                            insertOneToClient(client, data, website)
                        except:
                            print("Error entering information for ", website)

                    if websites[index+5000][1]:
                        break
            for index, entry in enumerate(websites[7500:]):
                print(index+7501)
                website, found = entry

                if checkIfFound(client, website):
                    continue
                for line in lines[6000:]:
                    data = json.loads(line)
                    if website in data.keys() and not found:
                        data[website]["rank"] = index + 6001
                        websites[index+7500][1] = True
                        try:
                            insertOneToClient(client, data, website)
                        except:
                            print("Error entering information for ", website)

                    if websites[index+7500][1]:
                        break
            for website, found in websites:
                if not found:
                    m.write(f"{website}\n")


if __name__ == '__main__':
    config = ConfigParser()
    config.read('creds.cfg')
    client = MongoClient(
        f"mongodb://admin:{config['mongodb']['passwd']}@76.10.62.77:27017")
    # clean_list()
    # scrape()
    aggregate_data(client)
