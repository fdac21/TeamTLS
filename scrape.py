import requests
import datetime
from time import sleep
import json


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
                except:
                    pass

            current_queue = []


def clean_list():
    with open("website_data.json") as d, open("ten_thousand_com.txt") as lst, open("filtered_list.txt", "w+") as nlst:
        lines = [line.strip() for line in lst.readlines()]
        for line in d.readlines():
            data = json.loads(line)
            host = data["host"].split("https://")[1]
            res = data["endpoints"][0]["statusMessage"]
            if host in lines and "Unable to connect to the server" in res:
                print(host)
                nlst.write(host + "\n")


if __name__ == '__main__':
    clean_list()
    # scrape()
