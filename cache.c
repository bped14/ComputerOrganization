//Direct Mapped Cache

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

//Structs (Cache and Block)
struct Block_ {
  //valid and dirty are not part of the block address
  int valid;
  int tag;
  int dirty;
}

struct Cache_ {
  int hits;
  int misses;
  int reads;
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
  }

  return cache;
}

/*how professor implemented

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

//do these before you enter i-cache_read
block address = [(address >> byte offset >> block offset) & block mask)]
cache address = block address << block offset + block offset value)
