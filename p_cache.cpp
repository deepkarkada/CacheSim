/*
 * p_Cache.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: deep
 */

#include <stdlib.h>
#include "p_cache.h"
#include "source.h"

void cache_init(int no_sets, int associativity, int bloc_size, int no_of_blks, int vc_size, int wp)
{
	/*This function is used for three tasks. First, the function initializes the cache structure according to blocks and sets.
	The function parses the trace file to execute one instruction at a time, simulating a processor.
	The cache invoke function is then called to simulate the cache operation*/
	int i = 0;
	char access;                                       //----> Data load or store instruction type
	int addr;                                          //---->address field extracted from the trace file
	char a, b;                                          //---->temporary variables used for string parsing
	int *addr_ptr;
	int L1_miss_rate;
	int VC_miss_rate;
	struct cache *new_cache;                            //---->declaring a new cache structure

	int flg;

	unsigned long long int *vc_cache;

	vc_cache = (unsigned long long int *)calloc(vc_size, sizeof(unsigned long long int));

	if (vc_cache == NULL)
		printf("\nSpace allocation for the Victim cache failed");

	/*for (int j = 0; j < no_of_blks; j++)
		vc_cache[j] = 0;*/



	printf("\nInside cache init:%d", no_sets);

	new_cache = (cache *)calloc(sizeof(Cache_line_t),1);  //---->dynamically allocating space for the new cache structure
	if (new_cache == NULL)
		printf("\nSpace allocation for the cache failed");

	//for (i = 0; i<no_sets; i++)
	//{
	new_cache->sets = (Cache_line_t *)calloc(no_sets,sizeof(Cache_line_t));  //---->dynamically allocating space for the sets
		if (new_cache->sets == NULL)
			printf("\nSpace allocation for the set failed");
		//else
		//printf("\nSpace allocated for the set:%d",sizeof(Cache_line_t));
	//}
		for (i = 0; i < no_sets; i++)
		{
			for (int j = 0; j<associativity; j++)
				new_cache->sets[i].blk = (block *)malloc(sizeof(block)*1);        //---->dynamically allocating space for the blocks
		}



	for (i = 0; i< no_sets; i++)
	{
		new_cache->sets[i].dirty_bit = 0;

		new_cache->sets[i].index = i;

		/*if (associativity>1)
		{
			for (int j = 0; j<associativity; j++)
			new_cache->sets[i].blk[j].tag = 0;
		}*/


		for (int j = 0; j< associativity; j++)
		{
			new_cache->sets[i].blk[j].val_bit = 0;
			new_cache->sets[i].blk[j].blk_index = j;
			new_cache->sets[i].blk[j].tag = 0;
		}

	}
	printf("\n*********** Initial L1 Cache structure **************");
	for (i = 0; i< no_sets; i++)
	{
		for (int j = 0; j < associativity; j++)
		{
			printf("\n%d\t%d\t%d", new_cache->sets[i].dirty_bit, new_cache->sets[i].index, new_cache->sets[i].blk[j].tag);
		}
	}

	printf("\n\n*********** Initial victim Cache structure **************");

	for (int j = 0; j < vc_size; j++)
		printf("\n%d", vc_cache[j]);

    //i = 0;

	fp = fopen("Trace.txt", "r");
	if (fp == NULL)
		printf("\nError:Unable to open the file\n");

	while (1)
	{

		if (fscanf(fp, " %c %x %c", &access, &addr, &b) != EOF)
		{

			//printf("\nFile opened");
			if ((access == 'L' || access == 'S') && addr>0)
			{
					if (access == 'L') L1_reads++;
					else if (access == 'S') L1_writes++;
				//printf("\n%c\t%d",access,addr);

				addr_ptr = address_calc(addr, no_sets, no_of_blks);
				int tag = *addr_ptr;
				int set_index = *(addr_ptr + 1);
				int blk_index = *(addr_ptr + 2);
				//printf("\n%d\t%d\t%d", tag, set_index, blk_index);
				//printf("\nBefore pass:%d\t%d",&new_cache,*new_cache);
				cache_invoke(new_cache, tag, set_index, no_sets, associativity, access, vc_cache, vc_size, wp);
			}
			else
				continue;

		}
		else
			break;

	}

	printf("\n\n*********** Final L1 Cache structure **************");
	for (i = 0; i< no_sets; i++)
	{
		for (int j = 0; j < associativity; j++)
		{
			printf("\n%d\t%d\t%d", new_cache->sets[i].dirty_bit, new_cache->sets[i].index, new_cache->sets[i].blk[j].tag);
		}
	}

	printf("\n\n*********** Final victim Cache structure **************");

	for (int j = 0; j < vc_size; j++)
		printf("\n%d", vc_cache[j]);

	/****************************Miss rate calculation***************************
	L1 miss rate = (L1 read misses + L1 write misses)/(L1 reads + L1 writes)
	Victim cache miss rate = (Victim cache read misses + Victim cache write misses)/(Victim cache reads + Victim cache writes)*/

	L1_miss_rate = (L1_read_misses + L1_write_misses) * 100 / (L1_reads + L1_writes);
    VC_miss_rate = (VC_read_misses + VC_write_misses) * 100 / (VC_reads + VC_writes);


	printf("\n\n************* Print statistics ********************");
	printf("\nNumber of L1 reads:%d", L1_reads);
	printf("\nNumber of L1 writes:%d", L1_writes);
	printf("\nNumber of L1 read misses:%d", L1_read_misses);
	printf("\nNumber of L1 write misses:%d", L1_write_misses);
	printf("\nNumber of Victim cache reads:%d", VC_reads);
	printf("\nNumber of Victim cache read misses:%d", VC_read_misses);
	printf("\nNumber of Victim cache writes:%d", VC_writes);
	printf("\nNumber of Victim cache write misses:%d", VC_write_misses);
    printf("\nL1 miss rate in percentage:%d", L1_miss_rate);
	printf("\nVictim cache miss rate in percentage:%d", VC_miss_rate);
	printf("\nTotal memory traffic:%d", (main_Memory_From_Traffic + main_Memory_To_Traffic));

}

int cache_invoke(cache *cache_ptr, int tag_f, int index_f, int no_sets, int assoc, char R_W, unsigned long long int  *vccache_ptr, int size_vc, int wp)
{
	/*This function simulates the cache operation. The cache is searched for the tag field using the unique tag and index fields
	Depending on the hits and misses, the hit and miss counts are incremented. If an element is not found in the L1 cache,
	function invokes the victim cache function*/
	int i;
	unsigned long long int temp;
    //printf("\n\nAccess type,Address tag and index:%c\t%d\t%d", R_W, tag_f, index_f);

	/*for (i = 0; i < no_sets; i++)
	{
	for (int j = 0; j < assoc; j++)
	{
	printf("\nCache index function:%d\t%d", cache_ptr->sets[i].index, cache_ptr->sets[i].blk[j].tag);
	}
	}*/

	for (i = 0; i < no_sets; i++)
	{

		if (cache_ptr->sets[i].index == index_f)
		{
			for (int j = 0; j <= assoc; j++)
			{
				if (cache_ptr->sets[i].blk[j].tag == tag_f)
				{
					if (R_W == 'L')
					{
						//printf("\nRead Hit");
						temp = cache_ptr->sets[i].blk[j].tag;
						for (int k = j; k > 0; k--)
						{
							cache_ptr->sets[i].blk[k].tag = cache_ptr->sets[i].blk[k - 1].tag;
						}
						cache_ptr->sets[i].blk[0].tag = tag_f;
						return 0;
					}
					else if (R_W == 'S')
					{
						//printf("\nWrite Hit");
						temp = cache_ptr->sets[i].blk[j].tag;
						for (int k = j; k > 0; k--)
						{
							cache_ptr->sets[i].blk[k].tag = cache_ptr->sets[i].blk[k - 1].tag;
						}
						cache_ptr->sets[i].blk[0].tag = tag_f;
						return 0;
					}

				}

			}

			if (R_W == 'L')
			{
				L1_read_misses++;
				//printf("\nRead Miss");
				//Implementing a victim cache
				VC_reads++;
				int a = victimcache_search(vccache_ptr, tag_f, size_vc);
				if (a)
				{
					//printf("\nFound in Victim Cache");
					temp = cache_ptr->sets[i].blk[assoc - 1].tag;
					for (int k = assoc - 1; k > 0; k--)
					{
						cache_ptr->sets[i].blk[k].tag = cache_ptr->sets[i].blk[k - 1].tag;
					}
					cache_ptr->sets[i].blk[0].tag = vccache_ptr[a];
					victimcache_replace(vccache_ptr, size_vc, a);
					VC_writes++;
					vccache_ptr[0] = temp;
					return 0;


				}
				else
				{
					VC_read_misses++;
					//VC_write_misses++;
					main_Memory_From_Traffic++;
				}

				if (cache_ptr->sets[i].blk[0].tag == 0)
					cache_ptr->sets[i].blk[0].tag = tag_f;
				else
				{
					temp = cache_ptr->sets[i].blk[assoc-1].tag;
					victimcache_replace(vccache_ptr, size_vc,0);
					VC_writes++;
					vccache_ptr[0] = temp;
					for (int k = assoc - 1; k > 0; k--)
					{
                       cache_ptr->sets[i].blk[k].tag = cache_ptr->sets[i].blk[k-1].tag;
					}
					cache_ptr->sets[i].blk[0].tag = tag_f;


				}
				return 0;

			}
			else if (R_W == 'S')
			{
				L1_write_misses++;
				//printf("\nWrite Miss");
				VC_reads++;
				//Implementing the victim cache
				if (wp == 0)
				{
					int a = victimcache_search(vccache_ptr, tag_f, size_vc);
					if (a)
					{
						//printf("\nFound in Victim Cache");
						temp = cache_ptr->sets[i].blk[assoc - 1].tag;
						for (int k = assoc - 1; k > 0; k--)
						{
							cache_ptr->sets[i].blk[k].tag = cache_ptr->sets[i].blk[k - 1].tag;
						}
						cache_ptr->sets[i].blk[0].tag = vccache_ptr[a];
						victimcache_replace(vccache_ptr, size_vc, a);
						VC_writes++;
						vccache_ptr[0] = temp;
						return 0;
					}

					else
					{
						VC_read_misses++;
						//VC_write_misses++;
						main_Memory_To_Traffic++;
					}

					if (cache_ptr->sets[i].blk[0].tag == 0)
						cache_ptr->sets[i].blk[0].tag = tag_f;
					else
					{
						temp = cache_ptr->sets[i].blk[assoc - 1].tag;
						victimcache_replace(vccache_ptr, size_vc, 0);
						VC_writes++;
						vccache_ptr[0] = temp;
						for (int k = assoc - 1; k > 0; k--)
						{
							cache_ptr->sets[i].blk[k].tag = cache_ptr->sets[i].blk[k - 1].tag;
						}
						cache_ptr->sets[i].blk[0].tag = tag_f;

                    }
				}

				else if (wp == 1 )
				{
					//VC_read_misses++;
					//VC_write_misses++;
					main_Memory_To_Traffic++;
				}

				return 0;
			}
		}
	}
}



int victimcache_search(unsigned long long int* vc_cache, int tag_c, int vc_size)
{
	/*This function is used to search for the presence of element specified by the tag in the cache.
	If it is present, the index of  element is returned otherwise 0 is returned*/
	int i;
	for (i = 0; i<vc_size; i++)
	{
		if (vc_cache[i] == tag_c)

			return i;

	}
	return 0;
}

void victimcache_replace(unsigned long long int* vc_cache, int vc_size, int replace_index)
{
	/*The victim cache replace function is used to implement LRU algorithm in the victim cache. If replace_index is specified,
	the function shifts the elements down till the replace index. Otherwise it shifts all the elements and the last element is
	moved into main memory*/
	unsigned long long int temp = 0;

	if (!replace_index)
	{
		if (vc_cache[0] != 0)
		{
			temp = vc_cache[vc_size - 1];
			for (int i = vc_size - 1; i > 0; i--)
			{
				if (vc_cache[i - 1] != 0)
					vc_cache[i] = vc_cache[i - 1];
			}

			if (temp != 0)
				main_Memory_To_Traffic++;
		}
	}
	else
	{
		for (int i = replace_index; i > 0; i--)
		{
			if (vc_cache[i - 1] != 0)
				vc_cache[i] = vc_cache[i - 1];
		}
	}



}
