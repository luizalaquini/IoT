/*
LAB 2 - SEGURANÇA EM ESP8266
Disciplina: Tópicos em Informática - Internet das Coisas 
Docente: Vinicius Fernandes Soares Mota
Discente: Luiza Batista Laquini
*/

#include <Crypto.h>
#include <SHA256.h>
#include <SHA512.h>
#include <SHA3.h>
#include <string.h>
#include <time.h>

#define HASH_SIZE_256 32
#define HASH_SIZE_512 64

#define TIMES 10

#define K256 0
#define K512 1

// STRUCTS AND FUNCTIONS

// Can be used for Sha256 and Sha3_256
struct TestHashVector_256 {
  const char *name;
  const char *data;
  uint8_t hash[HASH_SIZE_256];
};

// Can be used for Sha512 and Sha3_512
struct TestHashVector_512 {
    const char *name;
    const char *data;
    uint8_t hash[HASH_SIZE_512];
};

// Sha256
static TestHashVector_256 const testVectorSHA256 = {
  "SHA-256 abc",
  "abc",
  {0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
    0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
    0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
    0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad}
};

// Sha512
static TestHashVector_512 const testVectorSHA512 = {
  "SHA-512 abc",
  "abc",
  {0xdd, 0xaf, 0x35, 0xa1, 0x93, 0x61, 0x7a, 0xba,
    0xcc, 0x41, 0x73, 0x49, 0xae, 0x20, 0x41, 0x31,
    0x12, 0xe6, 0xfa, 0x4e, 0x89, 0xa9, 0x7e, 0xa2,
    0x0a, 0x9e, 0xee, 0xe6, 0x4b, 0x55, 0xd3, 0x9a,
    0x21, 0x92, 0x99, 0x2a, 0x27, 0x4f, 0xc1, 0xa8,
    0x36, 0xba, 0x3c, 0x23, 0xa3, 0xfe, 0xeb, 0xbd,
    0x45, 0x4d, 0x44, 0x23, 0x64, 0x3c, 0xe8, 0x0e,
    0x2a, 0x9a, 0xc9, 0x4f, 0xa5, 0x4c, 0xa4, 0x9f}
};

// Sha3_256
static TestHashVector_256 const testVectorSHA3_256 = {
  "SHA3-256 abc",
  "abc",
  {0x3A, 0x98, 0x5D, 0xA7, 0x4F, 0xE2, 0x25, 0xB2,
    0x04, 0x5C, 0x17, 0x2D, 0x6B, 0xD3, 0x90, 0xBD,
    0x85, 0x5F, 0x08, 0x6E, 0x3E, 0x9D, 0x52, 0x5B,
    0x46, 0xBF, 0xE2, 0x45, 0x11, 0x43, 0x15, 0x32}

};

// Sha3_512
static TestHashVector_512 const testVectorSHA3_512 = {
  "SHA3-512 #1",
  "abc",
  {0xB7, 0x51, 0x85, 0x0B, 0x1A, 0x57, 0x16, 0x8A,
    0x56, 0x93, 0xCD, 0x92, 0x4B, 0x6B, 0x09, 0x6E,
    0x08, 0xF6, 0x21, 0x82, 0x74, 0x44, 0xF7, 0x0D,
    0x88, 0x4F, 0x5D, 0x02, 0x40, 0xD2, 0x71, 0x2E,
    0x10, 0xE1, 0x16, 0xE9, 0x19, 0x2A, 0xF3, 0xC9,
    0x1A, 0x7E, 0xC5, 0x76, 0x47, 0xE3, 0x93, 0x40,
    0x57, 0x34, 0x0B, 0x4C, 0xF4, 0x08, 0xD5, 0xA5,
    0x65, 0x92, 0xF8, 0x27, 0x4E, 0xEC, 0x53, 0xF0}
};

// Test Function
bool testHash(Hash *hash, char *data, size_t inc, int key) {
  size_t size = strlen(data);
  size_t posn, len;
  int HASH_SIZE;
  if (key==K256)
    HASH_SIZE = HASH_SIZE_256;
  else if (key==K512)
    HASH_SIZE = HASH_SIZE_512;
  
  uint8_t value[HASH_SIZE];
  char texthash[2*HASH_SIZE+1];

  hash->reset();
  for (posn = 0; posn < size; posn += inc) {
    len = size - posn;
    if (len > inc)
      len = inc;
    hash->update(data + posn, len);
  }
  hash->finalize(value, sizeof(value));

  for(int i=0; i<HASH_SIZE; ++i)
    sprintf(texthash+2*i, "%02X", value[i]);
  Serial.println(texthash);
    
  return true;
}

// TESTS

SHA256 sha256;
SHA512 sha512;
SHA3_256 sha3_256;
SHA3_512 sha3_512;

unsigned long start;
unsigned long end;
unsigned long avg = 0;
int qtd;
char data[4] = "abc";

void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.println("=================================== NOTE ===================================");
  Serial.println("Test data = 'abc' for all hashes");
  Serial.println("============================================================================");
  Serial.println();

  // SHA256
  Serial.println("============================== TESTING SHA256 ==============================");
  Serial.print("State Size ... ");
  Serial.println(sizeof(SHA256));
  Serial.println("Test Vectors: ");

  for(int i=0; i<TIMES; i++){
    start = millis();
    testHash(&sha256, data, strlen(data), K256); // test and print vector
    end = millis() - start;

    if (i==0) 
      continue; // ignore first because is always 1
    else
      avg += end;      
    
    qtd = i;
  }
  avg /= qtd;
  Serial.print("Media tempo SHA256: ");
  Serial.println(avg);
  Serial.println();

  // SHA512
  Serial.println("============================== TESTING SHA512 ==============================");
  Serial.print("State Size ... ");
  Serial.println(sizeof(SHA512));
  Serial.println("Test Vectors: ");

  for(int i=0; i<TIMES; i++){
    start = millis();
    testHash(&sha512, data, strlen(data), K512); // test and print vector
    end = millis() - start;

    if (i==0) 
      continue; // ignore first because is always 1
    else
      avg += end;      
    
    qtd = i;
  }
  avg /= qtd;
  Serial.print("Media tempo SHA512: ");
  Serial.println(avg);
  Serial.println();

  // SHA3_256
  Serial.println("============================= TESTING SHA3_256 =============================");
  Serial.print("State Size ... ");
  Serial.println(sizeof(SHA3_256));
  Serial.println("Test Vectors: ");

  for(int i=0; i<TIMES; i++){
    start = millis();
    testHash(&sha3_256, data, strlen(data), K256); // test and print vector
    end = millis() - start;

    if (i==0) 
      continue; // ignore first because is always 1
    else
      avg += end;      
    
    qtd = i;
  }
  avg /= qtd;
  Serial.print("Media tempo SHA3_256: ");
  Serial.println(avg);
  Serial.println();

  // SHA3_512
  Serial.println("============================= TESTING SHA3_512 =============================");
  Serial.print("State Size ... ");
  Serial.println(sizeof(SHA3_512));
  Serial.println("Test Vectors: ");

  for(int i=0; i<TIMES; i++){
    start = millis();
    testHash(&sha3_512, data, strlen(data), K512); // test and print vector
    end = millis() - start;

    if (i==0) 
      continue; // ignore first because is always 1
    else
      avg += end;      
    
    qtd = i;
  }
  avg /= qtd;
  Serial.print("Media tempo SHA3_512: ");
  Serial.println(avg);
  Serial.println();
} 

void loop(){}