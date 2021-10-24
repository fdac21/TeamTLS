#include "dynamsched.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    DynamSched sched = DynamSched("source/config.txt");
}

void getValue(fstream& f, string& line, stringstream& ss) {
    getline(f, line, ':');
    getline(f, line);
    ss << line;
}

DynamSched::DynamSched(string configFile) {
    string line;
    fstream conf;

    conf.open(configFile);

    if (!conf.is_open()) {
        perror("Error opening config file.\n");
        exit(-1);
    }

    while (getline(conf, line) && line != "buffers")
        ;

    // Skip empty line
    getline(conf, line);

    // Allocate Effective Address Buffer
    this->allocateBuffer(conf, this->effAddr);

    // Allocate Floating-Point Adds Buffer
    this->allocateBuffer(conf, this->fpAdds);

    // Allocate Floating-Point Muls Buffer
    this->allocateBuffer(conf, this->fpMuls);

    // Allocate Int Buffer
    this->allocateBuffer(conf, this->Ints);

    // Allocate Reorder Buffer
    this->allocateBuffer(conf, this->reorder);
}

void DynamSched::allocateBuffer(fstream& conf, vector<ResStation>& buffer) {
    string line;
    stringstream ss;
    int size = 0;

    getValue(conf, line, ss);
    ss >> size;

    buffer.resize(size);

    for (int i = 0; i < buffer.size(); i++) {
        buffer.at(i) = ResStation();
    }
}

ResStation::ResStation() {
    this->instruction = 0;
}