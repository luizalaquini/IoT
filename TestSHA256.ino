/*
 * Copyright (C) 2015 Southern Storm Software, Pty Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
This example runs tests on the SHA256 implementation to verify correct behaviour.
*/

#include <Crypto.h>
#include <SHA256.h>
#include <string.h>
#include <time.h>

#define HASH_SIZE 32


struct TestHashVector
{
    const char *name;
    const char *key;
    const char *data;
    uint8_t hash[HASH_SIZE];
};

static TestHashVector const testVectorSHA256_1 = {
    "SHA-256 #1",
    0,
    "abc",
    {0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
     0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
     0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
     0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad}
};
static TestHashVector const testVectorSHA256_2 = {
    "SHA-256 #2",
    0,
    "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    {0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8,
     0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39,
     0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67,
     0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1}
};

bool testHash(Hash *hash, char *data, size_t inc)
{
    size_t size = strlen(data);
    size_t posn, len;
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

SHA256 sha256;

unsigned long start;
unsigned long end;
unsigned long avg = 0;

void setup()
{
    Serial.begin(9600);

    Serial.println();

  int i;
  for(i=0; i<10; i++){
    start = millis();

    Serial.print("State Size ...");
    Serial.println(sizeof(SHA256));
    Serial.println();

    Serial.println("Test Vectors:");
    char *data = "abc";
      
    testHash(&sha256, data, strlen(data)); //Pode se alerar o tamanho dos blocos
    
    Serial.println();
      
    end = millis() - start;
    // Serial.print(end);
    // Serial.println();

    if (i==0){
      continue;
    }
    else{
      avg += end;      
    }
  }
  avg /= i;
  Serial.println(avg);
  
} 

void loop()
{
  
}
