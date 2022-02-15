//
//  jenkins.c
//  BloomFilter
//
//  Created by Yosbi Alves Saenz on 04/04/2021.
//  Code taken of the implementation on wikipedia:
//  https://en.wikipedia.org/wiki/Jenkins_hash_function

#include "jenkins.h"

uint32_t jenkins_one_at_a_time_hash(const void * key, uint32_t length) {
  const uint8_t * data = (const uint8_t*)key;
    
  uint32_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += data[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}
