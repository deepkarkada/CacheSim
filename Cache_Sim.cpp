#include "p_cache.h"
#include "source.h"

int main(int argc, char **argv) {
	/***Local variable initializations***/

		int csize, assoc, bsize, vcsize;                      //----> declaring the cache size, associativity, block size and victim cache size
		int write_pol;                                //----> declaring a buffer to hold the user-input write policy
		int set_n;                                         //----> number of sets
		int blk_n;

		//printf("\nPlease input the L1 cache size,associativity,block size in bytes,\n write policy(0 for write back with write allocate and 1 for write through with no write allocate) and victim cache size in blocks\n in the respective order");

		csize = 64;
		assoc = 1;
		bsize = 4;
		write_pol = 0;
		vcsize = 16;
		//scanf("%d%d%d%d%d", &csize, &assoc, &bsize, &write_pol, &vcsize);
		printf("%d%d%d%d%d", csize, assoc, bsize, write_pol, vcsize);

		set_n = csize / (assoc * bsize);                   //---->Number of sets = cache size /(Associativity * block size)

		blk_n = csize / bsize;

		printf("%d%d%d%d%d", csize, assoc, bsize, write_pol, vcsize);
		cache_init(set_n, assoc, bsize, blk_n, vcsize, write_pol);             //---->Cache initialization function

		return 0;

	}

	int* address_calc(int addr, int set_n, int blk_n)
	{
		/*This function calculates the tag,index and offset for the given memory reference and returns the pointer to array where
		the values are stored*/
		int tag_f = 0;
		int index_off = 0;
		int blk_index = 0;
		int set_index = 0;
		int addr_arr[3];
		int *ptr;
		double no_sets;

		no_sets = (double)set_n;
		tag_f = addr / blk_n;
		index_off = tag_f % set_n;

		blk_index = (int)(log((double)blk_n) / log((double)2));
		set_index = index_off >> blk_index;

		addr_arr[0] = tag_f;
		addr_arr[1] = index_off;
		addr_arr[2] = set_index;

		ptr = addr_arr;

		return ptr;
	}
