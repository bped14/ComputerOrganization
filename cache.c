//Direct Mapped Cache

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"

//Structs (Cache and Block)
struct Block_ {
  //valid and dirty are not part of the block address
  int valid; //allows you to know if block is filled
  unsigned int tag;
  int dirty; //whether data is different than that stored in main memory. WB/WT stuff
  unsigned int data;
};

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
  Cache cache;
  int i = 0;
//making sure that user input is correct
  if(cache_size <= 0)
  {
    printf("Cache size needs to be greater than zero, nice try\n");
    return NULL;
  }

  if(block_size <= 0)
  {
    printf("Block size needs to be greater than zero, nice try\n");
    return NULL;
  }

  if(write_policy != 0 && write_policy != 1)
  {
    printf("Write policy must be write back or write through\n");
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
  cache->cache_size = cache_size;
  cache->block_size = block_size;
  cache->lines = (int)(cache_size/block_size);
  cache->blocks = (Block*) malloc(sizeof(Block) * cache->lines);
  assert(cache->blocks != 0);

  //initialize blocks
  //fill valid and dirty bits with zero

  for(i=0; i<cache->lines; i++)
  {
    cache->blocks[i] = (Block) malloc(sizeof(struct Block_));
    assert(cache->blocks[i] != NULL);
    cache->blocks[i]->valid = 0;
    cache->blocks[i]->dirty = 0;
    cache->blocks[i]->tag = 0;
    cache->blocks[i]->data = 0x00000000;
  }
  return cache;
}

/*Read cache and take dat from cache or go grab from memory if not in cache */
int iCacheRead(Cache cache, unsigned int address, unsigned int data)
{
  unsigned int memory = 0x00000000;
  /* Check inputs */
  if(cache == NULL)
  {
    printf("Put in a real cache.\n");
    return 0;
  }

  unsigned int block_address = ((address >> BYTE_OFFSET >> I_OFFSET) & I_BLOCK_MASK); //get block address
  unsigned int tag = (address >> BYTE_OFFSET >> I_OFFSET >> I_INDEX); //get tag

  if(DEBUG)
  {
    printf("Reading data from block: %i\nTag being used: %i\n", block_address,tag);
  }

  if(cache->blocks[block_address]->valid == 1 && cache->blocks[block_address]->tag == tag) //cache hit
  {
    //printf("Valid bit equals 1 and tags match\n");
    cache->hits++;
    memory = cache->blocks[block_address]->data; //take data from cache and put it in memory
  }
  else //cache miss
  {
    cache->misses++;
    cache->reads++; //memory read

    if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 1) //check to see if data in cache is dirty
    {
      memory = cache->blocks[block_address]->data; //put old data from cache into main memory
      cache->blocks[block_address]->dirty = 0; //mark data as not dirty dirty
      cache->blocks[block_address]->data = data; //put new data into cache
      cache->writes++;
      //clock_cycles = clock_cycles + PENALTY;
    }
    else if(cache->write_policy == 0)
    {
      memory = cache->blocks[block_address]->data; //write data to main memory
    }
    //clock_cycles = clock_cycles + PENALTY
    cache->blocks[block_address]->valid = 1;
    cache->blocks[block_address]->tag = tag;
  }

  if(cache->write_policy == 1)
  {
    cache->blocks[block_address]->dirty = 1;
  }

  printf("Memory: %i\n",memory);
  return 0;
}

int d_CacheRead(Cache cache, unsigned int address, unsigned int data)
{
  unsigned int memory = 0x00000000;
  /* Check inputs */
  if(cache == NULL)
  {
    printf("Put in a real cache.\n");
    return 0;
  }

  int block_address = ((address >> BYTE_OFFSET >> D_OFFSET) & D_BLOCK_MASK); //get block address
  unsigned int tag = (address >> BYTE_OFFSET >> D_OFFSET >> D_INDEX); //get tag

  if(DEBUG)
  {
    printf("Reading data from block: %i\nTag being used: %i\n", block_address,tag);
  }

  if(cache->blocks[block_address]->valid == 1 && cache->blocks[block_address]->tag == tag)
  {
    //printf("Valid bit equal 1 and tags match\n");
    cache->hits++;
    data = cache->blocks[block_address]->data; //take data from cache and store in variable to use;
    //might need to free tag, not sure yet
  }
  else //cache miss
  {
    cache->misses++;
    cache->reads++; //memory read

    if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 1) //check to see if data in cache is dirty
    {
      memory = cache->blocks[block_address]->data; //put old data from cache into main memory
      cache->blocks[block_address]->dirty = 0; //mark data as not dirty dirty
      cache->blocks[block_address]->data = data; //put new data into cache
      cache->writes++;
      //clock_cycles = clock_cycles + PENALTY;
    }
    else if(cache->write_policy == 0)
    {
      memory = cache->blocks[block_address]->data; //write data to main memory
    }
    //clock_cycles = clock_cycles + PENALTY
    cache->blocks[block_address]->valid = 1;
    cache->blocks[block_address]->tag = tag;
  }
  printf("Memory: %i\n", memory);
  return 0;
}


int iWriteCache(Cache cache, unsigned int address, unsigned int data)
{
  /* Validate inputs */
  if(cache == NULL)
  {
    printf("Put in a real cache.\n");
    return 0;
  }
  //Block block;
  int block_address = ((address >> BYTE_OFFSET >> I_OFFSET) & I_BLOCK_MASK); //get block address
  //block = cache->blocks[block_address];
  int tag = (address >> BYTE_OFFSET >> I_OFFSET >> I_INDEX); //get tag
  //cache->blocks[block_address]->tag = tag;

  if(DEBUG)
  {
    printf("Writing data from block: %i\n", block_address);
    printf("Tag being used: %i\n",tag);
  }


  cache->blocks[block_address]->data = data; //putting data into cache
  cache->blocks[block_address]->tag = tag; //changing tag
  //might need to free tag, not sure yet

  cache->writes++;
  cache->blocks[block_address]->valid = 1;
  //clock_cycles = clock_cycles + PENALTY
  return 0;
}

int d_WriteCache(Cache cache, unsigned int address, unsigned int data)
{
  /* Validate inputs */
  if(cache == NULL)
  {
    printf("Put in a real cache.\n");
    return 0;
  }
  //Block block;
  int block_address = ((address >> BYTE_OFFSET >> D_OFFSET) & D_BLOCK_MASK); //get block address
  //block = cache->blocks[block_address];
  unsigned int tag = (address >> BYTE_OFFSET >> D_OFFSET >> D_INDEX); //get tag
  //block->tag = tag;

  if(DEBUG)
  {
    printf("Writing data from block: %i\n",block_address);
    printf("Tag being used: %i\n",tag);
  }

  cache->blocks[block_address]->data = data; //put data into cache
  cache->blocks[block_address]->tag = tag; //change tag
  //might need to free tag, not sure yet

  cache->writes++;
  //clock_cycles = clock_cycles + PENALTY
  cache->blocks[block_address]->valid = 1;

  return 0;
}

/*Prints Cache results*/
int PrintCache(Cache cache)
{
  int i;
  int valid;
  int lines;
  unsigned int tag;
  unsigned int data;

  lines = cache->lines;
  if(cache != NULL)
  {
  for(i=0; i < lines; i++)
  {
      tag = cache->blocks[i]->tag;
      valid = cache->blocks[i]->valid;
      data = cache->blocks[i]->data;
      printf("[%i]: { valid: %i, tag: %i } DATA: %i\n", i, valid, tag, data);
  }
    printf("\n\tCACHE HITS: %i\n\tCACHE MISSES: %i\n\tMEMORY READS: %i\n\tMEMORY WRITES: %i\n\n\tCACHE SIZE: %i Words\n\tBLOCK SIZE: %i Words\n\tNUM LINES: %i\n", cache->hits, cache->misses, cache->reads, cache->writes, cache->cache_size, cache->block_size, cache->lines);
  }
    return 0;
}

int main()
{
  Cache iCache;
  //Cache d_Cache;

  unsigned int data1 = 0x77654321;
  unsigned int data2 = 0x73656383;
  unsigned int address1 = 0x8764444;
  unsigned int address2 = 0x00054321;
  unsigned int address3 = 0x58354321;
  unsigned int address4 = 0x52554321;

  iCache = CreateCache(I_CACHE_SIZE, I_BLOCK_SIZE, WRITE_POLICY);
  //d_Cache = CreateCache(D_CACHE_SIZE, D_BLOCK_SIZE, WRITE_POLICY);
  iWriteCache(iCache, address1, data1);
  iWriteCache(iCache, address2, data2);
  iCacheRead(iCache, address3, data2);
  iCacheRead(iCache, address4, data1);
  PrintCache(iCache);
  //d_WriteCache(d_Cache, 0x00054321, 0x12345678);
  //PrintCache(d_Cache);
}
