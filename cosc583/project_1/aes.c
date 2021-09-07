#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "aes.h"


// UTILS
void printW(word w) {
    for (int i = 0; i < 4; i++) {
        printf("%02x", w[i]);
    }
}

void printKey(uint8 *key, int keySize, char *str) {

    for (int i = 0; i < keySize; i++) {
        printf("%02x", key[i]);
    }
    printf("\n");
}

void printRoundKey(word *w) {
    for (int i = 0; i < Nb * (1+Nr); i++) {
        printf("Word %d: ", i);
        printW(w[i]);
    }
}

void printState(uint8 state[4][Nb]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02x", state[j][i]);
        }
    }
    printf("\n");
}

// OPS
uint8 ffAdd(uint8 a, uint8 b) {
    return a ^ b;
}

uint8 xtime(uint8 a) {
    uint8 r = a << 1;
    return  (a & 0x80) ?  ffAdd(r, 0x1b) : r;
}

uint8 ffMultiply(uint8 a, uint8 b) {
    uint8 r = 0x00;

    for(int i = 0; i < 8; i++) {
        if (b & (1 << i)) {
            r =  ffAdd(r, a);
        }
        a = xtime(a);
    }

    return r;
}

// KEY EXPANSION
void key_expansion(uint8 *key, word *w) {
    word temp;

    int i = 0;

    for( ; i < Nk; i++) {
        w[i][0] =  key[4*i];
        w[i][1] = key[4*i + 1];
        w[i][2] = key[4*i + 2];
        w[i][3] = key[4*i + 3];
    }

    for( ; i < Nb * (Nr + 1); i++) {
        for (int j = 0; j < 4; j++) temp[j] = w[i - 1][j];
            
        if (i % Nk == 0) {
            rotWord(temp);
            subWord(temp);

            // TODO: Verify that RCON Index is correct at i - 1 
            temp[0] = ffAdd(temp[0], RCON[(i - 1) / Nk]);
        } else if (Nk > 6 && i % Nk == 4) {
            subWord(temp);
        }
        
        for (int j = 0; j < 4; j++) w[i][j] = temp[j] ^ w[i - Nk][j];
        // printW(w[i]);
    }
}

void subWord(word w) {
    for (int i = 0; i < 4; i++) {
        w[i] = S_BOX[w[i]];
    }
}

void rotWord(word w) {
    uint8 byte = w[0] ;

    for (int i = 0; i < 4; i++) {
        w[i] = (i < 3) ? w[i + 1] : byte;
    }
}

void printRoundInfo(char *type, int round) {
    printf("round[%2d].%-10s", round, type);
}

// CIPHER
void cipher(uint8 in[4][Nb], uint8 out[4][Nb], word *w) {
    int round = 0;
    uint8 state[4][Nb];

    memcpy(state, in, sizeof(uint8) * 4 * Nb);

    printRoundInfo("input", round);
    printState(state);
    // round 0
    printRoundInfo("k_sch", round);
    addRoundKey(state, w, round);


    for (round = 1; round < Nr; round++) {
        printRoundInfo("start ", round);
        printState(state);

        subBytes(state);
        printRoundInfo("s_box", round);
        printState(state);

        shiftRows(state);
        printRoundInfo("s_row", round);
        printState(state);

        mixColumns(state);
        printRoundInfo("m_col", round);
        printState(state);
        printRoundInfo("k_sch", round);
        addRoundKey(state, w, round);
    }

    printRoundInfo("start", round);
    printState(state);

    subBytes(state);

    printRoundInfo("s_box", round);
    printState(state);

    shiftRows(state);
    printRoundInfo("s_row", round);
    printState(state);

    printRoundInfo("k_sch", round);
    addRoundKey(state, w, round);

    memcpy(out, state, sizeof(uint8) * 4 * Nb);
    printRoundInfo("output", round);
    printState(out);
}

void subBytes(uint8 state[4][Nb]) {
    for (int i = 0; i < 4; i++) {
        for(int j = 0; j < Nb; j++) {
            state[i][j] = S_BOX[state[i][j]];
        }
    }
}

void shiftRows(uint8 state[4][Nb]) {
   int i, j;
   // Temp row for overwriting values  in state
   uint8 *row = (uint8 *) malloc(sizeof(uint8) * Nb);

   if (row == NULL) {
       fprintf(stderr, "Row failed to be allocated");
   }

    for (i = 0; i < 4; i++) {
        memcpy(row, state[i], sizeof(uint8) * Nb);
        for (j = 0; j < Nb; j++) {
            state[i][j] = row[(j + i) % Nb];
        }
    }

    free(row);
}

uint8 matrixMultiply(uint8 state[4][Nb], const uint8 row[4], int col) {
    uint8 r = 0;
    for (int i = 0; i < 4; i++) {
        r = ffAdd(r, ffMultiply(state[i][col], row[i]));
    }

    return r;
}

void mixColumns(uint8 state[4][Nb]) {
    uint8 temp[4][Nb];

    memcpy(temp, state, sizeof(uint8) * 4 * Nb);
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < Nb; j++) {
            state[j][i] = matrixMultiply(temp, POLY_COLS[j], i);
        }
    }
}

void addRoundKey(uint8 state[4][Nb], word *w, int round) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < Nb; j++) {
            state[i][j] = ffAdd(state[i][j], w[j + round * Nb][i]);
        }
        printW(w[i + round * Nb]);
    }
    printf("\n");
}

void invCipher(uint8 in[4][Nb], uint8 out[4][Nb], word *w) {
    int round = Nr;
    uint8 state[4][Nb];

    memcpy(state, in, sizeof(uint8) * 4 *  Nb);

    printRoundInfo("iinput", Nr - round);
    printState(state);
    printRoundInfo("ik_sch", Nr - round);
    addRoundKey(state, w, round);

    for (round = Nr - 1; round > 0; round--) {
        printRoundInfo("istart", Nr - round);
        printState(state);
        
        invShiftRows(state);
        printRoundInfo("is_row", Nr - round);
        printState(state);

        invSubBytes(state);
        printRoundInfo("is_box", Nr - round);
        printState(state);
        
        printRoundInfo("ik_sch", Nr - round);
        addRoundKey(state, w, round);
        printRoundInfo("ik_add", Nr - round);
        printState(state);
        invMixColumns(state);
    }

    printRoundInfo("istart", Nr - round);
    printState(state);
        

    invShiftRows(state);
    printRoundInfo("is_row", Nr - round);
    printState(state);    
    
    invSubBytes(state);
    printRoundInfo("is_box", Nr - round);
    printState(state);

    printRoundInfo("ik_sch", Nr - round);
    addRoundKey(state, w, round);

    memcpy(out, state, sizeof(uint8) * 4 * Nb);
    printRoundInfo("ioutput", Nr - round);
    printState(out);
}

void invSubBytes(uint8 state[4][Nb]) {
    for (int i = 0; i < 4; i++) {
        for(int j = 0; j < Nb; j++) {
            state[i][j] = IS_BOX[state[i][j]];
        }
    }
}

void invShiftRows(uint8 state[4][Nb]) {
    int i, j;
   // Temp row for overwriting values  in state
   uint8 *row = (uint8 *) malloc(sizeof(uint8) * Nb);

   if (row == NULL) {
       fprintf(stderr, "Row failed to be allocated");
   }

    for (i = 0; i < 4; i++) {
        memcpy(row, state[i], sizeof(uint8) * Nb);
        for (j = 0; j < Nb; j++) {
            state[i][(j + i) % Nb] = row[j];
        }
    }

    free(row);
}

void invMixColumns(uint8 state[4][Nb]) {
    uint8 temp[4][Nb];

    memcpy(temp, state, sizeof(uint8) * 4 * Nb);
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < Nb; j++) {
            state[j][i] = matrixMultiply(temp, INV_POLY_COLS[j], i);
        }
    }
}

void aes(uint8 in[4][Nb], uint8 *key, int keySize) {
    uint8 out[4][4];
    word w[Nb*(Nr+1)];
    uint8 decrypt[4][4];
    char str[100];


    printf("%-20s", "PLAINTEXT:");
    printState(in);

    printf("%-20s","KEY:");
    printKey(key, keySize, str);
    // printf("%s\n", str);
    printf("\n");

    key_expansion(key, w);
    // Cipher Testing
    printf("CIPHER (ENCRYPT):\n");
    cipher(in, out, w);
    printf("\n");
    // Inv Cipher Testing
    printf("INVERSE CIPHER (DECRYPT):\n");
    invCipher(out, decrypt, w);
}

int main(int argc, char const *argv[])
{

    uint8 in[4][4] = {
        {0x00, 0x44, 0x88, 0xcc},
        {0x11, 0x55, 0x99, 0xdd},
        {0x22, 0x66, 0xaa, 0xee},
        {0x33, 0x77, 0xbb, 0xff}
    };

    uint8 key_16[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    };

    printf("C.1   AES-128 (Nk=%d, Nr=%d)\n\n", Nk, Nr);
    aes(in, key_16, 16);
    printf("\n\n");
    Nk = 6;
    Nr = 12;

    uint8 key_24[24] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    };
    printf("C.2   AES-192 (Nk=%d, Nr=%d)\n\n", Nk, Nr);
    aes(in, key_24, 24);
    printf("\n\n");
    Nk = 8;
    Nr = 14;

    uint8 key_32[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    };
    printf("C.3   AES-256 (Nk=%d, Nr=%d)\n\n", Nk, Nr);
    aes(in, key_32, 32);

    return 0;
}
