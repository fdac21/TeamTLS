#include "stdio.h"
#include "stdlib.h"
#include "memhier.h"

int main(int argc, char const *argv[])
{
    Config *conf;

    conf = readConf("trace.config");

    return 0;
}



void readLine(char *str, FILE *f) {
    if (fgets(str, SSIZE, f) == NULL) {
        perror("Error reading file.\n");
        exit(-1);
    }
}

void validateOption(char choice, char *option) {
    if (choice != 'n' && choice != 'y') fprintf(stderr, "Choice for %s must be 'y' or 'n'\n", option);
}

Config *readConf(char *fileName) {
    FILE *f;
    Config *conf = (Config *) malloc(sizeof(Config));
    char str[SSIZE];

    f = fopen("trace.config", "r");
    if (f == NULL) {
        perror("Error opening config file.\n");
        exit(-1);
    }

    // Skip the top line of the config file
    readLine(str, f);

    // Get number of TLB sets
    readLine(str, f);
    sscanf(str, "Number of sets: %d", &conf->tlbSets);

    if (conf->tlbSets < 0 || conf->tlbSets >  256) {
        perror("Set count can only be between 0 and 256\n");
        exit(-1);
    }

    if (conf->tlbSets  % 2 != 0) {
        perror("Set count must be a power of 2\n");
        exit(-1);
    }


    // Get tlb set size
    readLine(str, f);
    sscanf(str, "Set size: %d", &conf->tlbSetSize);

    //  Skip line
    readLine(str, f);
    readLine(str, f);

    // Get # of virtual pages
    readLine(str, f);
    sscanf(str, "Number of virtual pages: %d", &conf->ptVPages);
    if (conf->ptVPages < 0 || conf->ptVPages > 8192) {
        perror("Number of virtual pages can only be between 0 and 8192");
        exit(-1);
    }
    if (conf->ptVPages  % 2 != 0) {
        perror("Number of virtual pages must be a power of 2\n");
        exit(-1);
    }

    // Get # of phyrical pages
    readLine(str, f);
    sscanf(str, "Number of physical pages: %d", &conf->ptPPages);
    if (conf->ptPPages < 0 || conf->ptPPages > 1024) {
        perror("Number of physical pages can only be between 0 and 1024");
        exit(-1);
    }
    if (conf->ptPPages  % 2 != 0) {
        perror("Number of physical pages must be a power of 2\n");
        exit(-1);
    }


    readLine(str, f);
    sscanf(str, "Page size: %d", &conf->ptPageSize);


    readLine(str, f);
    readLine(str, f);

    // Get # of data cache sets
    readLine(str, f);
    sscanf(str, "Number of sets: %d", &conf->dcSets);
   
    if (conf->dcSets < 0 || conf->dcSets >  256) {
        perror("Set count can only be between 0 and 256\n");
        exit(-1);
    }

    if (conf->dcSets  % 2 != 0) {
        perror("Set count must be a power of 2\n");
        exit(-1);
    }
    // Get data cache set size
    readLine(str, f);
    sscanf(str, "Set size: %d", &conf->dcSetSize);

    // Get data cache line size
    readLine(str, f);
    sscanf(str, "Line size: %d", &conf->dcLineSize);
    if (conf->dcLineSize < 8) {
        perror("Data line size for DC must be greater than 8.\n");
        exit(-1);
    }

    // Get data cache write through option
    readLine(str, f);
    sscanf(str, "Write through/no write allocate: %c", &conf->dcWriteThrough);
    validateOption(conf->dcWriteThrough, "Data Cache Write Through");

    readLine(str, f);
    readLine(str, f);

    // Get # of L2 cache sets
    readLine(str, f);
    sscanf(str, "Number of sets: %d", &conf->L2Sets);

    // Get L2 cache set size
    readLine(str, f);
    sscanf(str, "Set size: %d", &conf->L2SetSize);

    // Get L2 cache line size
    readLine(str, f);
    sscanf(str, "Line size: %d", &conf->L2LineSize);

    // Get L2 cache write through option
    readLine(str, f);
    sscanf(str, "Write through/no write allocate: %c", &conf->L2WriteThrough);
    validateOption(conf->L2WriteThrough, "L2 Cache Write Through");

    readLine(str, f);

    // Get V addr option
    readLine(str, f);
    sscanf(str, "Virtual addresses: %c", &conf->vAddr);
    validateOption(conf->vAddr, "Virtual Addresses");

    // Get TLB option
    readLine(str, f);
    sscanf(str, "TLB: %c", &conf->tlb);
    validateOption(conf->tlb, "TLB");

    // Get L2 option
    readLine(str, f);
    sscanf(str, "L2 cache: %c", &conf->L2);
    validateOption(conf->L2, "L2 Cache");

    fclose(f);

}

void printConf(Config *conf) {
    if (conf->tlb == 'y') {
        printf("Data TLB contains %d sets.\n", conf->tlbSets);
        printf("Each set contains %d entries.\n", conf->tlbSetSize);
        printf("Number of bits used for the index is %d.\n", conf->tlbSetSize);
    }

    if (conf->vAddr == 'y') {
        printf("Number of virtual pages is %d.\n", conf->ptVPages);
    }

    printf("Number of physical pages is %d.\n", conf->ptPPages);
    printf("Each page contains %d bytes.\n", conf->ptPageSize);
    // TODO Figure out size sources
    printf("Number of bits used for the page table index is %d.\n", 6);
    printf("Number of bits used for the page offset is %d.\n", 8);

    printf("D-cache contains %d sets.\n", conf->dcSets);
    printf("Each set contains %d entries.\n", conf->dcSetSize);
    printf("Each line is %d bytes.\n", conf->dcLineSize);
    if (conf->dcWriteThrough  == 'y')  printf("The cache uses a write-allocate and write-back policy.\n");
    // TODO Figure out size sources
    printf("Number of bits used for the index is %d.\n", 2);
    printf("Number of bits used for the offset is %d.\n", 4);

    if (conf->L2 == 'y') {
        printf("L2-cache contains %d sets.\n", conf->L2Sets);
        printf("Each set contains %d entries.\n", conf->L2SetSize);
        printf("Each line is %d bytes.\n", conf->L2LineSize);
        if (conf->L2WriteThrough  == 'y')  printf("The cache uses a write-allocate and write-back policy.\n");
        // TODO Figure out size sources
        printf("Number of bits used for the index is %d.\n", 2);
        printf("Number of bits used for the offset is %d.\n", 4);
    }

}