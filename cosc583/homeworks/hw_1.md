**1) MixColumns pseudocode**
```
void mixColumns(byte state[4][Nb]) {
    byte temp[4][Nb]
    byte result

    temp = state

    for (i = 0; i < 4; i++) {
        for (j = 0; j < Nb; j++) {
            result = 0
            for (k = 0; k < Nb; k++) {
                result = result ^ ffMultiply(temp[k][i], POLY_COLS[j][k])
            }
            state[j][i] = result
        }
    }
}

```
**2) ffMultiply**
```
uint8_t ffMultiply(uint8_t a, uint8_t b) {
    uint8_t r = 0x00, t = 0x00;

    for(int i = 0; i < 8; i++) {
        if (b & (1 << i)) {
            r =  r ^ a;
        }
        t = a << 1;
        a =  (a & 0x80) ?  t ^ 0x1b : t;
    }

    return r;
}
```
