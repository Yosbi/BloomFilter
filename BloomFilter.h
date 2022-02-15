//
//  BloomFilter.h
//  BloomFilter
//
//  Created by Yosbi Alves Saenz on 03/04/2021.
//

#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H
#pragma once

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include "jenkins.h"
#include "murmur3.h"

#define MIN_FALSE_POSITIV_PROB 0.00001

class BloomFilter
{
private:
    uint32_t  m_nFilterSize;                                                   // Number of bits of the bloom filter
    uint32_t  m_nNumHashes;                                                    // Number of hashes
    uint32_t  m_nNumObjects;                                                   // Number of objects inserted in the bloomfilter
    uint32_t* m_pFilter;                                                       // Pointer to the first position of the array of bits of the bloom filter
    
public:
    explicit BloomFilter(uint32_t nFilterSize, uint32_t nNumHashes);            // Constructor using the filter size and the number of hashes
    explicit BloomFilter(uint32_t nNumItems, float fFalsePositivesProbability); // Constructor using the number of expected items to insert and the expected false positive probability
    ~BloomFilter();                                                             // Destructor
    
    void  Add(const void * data, uint32_t len);                                 // Add data to the filter
    bool  Check(const void * data, uint32_t len);                               // Check if data is inside the filter
    
    void  PrintFilter();                                                        // Prints the bits of the filter
    void  PrintStats();                                                         // Prints the current stats of the filter
    float GetFalsePositiveProbability();                                        // Returns the actual false positive probability of a check
    int   GetNumberHashesRepeated();                                            // Returns the number of collisions of hashes that have occurred
    
private:
    void  Init(uint32_t nFilterSize, uint32_t nNumHashes);                      // Init the bit array and the member variables
    void  SetBit(uint32_t bit);                                                 // Set a bit from 0 to 1 in the filter
    bool  TestBit(uint32_t bit);                                                // Test if a bit is 1 or 0 in the filter and returns it's value in a boolean
};

#endif /* BLOOMFILTER_H */
