#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define AES_block_size 16 
#define Nk 4       // Number of 32 bits words in the key
#define Nr 10      // Number of rounds
#define expandedKeySize 176

void keyExpansion(unsigned char[], unsigned char[]);
void createRoundkey(unsigned char*, unsigned char[]);
void addRoundKey(unsigned char[], unsigned char[]);

void ECB_encrypt(unsigned char[], unsigned char[], unsigned char[], unsigned int);
void encryption(unsigned char[],unsigned char[],unsigned char[]);
void subBytes(unsigned char[]);
void shiftRows(unsigned char[]);
void left_shift(unsigned char*, int);
void mixColumns(unsigned char[]);
void mixColumn(unsigned char[]);
unsigned char galois_multiplication(unsigned char, unsigned char);

unsigned char Sbox[256] =   {
//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F

unsigned char rSbox[256] =
{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb
, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb
, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e
, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25
, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92
, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84
, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06
, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b
, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73
, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e
, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b
, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4
, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f
, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef
, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61
, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };


uint8_t Rcon[11] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };

int main() {
	unsigned char plaintext[10000];   
	puts("\nEnter the text you want to encrypt using AES (Press enter to stop): ");    // prompting the user to enter text to encrypt
	fgets(plaintext, sizeof(plaintext), stdin);    // reading the text from the user
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Replacing newline with null terminator
    unsigned int plaintextLen = strlen(plaintext);

    unsigned char key[AES_block_size + 1];    // +1 for null character
    printf("\nEnter 16 digit secret key to encrypt your text: ");   
    scanf("%16s", key);
    // printf("Key in hexadecimal form: ");
    // for (int i = 0; i < AES_block_size; i++)
    //     printf("%02x ", key[i]);

    /* Array to hold the expanded key */
    unsigned char expandedKey[expandedKeySize]; // 16 bytes * 11 rounds = 176 bytes
    keyExpansion(key, expandedKey);
    // puts("\nExpanded Key:");
    // for (int i = 0; i < expandedKeySize; i++) 
    //     printf("%02x %c", expandedKey[i], ((i + 1) % 16 == 0) ? '\n' : ' ');
    // puts("");

    unsigned int ciphertextLen = ((plaintextLen + AES_block_size - 1) / AES_block_size) * AES_block_size;
    unsigned char *ciphertext = (unsigned char*)malloc(ciphertextLen + 1);    // +1 for null character
    if (ciphertext == NULL) {
        // Handling memory allocation failure
        printf("Memory allocation failed.\n");
        return 1;
    }
    ECB_encrypt(plaintext, expandedKey, ciphertext, plaintextLen);
    ciphertext[ciphertextLen] = '\0';
    puts("\nCiphertext:");
    puts("In hexadecimal form: ");
    for (int i = 0; i < ciphertextLen; i++)
        printf("%02x", ciphertext[i]);
    puts("");
    puts("\nIn base64 form: ");
    printf("\"  %s  \" \n\n", ciphertext);
    // Free allocated memory
    free(ciphertext);
    return 0;
}

void keyExpansion(unsigned char key[], unsigned char expandedKey[]) {
    uint32_t w[Nk * (Nr + 1)];        // Expanded key schedule
    uint32_t temp;
    // copy the original key to the expanded key schedule
    for (int i = 0; i < Nk; i++)
        w[i] = key[4 * i] << 24 | key[4 * i + 1] << 16 | key[4 * i + 2] << 8 | key[4 * i + 3];
    for (int i = Nk; i < Nk * (Nr + 1); i++) {
        temp = w[i - 1];
        if (i % Nk == 0) {
            temp = (Sbox[(temp >> 8) & 0xFF] << 24) | (Sbox[(temp >> 16) & 0xFF] << 16) |
                   (Sbox[(temp >> 24) & 0xFF] << 8) | Sbox[temp & 0xFF];
            temp ^= Rcon[i / Nk - 1] << 24;
        }
        w[i] = w[i - Nk] ^ temp;
    }
    // copy the expanded key back to the output expanded key
    for (int i = 0; i < Nk * (Nr + 1); i++) {
        expandedKey[4 * i] = (w[i] >> 24) & 0xFF;
        expandedKey[4 * i + 1] = (w[i] >> 16) & 0xFF;
        expandedKey[4 * i + 2] = (w[i] >> 8) & 0xFF;
        expandedKey[4 * i + 3] = w[i] & 0xFF;
    }
}

// Function for ECB mode to break the plaintext into 16 byte blocks for AES encryption 
void ECB_encrypt(unsigned char plaintext[], unsigned char expandedKey[], unsigned char ciphertext[], unsigned int length) {
    int numBlocks = length / AES_block_size;
    int extraBytes = length % AES_block_size;
    int idx = 0;
    for (int i = 0; i < numBlocks; ++i) {
        encryption(plaintext + idx, expandedKey, ciphertext + idx);
        idx += AES_block_size;
    }
    // Encrypt the remaining bytes (if any)
    if (extraBytes > 0) {
        unsigned char paddedBlock[AES_block_size];
        memset(paddedBlock, 0, AES_block_size);
        memcpy(paddedBlock, plaintext + idx, extraBytes);
        encryption(paddedBlock, expandedKey, ciphertext + idx);
    }
}

void encryption(unsigned char plaintext[], unsigned char expandedKey[], unsigned char ciphertext[]) {
    /* the 128 bit state to encode */
    unsigned char state[AES_block_size];
    /* copy the plaintext into the state */
    for (int i = 0; i < 4; i++) {    // iterate over the columns
        for (int j = 0; j < 4; j++)    // iterate over the rows
            state[(i + (4 * j))] = plaintext[(4 * i) + j];
    }
    unsigned char roundKey[AES_block_size];
    createRoundkey(expandedKey, roundKey);
    addRoundKey(state, roundKey);
    for (int i = 1; i < Nr; i++) {
        createRoundkey(expandedKey + 16 * i, roundKey);
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, roundKey);
    }
    createRoundkey(expandedKey + 16 * Nr, roundKey);
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKey);
    /* unmap the state into the ciphertext */
    for (int i = 0; i < 4; i++)    // iterate over the columns
        for (int j = 0; j < 4; j++)    // iterate over the rows
            ciphertext[(4 * i) + j] = state[i + (4 * j)];
}

void createRoundkey(unsigned char *expandedKey, unsigned char roundKey[]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            roundKey[i + (4 * j)] = expandedKey[(4 * i) + j];
}
void addRoundKey(unsigned char state[], unsigned char roundKey[]) {
    for (int i = 0; i < AES_block_size; i++) 
        state[i] = state[i] ^ roundKey[i];
}

void subBytes(unsigned char state[]) {
    for (int i = 0; i < AES_block_size; i++)
        state[i] = Sbox[state[i]];
}

void shiftRows(unsigned char state[]) {
    for (int i = 1; i < 4; i++)
        left_shift(state + i * 4, i);   // perform left shift on each row with offsets
}
// function to perform left circular shift on a row by i positions
void left_shift(unsigned char *state, int i) {
    unsigned char temp;
    for (int j = 0; j < i; j++) {
        temp = state[0];
        state[0] = state[1];
        state[1] = state[2];
        state[2] = state[3];
        state[3] = temp;
    }
}

void mixColumns(unsigned char state[]) {
    unsigned char column[4];
    for (int i = 0; i < 4; i++) {    // iterate over the 4 columns
        /* construct one column by iterating over the 4 rows */
        for (int j = 0; j < 4; j++) {
            column[j] = state[(j * 4) + i];
        }
        /* apply the mixColumn on one column */
        mixColumn(column);
        /* put the values back into the state */
        for (int j = 0; j < 4; j++) {
            state[(j * 4) + i] = column[j];
        }
    }
}
void mixColumn(unsigned char column[]) {
    unsigned char cpy[4];
    for(int i = 0; i < 4; i++) {
        cpy[i] = column[i];
    }
    column[0] = galois_multiplication(cpy[0],2) ^ galois_multiplication(cpy[3],1) ^ galois_multiplication(cpy[2],1) ^ galois_multiplication(cpy[1],3);
    column[1] = galois_multiplication(cpy[1],2) ^ galois_multiplication(cpy[0],1) ^ galois_multiplication(cpy[3],1) ^ galois_multiplication(cpy[2],3);
    column[2] = galois_multiplication(cpy[2],2) ^ galois_multiplication(cpy[1],1) ^ galois_multiplication(cpy[0],1) ^ galois_multiplication(cpy[3],3);
    column[3] = galois_multiplication(cpy[3],2) ^ galois_multiplication(cpy[2],1) ^ galois_multiplication(cpy[1],1) ^ galois_multiplication(cpy[0],3);
}
// Galois Multiplication of two bytes in GF(2^8)
unsigned char galois_multiplication(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char hi_bit_set;
    for(int counter = 0; counter < 8; counter++) {
        if((b & 1) == 1)
            p ^= a;
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if(hi_bit_set == 0x80)
            a ^= 0x1b;
        b >>= 1;
    }
    return p;
}
