
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
    uint32_t tlbSets;
    uint32_t tlbSetSize;

    // Page Table Config
    uint32_t ptVPages;
    uint32_t ptPPages;
    uint32_t ptPageSize;
    uint32_t ptIndex;
    uint32_t ptOffset;

    // Data Cache Config
    uint32_t dcSets;
    uint32_t dcSetSize;
    uint32_t dcLineSize;
    uint32_t dcIndex;
    uint32_t dcOffset; 
    bool dcWriteThrough;

    // L2 Cache Config
    uint32_t L2Sets;
    uint32_t L2SetSize;
    uint32_t L2LineSize;
    uint32_t L2Index;
    uint32_t L2Offset;
    bool L2WriteThrough;

    bool vAddr;
    bool tlb;
    bool L2;

    Config(string filename);

    void printConfig();
};

// Block
struct Block {
    uint32_t address;
    uint32_t tag;
    uint32_t index;
    uint32_t offset;
};

struct PageTableEntry {
    uint32_t address;
    uint32_t vpn;
    uint32_t ppn;
    uint32_t offset;
};

// Set
class Set {
public:
    vector<Block> blocks;    
};

class TLB {
private:
    vector<Set> sets;

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
private:
    vector<Set> sets;

public:
    DataCache(Config *conf);
};

class L2Cache {
private:
    vector<Set> sets;

public:
    L2Cache(Config *conf);
};

// Cache 
class Cache {
private:
    string source;
    DataCache *dc;
    PageTable *pt;
    TLB *tlb;
    L2Cache *L2;

public:
    Cache(string source, Config *conf);
    void readSource();
    void processData(Config *conf);
};

bool validateOption(char choice, string option);

#endif