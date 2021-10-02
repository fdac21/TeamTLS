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

    Memory* mem = new Memory("trace.config");
    mem->printConfig();
    if (mem->conf->vAddr) {
        printf("Virtual  Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2\n");
    } else {
        printf("Physical Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2\n");
    }
    printf("Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res.\n");
    printf("-------- ------ ---- ------ --- ---- ---- ---- ------ --- ---- ------ --- ----\n");
    // printf("%4s %4s  %4s %4s");
    mem->processData("trace.dat");

    delete mem;

    return 0;
}

/* Helper Functions */

bool validateOption(char choice, string option) {
    if (choice != 'n' && choice != 'y') fprintf(stderr, "Choice for %s must be 'y' or 'n'\n", option.c_str());
    return choice == 'y';
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
    sscanf(line.c_str(), "Number of sets: %ld", &this->tlbSets);

    if (this->tlbSets < 0 || this->tlbSets >  256) {
        perror("Set count can only be between 0 and 256\n");
        exit(-1);
    }

    // if (this->tlbSets  % 2 != 0) {
    //     perror("Set count must be a power of 2\n");
    //     exit(-1);
    // }


    // Get tlb set size
    getline(f, line);
    sscanf(line.c_str(), "Set size: %ld", &this->tlbSetSize);

    //  Skip line
    getline(f, line);
    getline(f, line);

    // Get # of virtual pages
    getline(f, line);
    sscanf(line.c_str(), "Number of virtual pages: %ld", &this->ptVPages);
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
    sscanf(line.c_str(), "Number of physical pages: %ld", &this->ptPPages);
    if (this->ptPPages < 0 || this->ptPPages > 1024) {
        perror("Number of physical pages can only be between 0 and 1024");
        exit(-1);
    }
    if (this->ptPPages  % 2 != 0) {
        perror("Number of physical pages must be a power of 2\n");
        exit(-1);
    }


    getline(f, line);
    sscanf(line.c_str(), "Page size: %ld", &this->ptPageSize);

    getline(f, line);
    getline(f, line);

    // Get # of data cache sets
    getline(f, line);
    sscanf(line.c_str(), "Number of sets: %ld", &this->dcSets);
   
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
    sscanf(line.c_str(), "Set size: %ld", &this->dcSetSize);

    // Get data cache line size
    getline(f, line);
    sscanf(line.c_str(), "Line size: %ld", &this->dcLineSize);
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
    sscanf(line.c_str(), "Number of sets: %ld", &this->L2Sets);

    // Get L2 cache set size
    getline(f, line);
    sscanf(line.c_str(), "Set size: %ld", &this->L2SetSize);

    // Get L2 cache line size
    getline(f, line);
    sscanf(line.c_str(), "Line size: %ld", &this->L2LineSize);

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

    printf("Data TLB contains %ld sets.\n", this->tlbSets);
    printf("Each set contains %ld entries.\n", this->tlbSetSize);
    printf("Number of bits used for the index is %ld.\n\n", this->tlbSetSize);

    printf("Number of virtual pages is %ld.\n", this->ptVPages);
    printf("Number of physical pages is %ld.\n", this->ptPPages);
    printf("Each page contains %ld bytes.\n", this->ptPageSize);
    printf("Number of bits used for the page table index is %ld.\n", this->ptIndex);
    printf("Number of bits used for the page offset is %ld.\n\n", this->ptOffset);

    printf("D-cache contains %ld sets.\n", this->dcSets);
    printf("Each set contains %ld entries.\n", this->dcSetSize);
    printf("Each line is %ld bytes.\n", this->dcLineSize);
    if (this->dcWriteThrough)  printf("The cache uses a write-allocate and write-back policy.\n");
    else printf("The cache uses a write-allocate and write-back policy.\n");
    // TODO Figure out size sources
    printf("Number of bits used for the index is %ld.\n", this->dcIndex);
    printf("Number of bits used for the offset is %ld.\n\n", this->dcOffset);

    printf("L2-cache contains %ld sets.\n", this->L2Sets);
    printf("Each set contains %ld entries.\n", this->L2SetSize);
    printf("Each line is %ld bytes.\n", this->L2LineSize);
    if (this->L2WriteThrough)  printf("The cache uses a write-allocate and write-back policy.\n");
    else printf("The cache uses a write-allocate and write-back policy.\n");
    printf("Number of bits used for the index is %ld.\n", this->L2Index);
    printf("Number of bits used for the offset is %ld.\n\n", this->L2Offset);

    if (this->vAddr) printf("The addresses read in are virtual addresses.\n");
    else printf("The addresses read in are physical addresses.\n");

    if (!this->tlb) printf("TLB is disabled in this configuration.\n");
    if (!this->L2) printf("L2 cache is disabled in this configuration.\n");

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
    this->write = NULL;
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

bool DataCache::processBlock(Block *blk, char action) {
    Set *s = NULL;
    Block  *resident = NULL;
    int i;
    // cout << this->setSize << endl;
    // cout << hex << blk->index << endl;

    s = this->sets.at(blk->index % this->nSets);

    for (i = 0; i < s->blocks.size(); i++) {
        resident = s->blocks.at(i);

        // Cache Hit
        if (resident->tag == blk->tag && resident->valid) {
            this->hits++;
            
            // Read
            if (action == 'R') move(s->blocks, i, 0);
            // Write-Back/Write-Allocate
            else if (action == 'W' && !this->writeThrough) {
                move(s->blocks, i, 0);
            } 
            // Write-Through/No Write-Allocate
            else {
               move(s->blocks, i, 0);
            }
            return true;
        }
    }
    if (s->blocks.size() < this->setSize) {
        
        // Cache Miss with room to insert
        // Read
        if (action == 'R') {
            s->blocks.insert(s->blocks.begin(), blk);
        }
        // Write-Back/Write-Allocate
        else if (action == 'W' && !this->writeThrough)  {
            blk->dirty = true;
            // TODO Determine if I need to write out the LRU block if it's not being evicted
            // this->write = createBlock(resident->address, resident->offset, resident->index, resident->tag, resident->ppn, resident->valid);
            // this->write->dirty = resident->dirty;
            s->blocks.insert(s->blocks.begin(), blk);
        } 
    } else {
        i--;
        // Cache Miss
        if (action == 'R') {
            this->replaceBlock(s, i, blk, resident);
        } 
        // Write-Back/Write-Allocate
        else if (action == 'W' && !this->writeThrough)  {
            blk->dirty = true;
            this->write = createBlock(resident->address, resident->offset, resident->index, resident->tag, resident->ppn, resident->valid);
            this->write->dirty = resident->dirty;
            this->replaceBlock(s, i, blk, resident);
        } 
        // Don't update cache on Write-Through/No Write-Allocate
    }

    this->misses++;
    return false;
}

void DataCache::invalidateBlock(uint64_t ppn) {
    Set *s;
    Block *blk;

    for (int i = 0; i < this->nSets; i++) {
        s = this->sets.at(i);

        for (int j = 0; j < s->blocks.size(); j++) {
            blk = s->blocks.at(j);

            if (blk->ppn == ppn) blk->valid = false;
        }
    }
}

/* End DataCache Functions */

/* L2Cache Functions */
L2Cache::L2Cache(Config *conf) {
    this->nSets = conf->L2Sets;
    this->setSize = conf->L2SetSize;
    this->lineSize = conf->L2LineSize;
    this->offset = conf->L2Offset;
    this->index = conf->L2Index + conf->L2Offset;
    this->writeBlock = false;
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
    int i;

    s = this->sets.at(blk->index % this->nSets);

    for (i = 0; i < s->blocks.size(); i++) {
        resident = s->blocks.at(i);

        // Cache Hit
        if (resident->tag == blk->tag && resident->valid) {
            this->hits++;
            
            // Read
            if (action == 'R') move(s->blocks, i, 0);
            // Write-Back/Write-Allocate
            else if (action == 'W' && !this->writeThrough) {
                blk->dirty = true;
                move(s->blocks, i, 0);
            }
            // Write-Through/No Write-Allocate
            else {
               move(s->blocks, i, 0);
            }
            return true;
        }
    }
    if (s->blocks.size() < this->setSize) {
        
        // Cache Miss with room to insert
        // Read
        if (action == 'R') {
            s->blocks.insert(s->blocks.begin(), blk);
        } else if (action == 'W' && !this->writeThrough)  {
            blk->dirty = true;
            s->blocks.insert(s->blocks.begin(), blk);
        }
    } else {
        i--;
        // Cache Miss
        if (action == 'R') {
            this->replaceBlock(s, i, blk, resident);
        }  else if (action == 'W' && !this->writeThrough)  {
            blk->dirty = true;
            this->writeBlock = true;
            this->replaceBlock(s, i, blk, resident);
        }
    }


    this->misses++;
    return false;
}

void L2Cache::invalidateBlock(uint64_t ppn) {
    Set *s;
    Block *blk;

    for (int i = 0; i < this->nSets; i++) {
        s = this->sets.at(i);

        for (int j = 0; j < s->blocks.size(); j++) {
            blk = s->blocks.at(j);

            if (blk->ppn == ppn) blk->valid = false;
        }
    }
}

/* End L2Cache Functions */

/* PageTable Functions */
PageTable::PageTable(Config *conf) {
    this->pPages = conf->ptPPages;
    this->vPages = conf->ptVPages;
    this->pageSize = conf->ptPageSize;
    this->offset = conf->ptOffset;
    this->index = conf->ptOffset + conf->ptIndex;
    this->hits = 0;
    this->misses = 0;
    this->pPageEntries.resize(vPages);
    entries.reserve(this->pPages);

    for (int i = 0; i < this->pPages; i++) pPageEntries.at(i) = this->pPages - (i + 1);
    // for (int i = 0; i < this->pPages; i++) cout << pPageEntries.at(i) << endl;
}

bool PageTable::processPTE(PageTableEntry *pte) {
    PageTableEntry *resident;
    uint64_t i = 0;

    // If there's a free PTE, insert the PTE
    if (this->entries.size() < this->vPages) {
        for (; i < this->entries.size(); i++) {
            resident = this->entries.at(i);

            // Move the PTE to the front of the entries vector on a hit
            if (resident->vpn == pte->vpn) {
                this->hits++;
                move(this->entries, i, 0);
                move(this->pPageEntries, i, 0);
                pte->ppn = resident->ppn;
                return true;
            } 
        }
        pte->ppn = this->pPageEntries.at(this->pPages - 1);
        move(this->pPageEntries, this->pPages - 1, 0);
        this->entries.insert(this->entries.begin(), pte);
    } else {
        for (i = 0; i < this->entries.size(); i++) {
            resident = this->entries.at(i);
            // Move the PTE to the front of the set on a read hit
            if (resident->vpn == pte->vpn) {
                this->hits++;
                move(this->entries, i, 0);
                move(this->pPageEntries, i, 0);
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

/* Translation Lookaside Buffer Functions */

TLB::TLB(Config *conf) {
    this->nSets = conf->tlbSets;
    this->setSize = conf->tlbSetSize;
    this->active = conf->tlb;
    this->offset = conf->ptOffset;
    this->index = log2(this->nSets) + conf->ptOffset;
    this->hits = 0;
    this->misses = 0;

    if (this->active) {
        this->sets.resize(this->nSets);

        for (int i = 0; i < this->nSets; i++) {
            this->sets.at(i) = new Set();
            this->sets.at(i)->blocks.reserve(this->setSize);
        }
    }
}

bool TLB::processData(Block *blk) {
    Set *s = NULL;
    Block  *resident = NULL;
    int i;
    // cout << hex << blk->index << endl;

    s = this->sets.at(blk->index % this->nSets);

    // If there's a free block, insert the block
    for (i = 0; i < s->blocks.size(); i++) {
        resident = s->blocks.at(i);

        if (resident->tag == blk->tag && resident->valid) {
            this->hits++;
            move(s->blocks, i, 0);
            return true;
        } 
    }

    if (s->blocks.size() < this->setSize) {
        if (resident != NULL && blk->ppn == resident->ppn) this->invalid = createBlock(resident->address, resident->offset, resident->index, resident->tag, resident->ppn, resident->valid);
        s->blocks.insert(s->blocks.begin(), blk);
    } else {
        if (blk->ppn == resident->ppn) this->invalid = createBlock(resident->address, resident->offset, resident->index, resident->tag, resident->ppn, resident->valid);
        this->replaceBlock(s, i - 1, blk, resident);
    } 
    this->misses++;
    return false;
}

void TLB::replaceBlock(Set *s, int i, Block *blk, Block *resident) {
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

void TLB::invalidateBlock(uint64_t ppn) {
    Set *s;
    Block *blk;
    int i;
    for (i = 0; i < this->nSets; i++) {
        s = this->sets.at(i);

        for (int j = 0; j < s->blocks.size(); j++) {
            blk = s->blocks.at(j);
            if (blk->ppn == ppn) blk->valid = false;
        }
    }
}

/* End Translation Lookaside Buffer Functions */

Memory::Memory(string configFile) {
    this->conf = new Config(configFile);
    this->dc = new DataCache(conf);
    this->L2 = new L2Cache(conf);
    this->pt = new PageTable(conf);
    this->tlb = new TLB(conf);
    this->memRefs = 0;
    this->diskRefs = 0;
}

void Memory::processData(string source) {
    string line;
    char action;
    uint64_t addr, offset, pageNumber, index, tag;
    bool dcHit, ptHit, tlbHit;
    Block *blk;
    PageTableEntry *pte;
    Set *s;
    Block *invalid = NULL, *temp;

    while(cin >> line) {
        blk = NULL;

        sscanf(line.c_str(), " %c:%lx", &action, &addr);

        if (action == 'R') this->reads++;
        else this->writes++;

        if (this->conf->vAddr) {
            offset = getPortion(addr, 0, this->conf->ptOffset);
            index = getPortion(addr, this->tlb->offset, this->tlb->index);
            tag = getPortion(addr, this->tlb->index, 64);   
            pte = createEntry(addr, this->conf);
            printf("%08lx %6lx %4lx", pte->vaddress, pte->vpn, pte->offset);

            if (!(ptHit = this->pt->processPTE(pte))) {
                this->diskRefs++;
            }


            pte->paddress = pte->offset | (pte->ppn << this->conf->ptOffset);

            blk = createBlock(pte->paddress, offset, index, tag, pte->ppn);

            if (this->tlb->active) {
                if (tlbHit = this->tlb->processData(blk)) {
                    printf(" %6lx %3lx hit  ", blk->tag, blk->index);
                    this->pt->hits--;
                } else {
                    printf(" %6lx %3lx miss ",  blk->tag, blk->index);
                    
                    if (this->tlb->invalid != NULL) {
                        for (int i = 0; i < conf->tlbSets; i++) {
                            s = this->tlb->sets.at(i);

                            for (int j = s->blocks.size() - 1; j >= 0; j--) {
                                temp = s->blocks.at(j);

                                if (temp->ppn = this->tlb->invalid->ppn) {
                                    // cout << "writing back DC line with tag " << hex << temp->tag << " and index " << hex << temp->index << " to L2 cache" << endl;
                                    move(s->blocks, j, s->blocks.size() - 1);
                                    this->diskRefs++;
                                }
                            }
                        }           
                    }
                }
            } else {
                printf(" %6s %3s      ",  "", "");
            }

            if (!tlbHit || !this->tlb->active) {
                if (ptHit) {
                    printf("hit  %4lx ", pte->ppn);
                } else {
                    printf("miss %4lx ", pte->ppn);
                }
            } else {
                printf("     %4lx ", pte->ppn);
            }
        } else {
            printf("%08lx %6s %4lx", pte->vaddress, "", pte->offset);
            printf(" %6s %3s           %4lx ", "", "",  pte->vpn);
        }


        addr = (this->conf->vAddr) ? pte->paddress : addr;
        offset = getPortion(addr, 0, this->dc->offset);
        index = getPortion(addr, this->dc->offset, this->dc->index);
        tag = getPortion(addr, this->dc->index, 64);

        blk = createBlock(addr, offset, index, tag, (this->conf->vAddr) ? pte->ppn : pte->vpn);

        if ((dcHit = this->dc->processBlock(blk, action))) {
            printf("%6lx %3lx hit  ", blk->tag, blk->index);
        } else {
            printf("%6lx %3lx miss ",  blk->tag, blk->index);
            
            
            if(this->conf->L2) {
                if (!tlbHit && !ptHit) {
                    // Check for any invalidated blocks and write them to L2 if found
                    for (int i = 0; i < conf->dcSets; i++) {
                        s = this->dc->sets.at(i);

                        for (int j = s->blocks.size() - 1; j >= 0; j--) {
                            temp = s->blocks.at(j);

                            if (temp->ppn == blk->ppn && blk != temp) {
                                index = getPortion(temp->address, this->L2->offset, this->L2->index);
                                tag = getPortion(temp->address, this->L2->index, 64);
                                invalid = createBlock(temp->address, offset, index, tag, temp->ppn, temp->valid);
                                invalid->dirty = temp->dirty;
                                // cout << "writing back DC line with tag " << hex << temp->tag << " and index " << hex << temp->index << " to L2 cache" << endl;

                                if (!this->L2->processBlock(invalid, action)) {
                                    this->memRefs++;

                                    if ( !tlbHit && !ptHit) {
                                        for (int i = 0; i < conf->L2Sets; i++) {
                                            s = this->L2->sets.at(i);

                                            for (int j = 0; j < s->blocks.size(); j++) {
                                                temp = s->blocks.at(j);
                                                if (temp->ppn == blk->ppn && blk != temp) {
                                                    // cout << "writing back L2 line with tag " << hex << temp->tag << " and index " << hex << temp->index << " to main memory" << endl;

                                                    // If there's an invalid L2 cache that needs to be written down, then it is simply another memory reference
                                                    this->memRefs++;
                                                }
                                            }
                                        }
                                    }

                                    // If L2 needs to write an evicted block to main memory, increment memRefs and reset 
                                    if (this->L2->writeBlock) {
                                        this->memRefs++;
                                        this->L2->writeBlock = false;
                                    }
                                }
                                invalid->valid = false;
                                move(s->blocks, j, s->blocks.size() - 1);
                            }
                        }
                    }
                }
                
                
                // If there is a block that needs to be written back from write-back policy, write it to L2 cache
                if (this->dc->write != NULL) {
                    index = getPortion(this->dc->write->address, this->L2->offset, this->L2->index);
                    tag = getPortion(this->dc->write->address, this->L2->index, 64);
                    blk = createBlock(this->dc->write->address, offset, index, tag, this->dc->write->ppn, this->dc->write->valid);
                    blk->dirty = this->dc->write->dirty;
                    // cout << "writing back DC line with tag " << hex << this->dc->write->tag << " and index " << hex << this->dc->write->index << " to L2 cache" << endl;
                    
                    // Handle cases where write back causes miss
                    if(!this->L2->processBlock(blk, action)) {
                        this->memRefs++;

                        if ( !tlbHit && !ptHit) {
                            for (int i = 0; i < conf->L2Sets; i++) {
                                s = this->L2->sets.at(i);

                                for (int j = s->blocks.size() - 1; j >= 0; j--) {
                                    temp = s->blocks.at(j);
                                    if (temp->ppn == blk->ppn && blk != temp) {
                                        // cout << "writing back L2 line with tag " << hex << temp->tag << " and index " << hex << temp->index << " to main memory" << endl;

                                        // If there's an invalid L2 cache that needs to be written down, then it is simply another memory reference
                                        this->memRefs++;
                                        move(s->blocks, j, s->blocks.size() - 1);
                                    }
                                }
                            }
                        }

                        // If L2 needs to write an evicted block to main memory, increment memRefs and reset 
                        if (this->L2->writeBlock) {
                            this->memRefs++;
                            this->L2->writeBlock = false;
                        }
                    }
                    this->dc->write = NULL;
                }
            } else {
                this->memRefs++;
                //  If L2 is inavtive, check for any invalidated blocks and write to main memory instead
                if (!tlbHit && !ptHit) {
                    for (int i = 0; i < conf->dcSets; i++) {
                        s = this->dc->sets.at(i);

                        for (int j = 0; j < s->blocks.size(); j++) {
                            temp = s->blocks.at(j);

                            if (temp->ppn == blk->ppn && blk != temp) {
                                this->memRefs++;
                            }
                        }
                    }
                }
                // If L2 is inactive and there is a block that needs to be written back, write to main memory instead
                if (this->dc->write != NULL) {
                    this->memRefs++;
                }
            }
        }
        


        if (this->conf->L2 && (!dcHit || this->dc->writeThrough)) {
            offset = getPortion(addr, 0, this->L2->offset);
            index = getPortion(addr, this->L2->offset, this->L2->index);
            tag = getPortion(addr, this->L2->index, 64);
            blk = createBlock(addr, offset, index, tag, (this->conf->vAddr) ? pte->ppn : pte->vpn);

            if (this->L2->processBlock(blk, action)) {
                printf("%6lx %3lx hit \n", blk->tag, blk->index);
            } else {
                printf("%6lx %3lx miss\n", blk->tag, blk->index);
                // This is the lowest cache so the next access would be to memory
                this->memRefs++;

                if ( !tlbHit && !ptHit) {
                    for (int i = 0; i < conf->L2Sets; i++) {
                        s = this->L2->sets.at(i);

                        for (int j = 0; j < s->blocks.size(); j++) {
                            temp = s->blocks.at(j);
                            if (temp->ppn == blk->ppn && blk != temp) {
                                // cout << "writing back L2 line with tag " << hex << temp->tag << " and index " << hex << temp->index << " to main memory" << endl;

                                // If there's an invalid L2 cache that needs to be written down, then it is simply another memory reference
                                this->memRefs++;
                            }
                        }
                    }
                }

                // If L2 needs to write an evicted block to main memory, increment memRefs and reset 
                if (this->L2->writeBlock) {
                    this->memRefs++;
                    this->L2->writeBlock = false;
                }
            }
        } else {
            printf("\n");
        }
    }

    cout << endl << "Simulation statistics" << endl << endl;
    cout << "dtlb hits        : " << this->tlb->hits << endl;
    cout << "dtlb misses      : " << this->tlb->misses << endl;
    if (this->tlb->active) {
        printf("dtlb hit ratio   : %.6f\n\n", (float) this->tlb->hits / (float) (this->tlb->hits + this->tlb->misses));
    } else {
        cout << "dtlb hit ratio   : N/A" << endl << endl;
    }

    cout << "pt hits          : " << this->pt->hits << endl;
    cout << "pt faults        : " << this->pt->misses << endl;
    if (this->conf->vAddr) {
        printf("pt hit ratio     : %.6f\n\n", (float) this->pt->hits / (float) (this->pt->hits + this->pt->misses));
    } else {
        cout << "pt hit ratio     : N/A" << endl << endl;
    }
    
    cout << "dc hits          : " << this->dc->hits << endl;
    cout << "dc misses        : " << this->dc->misses << endl;
    printf("dc hit ratio     : %.6f\n\n", (float) this->dc->hits / (float) (this->dc->hits + this->dc->misses));
    
    cout << "L2 hits          : " << this->L2->hits << endl;
    cout << "L2 misses        : " << this->L2->misses << endl;
    if (this->L2->active) {
        printf("L2 hit ratio     : %.6f\n\n", (float) this->L2->hits / (float) (this->L2->hits + this->L2->misses));
    } else {
        cout << "L2 hit ratio     : N/A" << endl << endl;
    }

    cout << "Total reads      : " << this->reads << endl;
    cout << "Total writes     : " << this->writes << endl;
    printf("Ratio of reads   : %.6f\n\n", (float) this->reads / (float) (this->reads + this->writes));

    cout << "main memory refs : " << this->memRefs << endl;
    cout << "page table refs  : " << this->pt->hits + this->pt->misses << endl;
    cout << "disk refs        : " << this->diskRefs << endl;
}

Block *createBlock(uint64_t addr, uint64_t offset, uint64_t index, uint64_t tag, uint64_t ppn) {
    Block *blk = new Block();
    blk->address = addr;
    blk->offset = offset;
    blk->index = index;
    blk->tag = tag;
    blk->ppn = ppn;
    blk->valid = true;
    blk->dirty = false;
    return blk;
}

Block *createBlock(uint64_t addr, uint64_t offset, uint64_t index, uint64_t tag, uint64_t ppn, bool valid) {
    Block *blk = new Block();
    blk->address = addr;
    blk->offset = offset;
    blk->index = index;
    blk->tag = tag;
    blk->ppn = ppn;
    blk->valid = valid;
    blk->dirty = false;
    return blk;
}

PageTableEntry *createEntry(uint64_t address, Config *conf) {
    PageTableEntry *entry = new PageTableEntry();
    entry->vaddress = address;
    entry->paddress = 0;
    entry->offset = getPortion(address, 0, conf->ptOffset);
    entry->vpn = getPortion(address, conf->ptOffset, log2(conf->ptVPages) + conf->ptOffset);
    // entry->used = false;
    // entry->resident = false;
    entry->dirty = false;

    return entry;
}
