//
//  BloomFilter.cpp
//  BloomFilter
//
//  Created by Yosbi Alves Saenz on 03/04/2021.
//

#include "BloomFilter.h"

//--------------------------------------------------------------------
// Name: Constructor
// Desc: Constructor using the filter size and the number of hashes
//--------------------------------------------------------------------
BloomFilter::BloomFilter(uint32_t nFilterSize, uint32_t nNumHashes)
{
    Init(nFilterSize, nNumHashes);
}

//--------------------------------------------------------------------
// Name: Constructor
// Desc: Constructor using the number of expected items to insert and
//       the expected false positive probability
//--------------------------------------------------------------------
BloomFilter::BloomFilter(uint32_t nNumItems, float fFalsePositivesProbability)
{
    // Validating the minimum of false positives to prevent a total '0' wich causes a big bloom filter
    if (fFalsePositivesProbability < MIN_FALSE_POSITIV_PROB)
        fFalsePositivesProbability = MIN_FALSE_POSITIV_PROB;
    
    uint32_t nFilterSize = -(nNumItems * log(fFalsePositivesProbability)) / pow(log(2), 2);
    uint32_t nNumHashes = (nFilterSize / nNumItems) * log(2);
    Init(nFilterSize, nNumHashes);
}

//--------------------------------------------------------------------
// Name: Destructor
// Desc: Clean allocated resources
//--------------------------------------------------------------------
BloomFilter::~BloomFilter()
{
    delete [] m_pFilter;
    m_pFilter = 0;
}

//--------------------------------------------------------------------
// Name: Init
// Desc: Init the bit array and the member variables
//--------------------------------------------------------------------
void BloomFilter::Init(uint32_t nFilterSize, uint32_t nNumHashes){
    m_nFilterSize = nFilterSize;
    m_nNumHashes  = nNumHashes;
    m_nNumObjects = 0;
    
    int nFilterArraySize = ceil((float)nFilterSize / (float)sizeof(uint32_t));
    m_pFilter = new uint32_t[nFilterArraySize];
    memset(m_pFilter, 0, nFilterSize);
}

//--------------------------------------------------------------------
// Name: SetBit
// Desc: Set a bit from 0 to 1 in the filter
//--------------------------------------------------------------------
void BloomFilter::SetBit(uint32_t bit)
{
    m_pFilter[bit / 32] |= 1 << (bit % 32);
}

//--------------------------------------------------------------------
// Name: TestBit
// Desc: Test if a bit is 1 or 0 in the filter and returns it's value
//       in a boolean
//--------------------------------------------------------------------
bool BloomFilter::TestBit(uint32_t bit)
{
    if ( (m_pFilter[bit / 32] & (1 << (bit % 32) )) != 0 )
        return true;
    return false;
}

//--------------------------------------------------------------------
// Name: Add
// Desc: Add data to the filter
//--------------------------------------------------------------------
void BloomFilter::Add(const void * data, uint32_t len)
{
    //uint32_t murmur = MurmurHash3_x86_32(data, len, 1);
    //std::cout << "murmur: " << murmur % m_nFilterSize<< std::endl;
    SetBit(MurmurHash3_x86_32(data, len, 1) % m_nFilterSize);
    
    
    if (m_nNumHashes >= 2){
        //uint32_t jenkins = jenkins_one_at_a_time_hash(data, len);
        //std::cout << "jenkins: " << jenkins % m_nFilterSize<< std::endl;
        SetBit(jenkins_one_at_a_time_hash(data, len) % m_nFilterSize);
        
        if (m_nNumHashes > 2)
        {
            for (int i = 2; i < m_nNumHashes; i++){
                //uint32_t murmur = MurmurHash3_x86_32(data, len, i);
                //std::cout << "murmur: " << murmur % m_nFilterSize<< std::endl;
                SetBit(MurmurHash3_x86_32(data, len, i) % m_nFilterSize);
            }
        }
    }
    m_nNumObjects++;
}

//--------------------------------------------------------------------
// Name: Check
// Desc: Check if data is inside the filter
//--------------------------------------------------------------------
bool BloomFilter::Check(const void * data, uint32_t len)
{
    if (!TestBit(MurmurHash3_x86_32(data, len, 1) % m_nFilterSize))
        return false;
    
    if (m_nNumHashes >= 2){
        if (!TestBit(jenkins_one_at_a_time_hash(data, len) % m_nFilterSize))
            return false;
        
        if (m_nNumHashes > 2)
        {
            for (int i = 2; i < m_nNumHashes; i++){
                if ( !TestBit(MurmurHash3_x86_32(data, len, i) % m_nFilterSize) )
                    return false;
            }
        }
    }
    return true;
}

//--------------------------------------------------------------------
// Name: PrintFilter
// Desc: Prints the bits of the filter
//--------------------------------------------------------------------
void BloomFilter::PrintFilter()
{
    std::cout << std::endl;
    for (uint32_t i = 0; i < m_nFilterSize; i++)
    {
        std::cout << TestBit(i);
    }
    std::cout << std::endl;
}
 
//--------------------------------------------------------------------
// Name: PrintStats
// Desc: Prints the current stats of the filter
//--------------------------------------------------------------------
void BloomFilter::PrintStats()
{
    std::cout << std::endl << "Stats:" << std::endl;
    std::cout << "Size of bloom filter:           " << m_nFilterSize << std::endl;
    std::cout << "Number of Hashes:               " << m_nNumHashes  << std::endl;
    std::cout << "Number of objects inserted:     " << m_nNumObjects << std::endl;
    std::cout << "Probability of false positives: ";
    std::cout << std::setprecision( 2 ) << std::fixed;
    std::cout << GetFalsePositiveProbability() << "%" <<std::endl;
    std::cout << "Number of hashes repeated:      " << GetNumberHashesRepeated() << std::endl;
}

//--------------------------------------------------------------------
// Name: GetFalsePositiveProbability
// Desc: Returns the actual false positive probability of a check
//--------------------------------------------------------------------
float BloomFilter::GetFalsePositiveProbability(){
    return pow(1.0f - exp( (-(float)m_nNumHashes * (float)m_nNumObjects) / (float)m_nFilterSize ), (float)m_nNumHashes) * 100.0f;
}

//--------------------------------------------------------------------
// Name: GetNumberHashesRepeated
// Desc: Returns the number of collisions of hashes that have occurred
//--------------------------------------------------------------------
int BloomFilter::GetNumberHashesRepeated(){
    int nNHExpected = m_nNumObjects * m_nNumHashes;
    int nNHActual = 0;
    for (uint32_t i = 0; i < m_nFilterSize; i++)
    {
        if (TestBit(i))
            nNHActual++;
    }
    return nNHExpected - nNHActual;
}
