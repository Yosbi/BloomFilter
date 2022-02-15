all:
	g++ -o bloomfilter main.cpp BloomFilter.cpp jenkins.c murmur3.c -O3
