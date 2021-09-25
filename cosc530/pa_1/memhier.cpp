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

template <typename T> void move(vector<T>& v, size_t oldIndex, size_t newIndex)
{
    if (oldIndex > newIndex)
        std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
    else        
        std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
}

/* End Helper Functions */

/* Config Functions */
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
    this->dcIndex = log2(this->dcSets);
    this->dcOffset = log2(this->dcLineSize);
    
    // Set L2 index and offset sizes
    this->L2Index = log2(this->L2Sets);
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

/* End Config Functions */

/* DataCache Functions */
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

void DataCache::replaceBlock(Set *s, int i, Block *blk, Block *resident) {
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
}

// TODO: Make sure sets doesn't need to be a vector of pointers
bool DataCache::processBlock(Block *blk, char action) {
    Set *s = NULL;
    Block  *resident = NULL;
    // cout << this->setSize << endl;
    // cout << hex << blk->index << endl;

    s = this->sets.at(blk->index % this->nSets);

    // If there's a free block, insert the block
    if (s->blocks.size() < this->setSize) {
        for (int i = 0; i < s->blocks.size(); i++) {
            resident = s->blocks.at(i);

            if (resident->tag == blk->tag) {
                this->hits++;
                // Update cache on write if there is a hit
                if (action == 'W') this->replaceBlock(s, i, blk, resident);
                // Move the block to the front of the set on a read hit
                else move(s->blocks, i, 0);
                return true;
            } 
        }
        s->blocks.insert(s->blocks.begin(), blk);
    } else {
        for (int i = 0; i < s->blocks.size(); i++) {
            resident = s->blocks.at(i);
            // If there's a block with matching index and tag
            if (resident->tag == blk->tag) {
                this->hits++;
                // Update cache on write if there is a hit
                if (action == 'W') this->replaceBlock(s, i, blk, resident);
                // Move the block to the front of the set on a read hit
                else move(s->blocks, i, 0);
                return true;
            } 
            // If there's not a matching block then it's a miss 
            else {
                // If this is a read or write-back is the active policiy, update the cache
                if (action == 'R' || !this->writeThrough) {
                    this->replaceBlock(s, i, blk, resident);
                    break;
                }
            }

        }

    }
    this->misses++;
    return false;
}
/* End DataCache Functions */

/* L2Cache Functions */

L2Cache::L2Cache(Config *conf) {
    this->nSets = conf->L2Sets;
    this->setSize = conf->L2SetSize;
    this->lineSize = conf->L2LineSize;
    this->offset = conf->L2Offset;
    this->index = conf->L2Index + conf->L2Offset;

    this->writeThrough = conf->L2WriteThrough;
    this->active = conf->L2;

    if (this->active) {
        this->sets.resize(this->nSets);
        for (int i = 0; i < this->nSets; i++) {
            this->sets.at(i) = new Set();
            this->sets.at(i)->blocks.reserve(setSize);
        }
    }

    this->hits = 0;
    this->misses = 0;
}

void L2Cache::replaceBlock(Set *s, int i, Block *blk, Block *resident) {
     // If the cache is direct mapped
    if (this->setSize == 1) {
        s->blocks.at(i) = blk;
        
        delete resident;
    } 
    // Otherwise LRU 
    else {
        s->blocks.pop_back();
        s->blocks.insert(s->blocks.begin(), blk);
    }
}

// TODO: Make sure sets doesn't need to be a vector of pointers
bool L2Cache::processBlock(Block *blk, char action) {
    Set *s = NULL;
    Block  *resident = NULL;
    // cout << hex << blk->index << endl;

    s = this->sets.at(blk->index % this->nSets);

    // If there's a free block, insert the block
    if (s->blocks.size() < this->setSize) {
        for (int i = 0; i < s->blocks.size(); i++) {
            resident = s->blocks.at(i);

            if (resident->tag == blk->tag) {
                this->hits++;
                // Update cache on write if there is a hit
                if (action == 'W') this->replaceBlock(s, i, blk, resident);
                // Move the block to the front of the set on a read hit
                else move(s->blocks, i, 0);
                return true;
            } 
        }
        s->blocks.insert(s->blocks.begin(), blk);
    } else {

        for (int i = 0; i < s->blocks.size(); i++) {
            resident = s->blocks.at(i);
            // If there's a block with matching index and tag
            if (resident->tag == blk->tag) {
                this->hits++;
                // Update cache on write if there is a hit
                if (action == 'W') this->replaceBlock(s, i, blk, resident);
                // Move the block to the front of the set on a read hit
                else move(s->blocks, i, 0);
                return true;
            } 
            // If there's not a matching block then it's a miss 
            else {
                // If this is a read or write-back is the active policiy, update the cache
                if (action == 'R' || !this->writeThrough) {
                    this->replaceBlock(s, i, blk, resident);
                    break;
                }
            }

        }

    }
    this->misses++;
    return false;
}

/* End L2Cache Functions */

/* PageTable Functions */
PageTable::PageTable(Config *conf) {
    this->pPages = conf->ptPPages;
    this->vPages = conf->ptVPages;
    this->pageSize = conf->ptPageSize;
    this->offset = conf->ptOffset;
    this->index = conf->ptOffset + conf->ptIndex;
    entries.reserve(this->pPages);
}

bool PageTable::processPTE(PageTableEntry *pte) {
    PageTableEntry *resident;
    uint64_t i = 0;

    // If there's a free PTE, insert the PTE
    if (this->entries.size() < this->pPages) {
        for (; i < this->entries.size(); i++) {
            resident = this->entries.at(i);

            // Move the PTE to the front of the entries vector on a hit
            if (resident->vpn == pte->vpn) {
                this->hits++;
                move(this->entries, i, 0);
                pte->ppn = resident->ppn;
                return true;
            } 
        }
        pte->ppn = i;
        this->entries.insert(this->entries.begin(), pte);
    } else {
        for (i = 0; i < this->entries.size(); i++) {
            resident = this->entries.at(i);
            // TODO Find value for matching (Probably vpn)
            // Move the PTE to the front of the set on a read hit
            if (resident->vpn == pte->vpn) {
                this->hits++;
                move(this->entries, i, 0);
                pte->ppn = resident->ppn;
                return true;
            } 
        } 
        // If there's not a matching PTE then it's a miss 
        pte->ppn = resident->ppn;
        this->entries.pop_back();
        this->entries.insert(this->entries.begin(), pte);
    }
    this->misses++;
    return false;
}

/* End Page Table Functions */

Memory::Memory(string configFile) {
    this->conf = new Config(configFile);
    this->dc = new DataCache(conf);
    this->L2 = new L2Cache(conf);
    this->pt = new PageTable(conf);
}

void Memory::processData(string source) {
    string line;
    char action;
    uint64_t addr, offset, pageNumber, index, tag;
    bool dcHit, ptHit;
    Block *blk;
    PageTableEntry *pte;

    while(cin >> line) {
        blk = NULL;

        sscanf(line.c_str(), " %c:%x", &action, &addr);
        offset = getPortion(addr, 0, this->conf->ptOffset);
        index = getPortion(addr, this->dc->offset, this->dc->index);
        tag = getPortion(addr, this->dc->index, 64);


        pte = createEntry(addr, this->conf);
        ptHit = this->pt->processPTE(pte);
        pte->paddress = pte->offset | (pte->ppn << this->conf->ptOffset);

        if (ptHit) {
            printf("%08x | %x | %2x | %x | hit  |", pte->vaddress, pte->vpn, pte->offset, pte->ppn);
        } else {
            printf("%08x | %x | %2x | %x | miss |", pte->vaddress, pte->vpn, pte->offset, pte->ppn);
        }
        addr = pte->paddress;
        index = getPortion(addr, this->dc->offset, this->dc->index);
        tag = getPortion(addr, this->dc->index, 64);

        blk = createBlock(addr, offset, index, tag);

        if ((dcHit = this->dc->processBlock(blk, action))) {
            printf(" %x | %x | hit  ", blk->tag, blk->index);
        } else {
            printf(" %x | %x | miss ",  blk->tag, blk->index);
        }


        if (this->conf->L2 && !dcHit) {
            index = getPortion(addr, this->L2->offset, this->L2->index);
            tag = getPortion(addr, this->L2->index, 64);
            blk = createBlock(addr, offset, index, tag);

            if (this->L2->processBlock(blk, action)) {
                printf("| %-6x | %x | hit  \n", blk->tag, blk->index);
            } else {
                printf("| %-6x | %x | miss \n", blk->tag, blk->index);
            }
        } else {
            printf("| %-6s | %s |     \n", " ", " ");
        }

        // delete blk;
        // printf("%08x  | %-11x | %-11x | %-6x | %x \n", blk->address, blk->offset, pageNumber, blk->tag, blk->index);


    }
    // for (int i = 0; i < this->dc->sets.size(); i++) {
    //     // cout << i << endl;
    //     for (int j = 0; j < this->dc->sets.at(i)->blocks.size(); j++) {
    //         blk = this->dc->sets.at(i)->blocks.at(j);
    //         if (blk != NULL) {
    //             printf("%08x  | %-11x | %-11x | %-6x | %x \n", blk->address, blk->offset, pageNumber, blk->tag, blk->index);
    //             delete blk;
    //         }

    //         blk = NULL;
    //     }
    // }

    cout << "DC Hits: " << this->dc->hits << endl;
    cout << "DC Misses: " << this->dc->misses << endl;
    printf("DC Hit Ratio: %.6f\n", (float) this->dc->hits / (float) (this->dc->hits + this->dc->misses));
    cout << "L2 Hits: " << this->L2->hits << endl;
    cout << "L2 Misses: " << this->L2->misses << endl;
    printf("L2 Hit Ratio: %.6f\n", (float) this->L2->hits / (float) (this->L2->hits + this->L2->misses));
    cout << "PT Hits: " << this->pt->hits << endl;
    cout << "PT Misses: " << this->pt->misses << endl;
    printf("PT Hit Ratio: %.6f\n", (float) this->pt->hits / (float) (this->pt->hits + this->pt->misses));
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
    entry->vaddress = address;
    entry->paddress = 0;
    entry->offset = getPortion(address, 0, conf->ptOffset);
    entry->vpn = getPortion(address, conf->ptOffset, log2(conf->ptVPages) + conf->ptOffset);
    entry->used = false;
    entry->resident = false;
    entry->dirty = false;

    return entry;
}
