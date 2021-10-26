#include "dynamsched.hpp"

#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    DynamSched sched = DynamSched("source/config.txt");
    sched.process();

    return 0;
}

int getValue(fstream& f) {
    string line;
    stringstream ss;
    int val;
    getline(f, line, ':');
    getline(f, line);
    ss << line;
    ss >> val;

    return val;
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
    this->allocateBuffer(conf, this->arithmetics);

    // Allocate Reorder Buffer
    this->allocateReorderBuffer(conf);

    // Set Latencies
    this->setLatencies(conf);

    // Allocate Floating-Point Registers
    this->allocateRegisters(this->fps);

    // Allocate Integer Registers
    this->allocateRegisters(this->ints);

    cout << this->latencies.fp_add << endl;
}

void DynamSched::allocateBuffer(fstream& conf, vector<ResStation>& buffer) {
    int size = 0;

    size = getValue(conf);

    buffer.resize(size);

    for (int i = 0; i < buffer.size(); i++) {
        buffer.at(i) = ResStation();
    }
}

void DynamSched::allocateReorderBuffer(fstream& conf) {
    int size = 0;

    size = getValue(conf);

    this->reorder.resize(size);

    for (int i = 0; i < this->reorder.size(); i++) this->reorder.at(i) = NULL;
}

void DynamSched::setLatencies(fstream& conf) {
    int cycles;

    cycles = getValue(conf);
    this->latencies.fp_add = cycles;

    cycles = getValue(conf);
    this->latencies.fp_sub = cycles;

    cycles = getValue(conf);
    this->latencies.fp_mul = cycles;

    cycles = getValue(conf);
    this->latencies.fp_div = cycles;
}

void DynamSched::allocateRegisters(vector<Register>& registers) {
    registers.resize(16);
    for (int i = 0; i < 16; i++) {
        registers.at(i) = Register();
    }
}

void DynamSched::process() {}

/* Reorder Buffer */

ReorderBuffer::ReorderBuffer(int size) { this->size = size; }

void ReorderBuffer::commitEntry() {
    // Remove entry at head of buffer and commit the results
    // Possibly return Entry to caller and take care of scheduler interaction
    // outside of buffer
}

/* End Reorder Buffer */

ResStation::ResStation() {}
