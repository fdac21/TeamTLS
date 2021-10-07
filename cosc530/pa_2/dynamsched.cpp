#include "dynamsched.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    initScheduler(NULL, NULL);
}

void getValue(fstream& f, string& line) {
    getline(f, line, ':');
    getline(f, line);
}

void initScheduler(Buffer* buf, Latency* lat) {
    string source = "source/config.txt";
    string buffer = "buffer";
    string latency = "latency";
    string line;

    fstream conf;

    conf.open(source);

    if (!conf.is_open()) {
        perror("Error opening config file.\n");
        exit(-1);
    }

    while (getline(conf, line) && line != "buffers")
        ;

    getline(conf, line);

    getValue(conf, line);

    cout << line << endl;
}
