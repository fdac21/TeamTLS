from pymongo import MongoClient
client = MongoClient(
    "mongodb://admin:cosc545mongodbfall2021@76.10.62.77:27017")
db = client.tls_data
db.websites.delete_many({})
db.endpoints.delete_many({})
db.certificates.delete_many({})
