#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "memhier.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[])
{

    Memory* mem = new Memory("old2.config");
    // mem->printConfig();
    // printf("Virtual  Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2\n");
    // printf("Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res.\n");
    // printf("-------- ------ ---- ------ --- ---- ---- ---- ------ --- ---- ------ --- ----\n");
    // printf("%08x %6x %4x %6x %3x %4s %4s %4x %6x %3x %4s %6x %3x %4s");
    printf("Phys Addr | Page Offset | Page Number | DC Tag | DC Index\n");
    mem->processData("trace.dat");

    delete mem;

    return 0;
}

/* Helper Functions */

bool validateOption(char choice, string option) {
    if (choice != 'n' && choice != 'y') fprintf(stderr, "Choice for %s must be 'y' or 'n'\n", option);
    return (choice == 'y') ? true : false;
}

uint64_t generateMask(int start, int end) {
    uint64_t mask = (end >= 48) ? 0xffffff : (1 << end) - 1;
    mask ^= (start <= 0) ? 0 : (1 << start) - 1;
    return mask;
}

uint64_t getPortion(uint64_t address, int start, int end) {
    uint64_t mask = generateMask(start, end);
    return (address & mask) >> start;
}

Config::Config(string filename) {
    ifstream f;
    string line;
    char choice;
  
    f.open(filename);
    if (!f.is_open()) {
        perror("Error opening config file.\n");
        exit(-1);
    }
    // Skip the top line of the config file
    getline(f, line);

    // Get number of TLB sets
    getline(f, line);
    sscanf(line.c_str(), "Number of sets: %d", &this->tlbSets);

    if (this->tlbSets < 0 || this->tlbSets >  256) {
        perror("Set count can only be between 0 and 256\n");
        exit(-1);
    }

    if (this->tlbSets  % 2 != 0) {
        perror("Set count must be a power of 2\n");
        exit(-1);
    }


    // Get tlb set size
    getline(f, line);
    sscanf(line.c_str(), "Set size: %d", &this->tlbSetSize);

    //  Skip line
    getline(f, line);
    getline(f, line);

    // Get # of virtual pages
    getline(f, line);
    sscanf(line.c_str(), "Number of virtual pages: %d", &this->ptVPages);
    if (this->ptVPages < 0 || this->ptVPages > 8192) {
        perror("Number of virtual pages can only be between 0 and 8192");
        exit(-1);
    }
    if (this->ptVPages  % 2 != 0) {
        perror("Number of virtual pages must be a power of 2\n");
        exit(-1);
    }

    // Get # of phyrical pages
    getline(f, line);
    sscanf(line.c_str(), "Number of physical pages: %d", &this->ptPPages);
    if (this->ptPPages < 0 || this->ptPPages > 1024) {
        perror("Number of physical pages can only be between 0 and 1024");
        exit(-1);
    }
    if (this->ptPPages  % 2 != 0) {
        perror("Number of physical pages must be a power of 2\n");
        exit(-1);
    }


    getline(f, line);
    sscanf(line.c_str(), "Page size: %d", &this->ptPageSize);

    getline(f, line);
    getline(f, line);

    // Get # of data cache sets
    getline(f, line);
    sscanf(line.c_str(), "Number of sets: %d", &this->dcSets);
   
    if (this->dcSets < 0 || this->dcSets >  256) {
        perror("Set count can only be between 0 and 256\n");
        exit(-1);
    }

    if (this->dcSets  % 2 != 0) {
        perror("Set count must be a power of 2\n");
        exit(-1);
    }

    // Get data cache set size
    getline(f, line);
    sscanf(line.c_str(), "Set size: %d", &this->dcSetSize);

    // Get data cache line size
    getline(f, line);
    sscanf(line.c_str(), "Line size: %d", &this->dcLineSize);
    if (this->dcLineSize < 8) {
        perror("Data line size for DC must be greater than 8.\n");
        exit(-1);
    }

    // Get data cache write through option
    getline(f, line);
    sscanf(line.c_str(), "Write through/no write allocate: %c", &choice);
    this->dcWriteThrough = validateOption(choice, "Data Cache Write Through");

    getline(f, line);
    getline(f, line);

    // Get # of L2 cache sets
    getline(f, line);
    sscanf(line.c_str(), "Number of sets: %d", &this->L2Sets);

    // Get L2 cache set size
    getline(f, line);
    sscanf(line.c_str(), "Set size: %d", &this->L2SetSize);

    // Get L2 cache line size
    getline(f, line);
    sscanf(line.c_str(), "Line size: %d", &this->L2LineSize);

    // Get L2 cache write through option
    getline(f, line);
    sscanf(line.c_str(), "Write through/no write allocate: %c", &choice);
    this->L2WriteThrough = validateOption(choice, "L2 Cache Write Through");

    getline(f, line);

    // Get V addr option
    getline(f, line);
    sscanf(line.c_str(), "Virtual addresses: %c", &choice);
    this->vAddr = validateOption(choice, "Virtual Addresses");

    // Get TLB option
    getline(f, line);
    sscanf(line.c_str(), "TLB: %c", &choice);
    this->tlb = validateOption(choice, "TLB");

    // Get L2 option
    getline(f, line);
    sscanf(line.c_str(), "L2 cache: %c", &choice);
    this->L2 = validateOption(choice, "L2 Cache");

    // Set Page Table index and offset sizes
    this->ptIndex = log2(this->ptVPages);
    this->ptOffset = log2(this->ptPageSize);

    // Set Data Cache index and offset sizes
    this->dcIndex = log2(this->dcSets / this->dcSetSize);
    this->dcOffset = log2(this->dcLineSize);
    
    // Set L2 index and offset sizes
    this->L2Index = log2(this->L2Sets / this->L2SetSize);
    this->L2Offset = log2(this->L2LineSize);

    f.close();    
}

void Config::printConfig() {

    printf("Data TLB contains %d sets.\n", this->tlbSets);
    printf("Each set contains %d entries.\n", this->tlbSetSize);
    printf("Number of bits used for the index is %d.\n\n", this->tlbSetSize);

    printf("Number of virtual pages is %d.\n", this->ptVPages);
    printf("Number of physical pages is %d.\n", this->ptPPages);
    printf("Each page contains %d bytes.\n", this->ptPageSize);
    printf("Number of bits used for the page table index is %d.\n", this->ptIndex);
    printf("Number of bits used for the page offset is %d.\n\n", this->ptOffset);

    printf("D-cache contains %d sets.\n", this->dcSets);
    printf("Each set contains %d entries.\n", this->dcSetSize);
    printf("Each line is %d bytes.\n", this->dcLineSize);
    if (this->dcWriteThrough  == 'n')  printf("The cache uses a write-allocate and write-back policy.\n");
    // TODO Figure out size sources
    printf("Number of bits used for the index is %d.\n", this->dcIndex);
    printf("Number of bits used for the offset is %d.\n\n", this->dcOffset);

    printf("L2-cache contains %d sets.\n", this->L2Sets);
    printf("Each set contains %d entries.\n", this->L2SetSize);
    printf("Each line is %d bytes.\n", this->L2LineSize);
    if (this->L2WriteThrough  == 'n')  printf("The cache uses a write-allocate and write-back policy.\n");
    printf("Number of bits used for the index is %d.\n", this->L2Index);
    printf("Number of bits used for the offset is %d.\n\n", this->L2Offset);

    if (this->vAddr == 'y') printf("The addresses read in are virtual addresses.\n");
    else printf("The addresses read in are physical addresses.\n");

    if (this->tlb == 'n') printf("TLB is disabled in this configuration.\n");
    if (this->L2 == 'n') printf("L2 cache is disabled in this configuration.\n");

    printf("\n");
}

DataCache::DataCache(Config *conf) {
    this->nSets = conf->dcSets;
    this->setSize = conf->dcSetSize;
    this->lineSize = conf->dcLineSize;
    this->offset = conf->dcOffset;
    this->index = conf->dcIndex + conf->dcOffset;
    this->writeThrough = conf->dcWriteThrough;
    this->sets.resize(nSets);


    for (int i = 0; i < this->nSets; i++) {
        this->sets.at(i) = new Set();
        this->sets.at(i)->blocks.reserve(setSize);
    }

    this->hits = 0;
    this->misses = 0;
}


void replaceBlock(vector<Block *> *set, Block *blk) {

}

// TODO: Make sure sets doesn't need to be a vector of pointers
bool DataCache::processBlock(Block *blk) {
    Set *s = NULL;
    Block  *resident = NULL;
    // cout << this->setSize << endl;
    // cout << hex << blk->index << endl;

    s = this->sets.at(blk->index % this->nSets);

    // If there's a free block, insert the block
    if (s->blocks.size() < this->setSize) {
        s->blocks.insert(s->blocks.begin(), blk);
    } else {
        for (int i = 0; i < s->blocks.size(); i++) {
            resident = s->blocks.at(i);
            // If there's a block with matching index and tag
            if (resident->tag == blk->tag) {
                this->hits++;
                return true;
            } 
            // If there's not a matching block
            else {
                // If the cache is direct mapped
                if (this->setSize == 1) {
                    s->blocks.at(i) = blk;
                    
                    delete resident;
                } 
                // Otherwise LRU 
                else {
                    // cout << hex << blk->offset << endl;
                    s->blocks.pop_back();
                    s->blocks.insert(s->blocks.begin(), blk);
                }

                break;
            }

        }

    }
    this->misses++;
    return false;
}

L2Cache::L2Cache(Config *conf) {
    this->nSets = conf->dcSets;
    this->setSize = conf->dcSetSize;
    this->lineSize = lineSize;
    this->writeThrough = writeThrough;
    this->active = conf->L2;
    if (this->active) {
        this->sets.resize(this->nSets);
        for (int i = 0; i < this->nSets; i++) {
            this->sets.at(i).blocks.reserve(this->setSize);
        }
    }
}

Memory::Memory(string configFile) {
    this->conf = new Config(configFile);
    this->dc = new DataCache(conf);
    // this->pt = new PageTable(conf);
}

void Memory::processData(string source) {
    ifstream f;
    string line;
    char action;
    uint64_t addr;
    uint64_t offset;
    uint64_t pageNumber;
    uint64_t index;
    uint64_t tag;
    Block *blk;
    PageTableEntry *entry;

    f.open(source);

    if (!f.is_open()) {
        perror("Error opening config file.\n");
        exit(-1);
    }

    while(getline(f, line)) {
        blk = NULL;

        sscanf(line.c_str(), " %c:%x", &action, &addr);
        offset = getPortion(addr, 0, this->conf->ptOffset);
        pageNumber = getPortion(addr, this->conf->ptOffset, this->conf->ptIndex + this->conf->ptOffset);
        index = getPortion(addr, this->dc->offset, this->dc->index);
        tag = getPortion(addr, this->dc->index, 64);

        blk = createBlock(addr, offset, index, tag);

        if(this->dc->processBlock(blk)) {
            printf("%08x  | %-11x | %-11x | %-6x | %x | hit \n", blk->address, blk->offset, pageNumber, blk->tag, blk->index);
            delete blk;
        } else {
            printf("%08x  | %-11x | %-11x | %-6x | %x | miss \n", blk->address, blk->offset, pageNumber, blk->tag, blk->index);
        }
        // delete blk;
        // printf("%08x  | %-11x | %-11x | %-6x | %x \n", blk->address, blk->offset, pageNumber, blk->tag, blk->index);


    }
    for (int i = 0; i < this->dc->sets.size(); i++) {
        // cout << i << endl;
        for (int j = 0; j < this->dc->sets.at(i)->blocks.size(); j++) {
            blk = this->dc->sets.at(i)->blocks.at(j);
            if (blk != NULL) {
                printf("%08x  | %-11x | %-11x | %-6x | %x \n", blk->address, blk->offset, pageNumber, blk->tag, blk->index);
                delete blk;
            }

            blk = NULL;
        }
    }
    cout << "Hits: " << this->dc->hits << endl;
    cout << "Misses: " << this->dc->misses << endl;
    printf("Hit Ratio: %.6f\n", (float) this->dc->hits / (float) (this->dc->hits + this->dc->misses));
    f.close();
}

Block *createBlock(uint64_t addr, uint64_t offset, uint64_t index, uint64_t tag) {
    Block *blk = new Block();
    blk->address = addr;
    blk->offset = offset;
    blk->index = index;
    blk->tag = tag;
    return blk;
}

PageTableEntry *createEntry(uint64_t address, Config *conf) {
    PageTableEntry *entry = new PageTableEntry();
    entry->address = address;
    entry->offset = getPortion(address, 0, conf->ptOffset);
    entry->vpn = getPortion(address, conf->ptOffset, log2(conf->ptVPages));
    entry->ppn = getPortion(address, conf->ptOffset, log2(conf->ptPPages));
    return entry;
}
