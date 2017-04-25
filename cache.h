//cache declarations

#ifndef CACHE_H
#define CACHE_H

/* for when you want to print debug messages */
#define DEBUG 1
//#define WRITE_POLICY 1
/* Cache Sizes in words*/
#define I_CACHE_SIZE 16
#define I_BLOCK_SIZE 1

#define D_CACHE_SIZE 128
#define D_BLOCK_SIZE 1

#define PENALTY 8
#define WRITE_POLICY 0
#define BYTE_OFFSET 2
//#define BLOCK_OFFSET 2
//#define BLOCK_MASK 0xffffffff
//#define BLOCK_INDEX 5

/* Block Size word alligned*/
//#define I_TAG
#define I_INDEX 5  //block index
#define I_OFFSET 0 //block offset
#define I_BLOCK_MASK 0x0000001f

//#define D_TAG
#define D_INDEX 7 //block index
#define D_OFFSET 0 //block offset
#define D_BLOCK_MASK 0x0000007f

/* Typedefs */
typedef struct Cache_* Cache;
typedef struct Block_* Block;

/*CreateCache
 *
 *Creates an cache.
 *Will be used to create I and D cache_size
 *
*/

Cache CreateCache(int cache_size, int block_size, int write_policy);

/*DestroyCache
 *not sure if we will need this but might come in handy
*/

void DeleteCache(Cache cache);

/*iCacheRead
 *read stuff from iCache for write back
*/

int iCacheRead(Cache cache, unsigned int address);

/*d_CacheRead
 *read stuff from d Cache for write back
*/

int d_CacheRead(Cache cache, unsigned int address);


/*iWriteCache
 *put stuff in the i cache
*/

int iWriteCache(Cache cache, unsigned int address, unsigned int data);

/*d_WriteCache
 *put stuff in the d cache
*/

int d_WriteCache(Cache cache, unsigned int address, unsigned int data);


/*PrintCache
 *see what is in the cache for debugging
*/

int PrintCache(Cache cache);

#endif
