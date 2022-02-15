//
//  jenkins.h
//  BloomFilter
//
//  Created by Yosbi Alves Saenz on 04/04/2021.
//  Code taken of the implementation on wikipedia:
//  https://en.wikipedia.org/wiki/Jenkins_hash_function

#ifndef JENKINSHASH_H
#define JENKINSHASH_H
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
uint32_t jenkins_one_at_a_time_hash(const void * key, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* JENKINSHASH_H */
