//cache declarations

#ifndef CACHE_H
#define CACHE_H

/* VARIABLES TO CHANGE */
#define PROGRAM 1 //choose program to calculate correct CPI
#define I_CACHE_SIZE 32 //size in words
#define D_CACHE_SIZE 64 //size in words
#define WRITE_POLICY 0 //0 == WT and 1 == WB
#define BLOCK_WORDS 16 //how many words you want in block
#define INSTRUCTION_COUNT3 0 //ENTER amouont of intructions for program 3 to calculate correct CPI




/* Typedefs */
typedef struct Cache_* Cache;
typedef struct Block_* Block;
typedef struct Boffset_* Boffset;

/*CreateCache
 *Creates a cache with custom size
 *Will be used to create I and D cache_size
*/

Cache CreateCache(int cache_size);

/*iCacheRead
 *read stuff from iCache
 *take data from cache or go grab from memory if not in cache and place in cache after
 *use iCacheRead for when you need an instruction, see if iCache has it first
*/

unsigned int iCacheRead(Cache cache, unsigned int address);

/*d_CacheRead
 *read stuff from d_Cache
 *take data from cache or go grab from memory if not in cache and place in cache after
 *use d_CacheRead for things like load word or when need data from memory
*/

unsigned int d_CacheRead(Cache cache, unsigned int address);

/*iWriteCache
 *put stuff in the i cache
 *use iWriteCache when you want to put instructions in iCache
*/

int d_WriteCache(Cache cache, unsigned int address, unsigned int data);

/*PrintCache
 *see what is in the cache for debugging
*/

int PrintCache(Cache cache);

/* Calculate block index offset*/
int blockindexoffset(int block_size, int cache_size);

/* Calculate block index mask */
unsigned int blockindexmask(int block_size, int cache_size);

/* Calculate block offset */
int blockoffset(int block_size);

/* Calculate block offset mask */
unsigned int blockoffsetmask(int block_size);

#define DEBUG 0

#endif
