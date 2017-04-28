//Direct Mapped Cache

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "cache.h"
#include "Load_Program.h"

unsigned int write_buffer[BLOCK_WORDS]; //write buffer to hold data and save clock cycles
bool fullBuff = false;
unsigned int oldCount = 0;

//Structs (Cache and Block and Boffset)
struct Boffset_ {
  unsigned int data;
};

struct Block_ {
  //valid and dirty are not part of the block address
  int valid; //allows you to know if block is filled
  unsigned int tag;
  int dirty; //whether data is different than that stored in main memory. WB/WT stuff
  //unsigned int data;
  Boffset* boffset;
};

struct Cache_ {
  int hits;
  int misses;
  int reads; //cache reads
  int writes; //memory writes
  int cache_size;
  int block_size;
  int lines;
  int write_policy; //WB or WT
  Block* blocks;
};

//create a cache (I cache and D cache)
Cache CreateCache(int cache_size)
{
  Cache cache;
  int i = 0;
  int j = 0;
//making sure that user input is correct
  if(cache_size <= 0)
  {
    printf("Cache size needs to be greater than zero, nice try\n");
    return NULL;
  }

  if(BLOCK_WORDS <= 0)
  {
    printf("Block size needs to be greater than zero, nice try\n");
    return NULL;
  }

  if(WRITE_POLICY != 0 && WRITE_POLICY != 1)
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
  cache->write_policy = WRITE_POLICY;
  cache->cache_size = cache_size;
  cache->block_size = BLOCK_WORDS;
  cache->lines = (int)(cache_size/BLOCK_WORDS);
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
    //cache->blocks[i]->data = 0x00000000;
    cache->blocks[i]->boffset = (Boffset*) malloc(sizeof(Boffset) * BLOCK_WORDS);
    for(j=0; j<BLOCK_WORDS; j++)
    {
      cache->blocks[i]->boffset[j] = (Boffset) malloc(sizeof(struct Boffset_));
      cache->blocks[i]->boffset[j]->data = 0x00000000;
    }
  }

  return cache;
}

unsigned int iCacheRead(Cache cache, unsigned int address)
{
  int i;
  //cycleCount = cycleCount + 1;
  /* Check inputs */
  if(cache == NULL)
  {
    printf("Put in a real cache.\n");
    return 0;
  }

  int block_address = ((address >> I_OFFSET) & I_BLOCK_MASK); //get block address
  unsigned int tag = (address >> I_OFFSET >> I_INDEX); //get tag
  int blockoffset = (address & OFFSET_MASK);

  if(DEBUG)
  {
    printf("Block: %i\nTag: %i\nBlock Offset: %i\n", block_address,tag,blockoffset);
  }

  if(cache->blocks[block_address]->valid == 1 && cache->blocks[block_address]->tag == tag) //cache hit
  {
    cache->hits++;
    return cache->blocks[block_address]->boffset[blockoffset]->data;
  }
  else //cache miss
  {
    cache->misses++;
    cache->reads++; //memory read
    cycleCount = cycleCount + 14;
    //perform main memory read
    //block size of 1, grab 1 word and put in cache
    //block size of 4, grab the 4 words with same block and tag, but stop at offset you want and let pipeline run the get other reads come when memory isnt busy
    //block size of 16, grab the 16 words with same block and tag, but stop at offset you want and let pipeline run the get other reads come when memory isnt busy
    if(BLOCK_WORDS == 1){cache->blocks[block_address]->boffset[blockoffset]->data = memory[address];} //put data from main memory to cach

    int plus = 0;
    int minus = 0;
    if(BLOCK_WORDS != 1)
    {
      for(i=blockoffset;i<BLOCK_WORDS;i++)
      {
        cache->blocks[block_address]->boffset[blockoffset+plus]->data = memory[address+plus];
        plus++;
      }
      for(i=blockoffset;i>=0;i--)
      {
        cache->blocks[block_address]->boffset[blockoffset-minus]->data = memory[address-minus];
        minus++;
      }
    }

    }

    cache->blocks[block_address]->tag = tag;
    cache->blocks[block_address]->valid = 1;

    return cache->blocks[block_address]->boffset[blockoffset]->data;
}

unsigned int d_CacheRead(Cache cache, unsigned int address)
{
  int i = 0;
  /* Check inputs */
  if(cache == NULL)
  {
    printf("Put in a real cache.\n");
    return 0;
  }

  int block_address = ((address >> D_OFFSET) & D_BLOCK_MASK); //get block address
  unsigned int tag = (address >> D_OFFSET >> D_INDEX); //get tag
  int blockoffset = (address & OFFSET_MASK);

  if(DEBUG)
  {
    printf("Reading data from block: %i\nTag being used: %i\n", block_address,tag);
  }

  if(cache->blocks[block_address]->valid == 1 && cache->blocks[block_address]->tag == tag)
  {
    cache->hits++;
    return cache->blocks[block_address]->boffset[blockoffset]->data;
    //take data with correct offset from cache and use it
  }
  else //cache miss
  {
    cache->misses++;
    cache->reads++; //memory read
    cycleCount = cycleCount + 8;

    if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 1) //check to see if data in cache is dirty
    {
      //write_buffer[] = entire cache block (how it will actually be implemented)
      memory[address] = cache->blocks[block_address]->boffset[blockoffset]->data; //put old data from cache into main memory
      cache->blocks[block_address]->dirty = 0; //mark data as not dirty dirty
      //perform main memory read
      //for block size of 4, grab the 4 words with same block and tag
      //for block size of 16, grab the 16 words with same block and tag
      cache->blocks[block_address]->boffset[blockoffset]->data = memory[address]; //put data from memory into cache
      //clock_cycles = clock_cycles + PENALTY;
    }
    else if(cache->write_policy == 0)
    {
      //perform main memory read
      if(BLOCK_WORDS == 1){cache->blocks[block_address]->boffset[blockoffset]->data = memory[address];} //put data from main memory read into cache

      int plus = 0;
      int minus = 0;
      if(BLOCK_WORDS != 1)
      {
        for(i=blockoffset;i<BLOCK_WORDS;i++)
        {
          cache->blocks[block_address]->boffset[blockoffset+plus]->data = memory[address+plus];
          plus++;
        }
        for(i=blockoffset;i>=0;i--)
        {
          cache->blocks[block_address]->boffset[blockoffset-minus]->data = memory[address-minus];
          minus++;
        }
      }
    }
    cache->blocks[block_address]->valid = 1;
    cache->blocks[block_address]->tag = tag;

  }

  return cache->blocks[block_address]->boffset[blockoffset]->data;
}

int d_WriteCache(Cache cache, unsigned int address, unsigned int data)
{
  /* Validate inputs */
  if(cache == NULL)
  {
    printf("Put in a real cache.\n");
    return 0;
  }

  int block_address = ((address >> D_OFFSET) & D_BLOCK_MASK); //get block address
  unsigned int tag = (address >> D_OFFSET >> D_INDEX); //get tag
  int blockoffset = (address & OFFSET_MASK);

  if(DEBUG)
  {
    printf("Writing data from block: %i\n",block_address);
    printf("Tag being used: %i\n",tag);
  }

  if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 1)
  {
    //will be implemented with buffer
    memory[address] = cache->blocks[block_address]->boffset[blockoffset]->data; //put old data into memory
    cycleCount = cycleCount + 6;
  }

  if(cache->write_policy == 1)
  {
    cache->blocks[block_address]->dirty = 1;
  }

  cache->blocks[block_address]->boffset[blockoffset]->data = data; //put new data into cache
  cache->blocks[block_address]->tag = tag; //change tag

  if(cache->write_policy == 0)
  {
    //write_buffer[0] = data;
    //cycleCount = cycleCount + 6;
    memory[address] = cache->blocks[block_address]->boffset[blockoffset]->data; //also write to main memory

  }

  cache->writes++;
  cache->blocks[block_address]->valid = 1;
  return 0;
}

/*Prints Cache results*/
int PrintCache(Cache cache)
{
  int i;
  int j;
  int valid;
  int lines;
  int dirty;
  unsigned int tag;
  unsigned int data;
  double hits = cache->hits;
  double misses = cache->misses;

  lines = cache->lines;
  if(cache != NULL)
  {
  for(i=0; i < lines; i++)
  {
      tag = cache->blocks[i]->tag;
      valid = cache->blocks[i]->valid;
      dirty = cache->blocks[i]->dirty;
      printf("BLOCK[%i]:    { valid: %i, dirty: %i tag: %i }", i, valid, dirty, tag);
      for(j=0; j<BLOCK_WORDS; j++)
      {
       data = cache->blocks[i]->boffset[j]->data;
       printf(" DATA[%i]: 0x%08x",j, data);
      }
      printf("\n");
  }
    double hit_rate = (hits/(hits + misses))*100;

    printf("\n\tCACHE HITS: %i\n\tCACHE MISSES: %i\n\tMEMORY READS: %i\n\tCACHE WRITES: %i\n\n\tCACHE SIZE: %i Words\n\tBLOCK SIZE: %i Words\n\tNUM LINES: %i\n\tHIT RATE: %f%%\n\n", cache->hits, cache->misses, cache->reads, cache->writes, cache->cache_size, cache->block_size, cache->lines,hit_rate);
  }
    return 0;
}


/*
int main()
{
  Cache iCache;
  Cache d_Cache;

  unsigned int data1 = 0x77654321;
  unsigned int data2 = 0x73656383;
  unsigned int address1 = 0x8764444;
  signed int address2 = 0x00054321;
  unsigned int address3 = 0x58354321;
  //unsigned int address4 = 0x52554444;

  iCache = CreateCache(I_CACHE_SIZE);
  d_Cache = CreateCache(D_CACHE_SIZE);
  iCacheRead(iCache, address3, data2);
  iCacheRead(iCache, address3, data2);
  iCacheRead(iCache, address1, data1);

  printf("iCache\n");
  PrintCache(iCache);

  d_WriteCache(d_Cache, address1, data1);
  d_WriteCache(d_Cache, address2, data2);
  d_CacheRead(d_Cache, address3, data2);
  d_CacheRead(d_Cache, address1, data1);

  printf("d_Cache\n");
  PrintCache(d_Cache);
}

*/
