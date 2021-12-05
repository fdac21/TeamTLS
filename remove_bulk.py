import json

with open('test.json') as input_file, open('test.json') as output_file:
    for line in input_file.readlines():
        data = json.load(line)
        for element in data: 
            del element['imageData'] 