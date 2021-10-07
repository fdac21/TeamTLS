#include "dynamsched.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    vector<string> msg{"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    BufferConfig buf = BufferConfig("config.txt", "buffer");

    for (const string& word : msg) {
        cout << word << " ";
    }
    cout << endl;
}

void initScheduler(Buffer* buf, Latency lat*) {
    string source = "config.txt";
    string buffer = "buffer";
    string latency = "latency";
    string line;

    fstream conf;

    conf.open(source);

    if (!f.is_open()) {
        perror("Error opening config file.\n");
        exit(-1);
    }

    while (getline(conf))
}

Buffer::Buffer()
