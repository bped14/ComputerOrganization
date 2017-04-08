//cache declarations

#ifndef CACHE_H
#define CACHE_H

/* for when you want to print debug messages */
#define DEBUG 1

/* Cache Size in words*/
#define CACHE_SIZE 32
#define BLOCK_SIZE 8

/* Block Size */
#define TAG
#define INDEX
#define OFFSET

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

void DestroyCache(Cache cache);

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
