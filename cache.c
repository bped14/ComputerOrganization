//Direct Mapped Cache

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

//Structs (Cache and Block)
struct Block_ {
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
}
