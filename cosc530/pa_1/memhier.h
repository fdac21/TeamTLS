
#ifndef MEMHIER_H
#define MEMHIER_H
#include "stdio.h"
#include "stdint.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

// Config
class Config {
public:
    uint64_t tlbSets;
    uint64_t tlbSetSize;

    // Page Table Config
    uint64_t ptVPages;
    uint64_t ptPPages;
    uint64_t ptPageSize;
    uint64_t ptIndex;
    uint64_t ptOffset;

    // Data Cache Config
    uint64_t dcSets;
    uint64_t dcSetSize;
    uint64_t dcLineSize;
    uint64_t dcIndex;
    uint64_t dcOffset; 
    bool dcWriteThrough;

    // L2 Cache Config
    uint64_t L2Sets;
    uint64_t L2SetSize;
    uint64_t L2LineSize;
    uint64_t L2Index;
    uint64_t L2Offset;
    bool L2WriteThrough;

    bool vAddr;
    bool tlb;
    bool L2;

    Config(string filename);

    void printConfig();
};

// Block
struct Block {
    uint64_t address;
    uint64_t tag;
    uint64_t index;
    uint64_t offset;
};

struct PageTableEntry {
    uint64_t address;
    uint64_t vpn;
    uint64_t ppn;
    uint64_t offset;
};

// Set
class Set {
public:
    vector<Block *> blocks;    
};

class TLB {
private:
    vector<Set *> sets;

public:
    TLB(Config *conf);
};

class PageTable {
private:
    vector<PageTableEntry> entries;

public:
    PageTable(Config *conf);
};

class DataCache {
protected:
    int nSets;
    int setSize;
    int lineSize;
    bool writeThrough;

public:
    int offset;
    int index;
    vector<Set *> sets;

    DataCache(Config *conf);
    void initFrame();
    bool processBlock(Block *blk);
};

class L2Cache {
    vector<Set> sets;
    int nSets;
    int setSize;
    int lineSize;
    bool writeThrough;
public:
    bool active;
    L2Cache(Config *conf);
};

// Cache 
class Memory {
private:
    Config *conf;
    DataCache *dc;
    PageTable *pt;
    TLB *tlb;
    L2Cache *L2;

public:
    Memory(string configFile);
    void processData(string source);
    void printConfig() {this->conf->printConfig();};
};

bool validateOption(char choice, string option);

Block *createBlock(uint64_t addr, uint64_t offset, uint64_t index, uint64_t tag);

#endif