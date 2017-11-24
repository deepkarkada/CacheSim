# CacheSim
##Parametric cache simulator (Trace driven):

A parametric cache simulator takes in input parameters such as data cache size, associativity, block size, and write policy, victim cache size, an input trace file and simulates a data cache suited for FFT .
The simulator is trace driven, where an access to the cache blocks is represented by fixed-length fields representing the address and the load/store tags.

The access to the cache is done through tag and index fields. The tags contain the information needed to identify whether a word in the cache corresponds to the requested word. 

The index was calculated using the formula :
`Set index = (address / number of blocks) % number of sets`

###Implementation:
The simulator simulates the following memory hierarchy:
i) L1 cache ii) Victim cache (depending on the size of the victim cache size specified, no victim cache if the size specified is zero).

L1 cache specifications:
The L1 cache is specified by the L1 cache size, associativity and block size. The L1 cache structure was implemented using structures.
The number of sets and block size is calculated as follows:

Number of sets = cache size / (associativity * block size)

Number of blocks = cache size / block size in bytes

###Victim cache implementation:
A Victim cache implementation reduces the average miss rate