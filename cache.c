//Direct Mapped Cache

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

//Structs (Cache and Block)
struct Block_ {
  //valid and dirty are not part of the block address
  int valid; //allows you to know if block is filled
  int tag;
  int dirty; //whether data is different than that stored in main memory. WB/WT stuff
  unsigned int data;
}

struct Cache_ {
  int hits;
  int misses;
  int reads; //memory reads
  int writes;
  int cache_size;
  int block_size;
  int lines;
  int write_policy;
  Block* blocks;
};

//create a cache (I cache and D cache)
Cache CreateCache(int cache_size, int block_size, int write_policy)
{
  //local variables
  Cache cache;
  int i;

//making sure that user input is correct
  if(cache_size <= 0)
  {
    printf("Cache size needs to be greater than zero, nice try");
    return NULL;
  }

  if(block_size <= 0)
  {
    printf("Block size needs to be greater than zero, nice try");
    return NULL;
  }

  if(write_policy != 0 && write_policy != 1)
  {
    printf("Write policy must be write back or write through");
    return NULL;
  }

  //making the Cache
  cache = (Cache) malloc( sizeof( struct Cache_ ));
  if(cache == NULL) //check to see if cache has size
  {
    printf("Could not create cache");
    return NULL;
  }

  cache->hits = 0;
  cache->misses = 0;
  cache->reads = 0;
  cache->writes = 0;
  cache->write_policy = write_policy;
  cache->cache_size = CACHE_SIZE;
  cache->block_size = BLOCK_SIZE;
  cache->lines = (int)(CACHE_SIZE/BLOCK_SIZE);
  cache->blocks = (BLock*) malloc(sizeof(Block) * cache->lines);
  assert(cache->blocks != NULL);

  //initialize cache
  //fill valid and dirty bits with zero

  for(i=0; i<cache->lines; i++)
  {
    cache->blocks[i] = (Block) malloc(sizeof(struct Block_));
    assert(cache->blocks[i] != NULL);
    cache->blocks[i]->valid = 0;
    cache->blocks[i]->dirty = 0;
    cache->blocks[i]->tag = NULL;
    cache->blocks[i]->data = 0x00000000;
  }

  return cache;
}

/*Prints Cache results*/
void PrintCache(Cache cache)
{
  int i;
  char* tag;

  if(cache != NULL)
  {
    for(i=0; i<cache->numLines;i++);
    {
      tag = "NULL";
      if(cache->block[i]->tag != NULL)
      {
        tag = cache->blocks[i]->tag;
      }
      printf("[%i]: {valid: %i, tag: %s}",i,cache->blocks[i]->valid,tag);
    }
    printf("Cache\n\tCACHE HITS: %i\n\tCACHE MISSES: %i\n\tMEMORY READS: %i\n\tMEMORY WRITES: %i\n\n\tCACHE SIZE: %i Bytes\n\tBLOCK SIZE: %i Bytes\n\tNUM LINES: %i\n", cache->hits, cache->misses, cache->reads, cache->writes, cache->cache_size, cache->block_size, cache->numLines);
    }
}

/*Read cache and take dat from cache or go grab from memory if not in cache */
void CacheRead(Cache cache, unsigned int address, unsigned int data);
{
  /* Check inputs */
  if(cache == NULL)
  {
    fprintf("Put in a real cache.\n");
    return 0;
  }

  if(address == NULL)
  {
    fprintf("Put in a real address.\n");
    return 0;
  }

  Block block;
  //find block address to look for
  int block_address = ((address >> BYTE_OFFSET >> BLOCK_OFFSET) & BLOCK_MASK);
  block = cache->blocks[block_address];
  //obtain tag from address
  int tag = (address >> BYTE_OFFSET >> BLOCK_OFFSET >> BLOCK_ADDRESS);

  if(DEBUG)
  {
    printf("Reading data from block: %i\n Tag being used: %i", block_address,tag);
  }

  if(block->valid == 1 && block->tag == tag)
  {
    cache->hits++;
    data = cache->blocks[block_address]->data;
    //might need to free tag, not sure yet
  }
  else
  {
    cache->misses++;
    cache->reads++;

    //need to go to main memory to retrive data
    data = cache->blocks[block_address]->data;

    if(cache->write_policy == 1 && block->dirty ==1)
    {
      cache->writes++;
      block->dirty = 0;
    }
    //clock_cycles = clock_cycles + PENALTY
    block->valid = 1;
    block->tag = tag;
  }
  //find
}

void WriteCache(Cache cache, unsigned int address, unsigned int data)
{
  /* Validate inputs */
  if(cache == NULL)
  {
    fprintf("Put in a real cache.\n");
    return 0;
  }

  if(address == NULL)
  {
    fprintf("Put in a real address.\n");
    return 0;
  }
  Block block;
  //find block address to look for
  int block_address = ((address >> BYTE_OFFSET >> BLOCK_OFFSET) & BLOCK_MASK);
  block = cache->blocks[block_address];
  //obtain tag from address
  int tag = (address >> BYTE_OFFSET >> BLOCK_OFFSET >> BLOCK_ADDRESS);

}

int main()
{
  iCache = CreateCache(CACHE_SIZE, BLOCK_SIZE, WRITE_POLICY);
  d_Cache = CreateCache(CACHE_SIZE, BLOCK_SIZE, WRITE_POLICY);

  WriteCache(iCache, 0x00003759, 0x85937593);
  printCache(iCache);

  WriteCache(d_Cache, 0x00003759, 0x85937593);
  printCache(d_Cache);
}
/*how professor implemented

//do these before you enter i-cache_read
block address = [(address >> byte offset >> block offset) & block mask)]
cache address = block address << block offset + byte offset value)

i-cache_read(takes in address, &data)
{
if (address & tag mask(bit mask)) == tag[blockaddress]) && (valid[blockaddress])
{
then if true we have cache hits
&data = icache[cacheaddress];
cachehit = true;
}
//must have cache miss
else
{
cachehit = false;
valid[blockaddress] = false;
//call main memory to get data
datavalid = main_memory(address, &data) //include penalty of 8 cycles mainmemory count down from 8, when = 0 change to hit
if(datavalid == true)
{
cachedata[cache address] = data;
//when valid
update valid bit
}
return cachehit; if hit tell pipe data was valid, if false stall pipeline
}
}
