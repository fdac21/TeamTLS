
#ifndef MEMHIER_H
#define MEMHIER_H
#include "stdio.h"
#include "stdint.h"
#include <string>
#include <vector>
#include <fstream>
using std::string;
using std::vector;
using std::ofstream;


enum updateType{NONE, WRITEBACK, MULTI};

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
    uint64_t ppn;
    bool dirty;
    bool valid;
};

struct PageTableEntry {
    uint64_t vaddress;
    uint64_t paddress;
    uint64_t vpn;
    uint64_t ppn;
    uint64_t offset;
    bool dirty;
    bool valid;
};

// Set
class Set {
public:
    vector<Block *> blocks; 
};

class TLB {
private:
    int nSets;
    int setSize;

public:
    int offset;
    int index;
    int hits;
    int misses;
    bool active;
    Block *invalid;
    vector<Set *> sets;
    TLB(Config *conf);
    bool processData(Block *blk);
    void invalidateBlock(uint64_t ppn);
    void replaceBlock(Set *s, int i, Block *blk, Block *resident);
};

class PageTable {
private:
    vector<PageTableEntry *> entries;
    vector<int> pPageEntries;
    int vPages;
    int pPages;
    int pageSize;
    int offset;
    int index;

public:
    int hits;
    int misses;
    int invalidPPN;
    PageTable(Config *conf);
    bool processPTE(PageTableEntry *pte);
    void virt2phys(PageTableEntry *pte);
    void printTable(ofstream& f);
};

class DataCache {
protected:
    int nSets;
    int setSize;
    int lineSize;


public:
    bool writeThrough;
    int offset;
    int index;
    int hits;
    int misses;
    Block *write;
    vector<Set *> sets;

    DataCache(Config *conf);
    void initFrame();
    void replaceBlock(Set *s, int i, Block *blk, Block *resident);
    bool processBlock(Block *blk, char action);
};

class L2Cache {
    int nSets;
    int setSize;
    int lineSize;

public:
    bool writeThrough;
    bool writeBlock;
    bool active;
    int hits;
    int misses;
    int offset;
    int index;
    vector<Set *> sets;
    L2Cache(Config *conf);
    void initFrame();
    void replaceBlock(Set *s, int i, Block *blk, Block *resident);
    bool processBlock(Block *blk, char action, updateType update);
    void printCache(ofstream &f);
};

// Cache 
class Memory {
private:
    DataCache *dc;
    PageTable *pt;
    TLB *tlb;
    L2Cache *L2;

public:
    Config *conf;
    int diskRefs;
    int memRefs;
    int reads;
    int writes;
    Memory(string configFile);
    void processData(string source);
    void printConfig() {this->conf->printConfig();};
};

bool validateOption(char choice, string option);

Block *createBlock(uint64_t addr, uint64_t offset, uint64_t index, uint64_t tag, uint64_t ppn);
Block *createBlock(uint64_t addr, uint64_t offset, uint64_t index, uint64_t tag, uint64_t ppn, bool valid);
PageTableEntry *createEntry(uint64_t address, Config *conf);

#endif