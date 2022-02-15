//
//  main.cpp
//  BloomFilter
//
//  Created by Yosbi Alves Saenz on 03/04/2021.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "BloomFilter.h"

using namespace std;

BloomFilter* g_pBloomFilter;    // Global pointer to the bloomfilter
char         g_buff[255];       // Buffer to be used as input data

void addData()
{
    cout << endl << "Enter data: ";
    cin  >> g_buff;
    
    g_pBloomFilter->Add(g_buff, (int)strlen(g_buff));
}

void checkData()
{
    cout << endl << "Enter data: ";
    cin  >> g_buff;
    
    if (g_pBloomFilter->Check(g_buff, (int)strlen(g_buff)))
    {
        cout << "The data maybe is IN the filter with false positive probability of ";
        cout << std::setprecision( 2 ) << fixed;
        cout << g_pBloomFilter->GetFalsePositiveProbability() << "%" << endl;
        return;
    }
    
    cout << "The data is NOT in the filter" << endl;
}

void benchmark()
{
    char cAnswer;
    cout << endl << "This benchmark will need at least 3,59GB of RAM memory because it will create a temporary bloom filter with 1.000.000.000 expected items and a false positive probability of 0.02." << endl;
    cout << "do you want to continue?(y/n) ";
    cin  >> cAnswer;
    
    if (cAnswer == 'y')
    {
        cout << "Working... please wait!!..." << endl;
        
        clock_t start = clock() ;
        BloomFilter bloomfilter(1000000000, 0.02f);
        
        for (uint32_t i = 0; i < 1000000000; i++)
        {
            bloomfilter.Add(&i, sizeof(uint32_t));
        }
        
        clock_t end = clock() ;

        cout << "Benchmark done!" << endl;
        cout << "Your computer took " << (end-start)/(double)CLOCKS_PER_SEC  << " seconds to do 1.000.000.000 insertions in the bloom filter" << endl;
    }
}

int main(int argc, const char * argv[])
{
    bool bExit       = false;
    int  nSelecction = 0;
    
    std::cout   << UINT32_MAX << endl;
    std::cout   << "Welcome to the bloom filter!" << endl;
    std::cout   << "Do you know number of items to insert in the bloom filter?" << endl;
    std::cout   << "1 - yes" << endl;
    std::cout   << "2 - no" << endl;
    std::cout   << "option: ";
    std::cin    >> nSelecction;
    
    if (nSelecction == 1)
    {
        uint32_t nNumItems                  = 0;
        float    fFalsePositivesProbability = 0;
        
        std::cout   << "Enter the number of items: ";
        std::cin    >> nNumItems;
        std::cout   << "Enter the max acceptable false positive probability value (Ex: 0.02): ";
        std::cin    >> fFalsePositivesProbability;
        g_pBloomFilter = new BloomFilter(nNumItems, fFalsePositivesProbability);
    }
    else
    {
        uint32_t nBloomfilterSize   = 0;
        uint32_t nHashes            = 0;
        
        std::cout   << "Enter the size of the bloom filter: ";
        std::cin    >> nBloomfilterSize;
        std::cout   << "Enter the number of hashes for the bloom filter: ";
        std::cin    >> nHashes;
        g_pBloomFilter = new BloomFilter(nBloomfilterSize, nHashes);
    }
    
    
    while (!bExit)
    {
        std::cout << endl << "MENU" << endl;
        std::cout << "1 - Add data to filter" << endl;
        std::cout << "2 - Check data on filter" << endl;
        std::cout << "3 - Print filter" << endl;
        std::cout << "4 - Print filter stats" << endl;
        std::cout << "5 - Benchmark" << endl;
        std::cout << "6 - Exit" << endl;
        std::cout << "option: ";
        std::cin  >> nSelecction;
        
        switch (nSelecction)
        {
            case 1:
                addData();
                break;
            case 2:
                checkData();
                break;
            case 3:
                g_pBloomFilter->PrintFilter();
                break;
            case 4:
                g_pBloomFilter->PrintStats();
                break;
            case 5:
                benchmark();
                break;
            case 6:
                bExit = true;
                break;
            default:
                std::cout << "Option not recognized" << endl << endl;
        }
    }
    
    delete g_pBloomFilter;
    return 0;
}
