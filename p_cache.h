/*
 * p_cache.h
 *
 *  Created on: Nov 18, 2017
 *      Author: deep
 */

#ifndef P_CACHE_H_
#define P_CACHE_H_

#pragma once

//#include <stdint.h>


/*Default cache parameters- gets changed once the new parameters are input
#define DATA_CACHE_SIZE (1024 * 8)
#define ASSOCIATIVITY 1
#define BLOCK_SIZE
#define WRITE_POLICY
#define VICTIM_CACHE_SIZE*/

typedef struct block{
	int val_bit;
	int blk_index;
	int tag;
};

typedef struct Cache_line_t{
	//int tag;
	int index;
	int dirty_bit;
	block *blk;
};

typedef struct cache{
	Cache_line_t *sets;
};

//

/*************************************************/
/*Global Variable intitializations*/
/*************************************************/

static int L1_reads;
static int L1_read_misses;
static int VC_reads;
static int VC_read_misses;
static int L1_writes;
static int L1_write_misses;
static int VC_writes;
static int VC_write_misses;
static int main_Memory_To_Traffic;
static int main_Memory_From_Traffic;



/************************************************/

/***Function initializations ********************/
/*************************************************/
void cache_init(int no_sets, int associativity, int bloc_size, int no_of_blks, int size_vc, int wp);

int cache_invoke(cache*, int, int, int, int, char, unsigned long long int  *, int, int wp);

int victimcache_search(unsigned long long int*, int tag_c, int);

void victimcache_replace(unsigned long long int*, int,int);

/************************************************/


#endif /* P_CACHE_H_ */
