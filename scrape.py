#!/usr/bin/env python
import requests
import datetime
from time import sleep
import json
from pathlib import Path


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


def aggregate_data():
    website_found = []
    with open("ten_thousand_com.txt") as wf:
        websites = [[w, False] for w in wf.read().splitlines()]
        # print(websites[:2])
        # return
        with open("website_data.json") as df, open("aggregate_data.json", "w") as agg, open("missing.txt", "w") as m:
            lines = df.read().splitlines()
            for index, entry in enumerate(websites):
                print(index)
                website, found = entry
                for line in lines:
                    data = json.loads(line)
                    if website in data.keys() and not found:
                        data[website]["rank"] = index + 1
                        websites[index][1] = True
                        if index == 9999:
                            agg.write(json.dumps(data))
                        else:
                            agg.write(json.dumps(data) + ",")

                    if websites[index][1]:
                        break
            for website, found in websites:
                if not found:
                    m.write(f"{website}\n")


if __name__ == '__main__':
    # clean_list()
    # scrape()
    aggregate_data()
