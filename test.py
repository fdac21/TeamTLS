with open('filtered_list.txt') as f:
    if 'fastcgi.com' in f.read():
        print("true")