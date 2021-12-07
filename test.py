from pymongo import MongoClient
import json
client = MongoClient(
    "mongodb://admin:cosc545mongodbfall2021@76.10.62.77:27017")

db = client.admin
serverStatusResult = db.command("listDatabases")

web_collection = client["tls_data"]["websites"]
certs_collection = client["tls_data"]["certificates"]

print(certs_collection.count_documents({"domain": "google.com"}))
