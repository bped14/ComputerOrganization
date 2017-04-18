//cache declarations

#ifndef CACHE_H
#define CACHE_H

/* for when you want to print debug messages */
#define DEBUG 1
//#define WRITE_POLICY 1
/* Cache Sizes in words*/
#define I_CACHE_SIZE 32
#define I_BLOCK_SIZE 8

#define D_CACHE_SIZE 64
#define D_BLOCK_SIZE 8

#define PENALTY 8

/* Block Size word alligned*/
#define I_TAG 27  //byte 25
#define I_INDEX 2  //byte 2
#define I_OFFSET 3 //byte 5

#define D_TAG 25  //byte 21
#define D_INDEX 3 //byte 5
#define D_OFFSET 3 //byte 5

/* Typedefs */
typedef struct Cache_* Cache;
typedef struct Block_* Block;

/*CreateCache
 *
 *Creates a cache.
 *Will be used to create I and D cache_size
 *
*/

Cache CreateCache(int cache_size, int block_size);

/*DestroyCache
 *not sure if we will need this but might come in handy
*/

void DeleteCache(Cache cache);

/*ReadCache
 *read stuff from cache for write back
*/

int ReadCache(Cache cache, int address);

/*WriteCache
 *put stuff in the cache
*/

int WriteCache(Cache cache, int address);

/*PrintCache
 *see what is in the cache for debugging
*/

void PrintCache(Cache cache);

#endif
