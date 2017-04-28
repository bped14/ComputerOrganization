//cache declarations

#ifndef CACHE_H
#define CACHE_H

/* for when you want to print debug messages */
#define DEBUG 0
#define WRITE_POLICY 0 //0 == WT and 1 == WB
#define BUFFER_SIZE 1 //set equal to block size

#define BLOCK_WORDS 1 //how many words you want in block
#define OFFSET_MASK 0x00000000 //block offset mask, 16 words = f, 4 words = 3, 1 word = 0

/* Cache Sizes in words*/
#define I_CACHE_SIZE 16
#define D_CACHE_SIZE 256

/* Block Size word alligned*/
#define I_INDEX 4  //block index, 16 words = 0, 4 words = 2, 1 word = 4
#define I_OFFSET 0 //block offset, 16 words = 4, 4 words = 2, 1 word = 0
#define I_BLOCK_MASK 0x0000000f //16 words = 0, 4 words = 3, 1 word = f

/*Block size word alligned*/
#define D_INDEX 7  //block index, 16 words = 1, 4 words = 4, 1 word = 16
#define D_OFFSET 0 //block offset, 16 words = 4, 4 words = 2, 1 word = 0
#define D_BLOCK_MASK 0x0000007f //16 words = 0, 4 words = 3, 1 word = f

#define I_PENALTY 8 //1 word block = 8, 4 word block = 14, 16 word block = 38
#define D_PENALTY 6 //1 word block = 6, 4 word block = 12, 16 word block = 36
#define PENALTY 2

/* Typedefs */
typedef struct Cache_* Cache;
typedef struct Block_* Block;
typedef struct Boffset_* Boffset;
/*CreateCache
 *Creates an cache with custom size
 *Will be used to create I and D cache_size
*/

Cache CreateCache(int cache_size);

/*DestroyCache
 *not sure if we will need this but might come in handy
*/

void DeleteCache(Cache cache);

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

//iWriteCache function, don't need, you don't write to cache
// int iWriteCache(Cache cache, unsigned int address, unsigned int data)
// {
//   unsigned int memory = 0x00000000;
//   /* Validate inputs */
//   if(cache == NULL)
//   {
//     printf("Put in a real cache.\n");
//     return 0;
//   }
//
//   int block_address = ((address >> BYTE_OFFSET >> I_OFFSET) & I_BLOCK_MASK); //get block address
//   unsigned int tag = (address >> BYTE_OFFSET >> I_OFFSET >> I_INDEX); //get tag
//   int blockoffset = ((address >> BYTE_OFFSET) & OFFSET_MASK);
//
//   if(DEBUG)
//   {
//     printf("Writing data from block: %i\n", block_address);
//     printf("Tag being used: %i\n",tag);
//   }
//
//   if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 1)
//   {
//     memory = cache->blocks[block_address]->boffset[blockoffset]->data; //put old data into memory
//     //clock_cycles = clock_cycles + PENALTY;
//   }
//
//   if(cache->write_policy == 1)
//   {
//     cache->blocks[block_address]->dirty = 1;
//   }
//
//   cache->blocks[block_address]->boffset[blockoffset]->data = data; //putting data into cache
//   cache->blocks[block_address]->tag = tag; //changing tag
//
//   if(cache->write_policy == 0)
//   {
//     write_buffer[0] = data; //can go back to pipeline while data gets stored in memory (8 clock cycles)
//     memory = cache->blocks[block_address]->boffset[blockoffset]->data;
//     //also store new data in main memory for WT
//     //clock_cycles = clock_cycles + PENALTY;
//   }
//
//   cache->writes++;
//   cache->blocks[block_address]->valid = 1;
//
//   printf("MEMORY: %i\n",memory);
//   return 0;
// }
