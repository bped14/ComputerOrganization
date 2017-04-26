//cache declarations

#ifndef CACHE_H
#define CACHE_H

/* for when you want to print debug messages */
#define DEBUG 0
//#define WRITE_POLICY 1
/* Cache Sizes in words*/
#define I_CACHE_SIZE 16
#define I_BLOCK_SIZE 4

#define D_CACHE_SIZE 16
#define D_BLOCK_SIZE 4

#define PENALTY 8
#define WRITE_POLICY 0
#define BYTE_OFFSET 2

/* Block Size word alligned*/
#define I_INDEX 2  //block index
#define I_OFFSET 2//block offset
#define I_BLOCK_MASK 0x00000003

//#define D_TAG
#define D_INDEX 2 //block index
#define D_OFFSET 2 //block offset
#define D_BLOCK_MASK 0x00000003

/* Typedefs */
typedef struct Cache_* Cache;
typedef struct Block_* Block;

/*CreateCache
 *Creates an cache with custom size
 *Will be used to create I and D cache_size
*/

Cache CreateCache(int cache_size, int block_size, int write_policy);

/*DestroyCache
 *not sure if we will need this but might come in handy
*/

void DeleteCache(Cache cache);

/*iCacheRead
 *read stuff from iCache
 *take data from cache or go grab from memory if not in cache and place in cache after
 *use iCacheRead for when you need an instruction, see if iCache has it first
*/

int iCacheRead(Cache cache, unsigned int address, unsigned int data);

/*d_CacheRead
 *read stuff from d_Cache
 *take data from cache or go grab from memory if not in cache and place in cache after
 *use d_CacheRead for things like load word or when need data from memory
*/

int d_CacheRead(Cache cache, unsigned int address, unsigned int data);

/*iWriteCache
 *put stuff in the i cache
 *use iWriteCache when you want to put instructions in iCache
*/

int iWriteCache(Cache cache, unsigned int address, unsigned int data);

/*d_WriteCache
 *put stuff in the d cache
 *use d_WriteCache when you want to right things to memory like store word
*/

int d_WriteCache(Cache cache, unsigned int address, unsigned int data);


/*PrintCache
 *see what is in the cache for debugging
*/

int PrintCache(Cache cache);

#endif
