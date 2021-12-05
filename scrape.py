import requests
import datetime
from time import sleep
import json


def nextURL(f):
    for line in f.readlines():
        yield line.strip()


def scrape():
    with open("ten_thousand_com.txt") as rf, open("website_data9000.json", 'a') as wf, open("errors9000.json", 'a') as ef:

        website_queue = []
        current_queue = []

        website_number = 9961
        max_connections = 25
        stop = False
        urlGen = nextURL(rf)

        while True:

            url = ""

            if len(website_queue) < max_connections and not stop:
                
                try:
                    url = next(urlGen)
                    website_number += 1
                    website_queue.append((url, website_number))
                except:
                    stop = True

            current_queue = website_queue

            if stop and len(current_queue) == 0:
                break

            outstanding_connections = 0

            for website, rank in current_queue:

                outstanding_connections += 1

                if outstanding_connections >= max_connections:
                    break

                try:


                    print(
                        f"Fetching website: max={max_connections}: #={rank}: {website} at {datetime.datetime.now().strftime('%I:%M:%S')}")

                    response = requests.get(
                        f"https://api.ssllabs.com/api/v3/analyze?host=https://{website}&all=on&fromCache=on")

                    if (response.json()["status"] == "READY"):
                        output = f'{{"{website}": {{"rank": {rank}, "info": {response.text}}}}}\n'
                        wf.write(output)
                        website_queue.remove((website, rank))

                    if (response.json()["status"] == "ERROR"):
                        output = f'{{"{website}": {{"rank": {rank}, "info": {response.text}}}}}\n'
                        ef.write(output)
                        website_queue.remove((website, rank))

                    max_connections = int(response.headers['X-Max-Assessments']) - 1
                    sleep(2)
                    
                except Exception as e:
                    print(e, website, response.text)
                    website_queue.remove(website)

            current_queue = []


def clean_list():
    with open("combined_websites.json") as d, open("ten_thousand_com.txt") as lst, open("filtered_list.txt", "w+") as nlst:
        hosts = []
        bad_hosts = []
        for line in d.readlines():
            data = json.loads(line)
            host = data["host"].split("https://")[1]
            res = data["endpoints"][0]["statusMessage"]
            hosts.append(host)
            if "Unable to connect to the server" in res:
                print(host)
                bad_hosts.append(host)


        lines = [line.strip() for line in lst.readlines()]

        for line in lines:
            if line not in hosts and line not in bad_hosts:
                print(line)
                nlst.write(line + "\n")



if __name__ == '__main__':
    # clean_list()
    scrape()
