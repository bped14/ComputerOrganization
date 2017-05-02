//Direct Mapped Cache

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "cache.h"
#include "Load_Program.h"

//GLOBALS
//unsigned int WT_[1] = {0}; //WT buffer
//unsigned int WB_[BLOCK_WORDS] = {0}; //WB buffer
unsigned int oldCount_iRead = 0;
unsigned int oldCount_dRead = 0;

unsigned int oldCount_WT = 0;
unsigned int oldCount_WB = 0;

unsigned int oldAddress_i = 0;
unsigned int oldBlockaddress_i = 0;
int oldBlockoffset_i = 0;
unsigned int diff = 0;

unsigned int oldAddress_d = 0;
unsigned int oldBlockaddress_d = 0;
unsigned int oldBlockoffset_d = 0;

unsigned int oldCount_dRead1 = 0;
unsigned int oldBlockaddress_d1 = 0;
unsigned int oldBlockoffset_d1 = 0;
unsigned int oldAddress_d1 = 0;

unsigned int iPenalty = 0;
unsigned int dPenalty = 0;

bool WT_full = false;
bool WB_full = false;

/*Function to find block offsets and indexes */
int blockindexoffset(int block_size, int cache_size)
{
  int indexoffset = log(cache_size/block_size)/log(2);
  return indexoffset;
}

unsigned int blockindexmask(int block_size, int cache_size)
{
  int blockmask = pow(2,log(cache_size/block_size)/log(2))-1;
  return blockmask;
}

int blockoffset(int block_size)
{
  int offset = log(block_size)/log(2);
  return offset;
}

unsigned int blockoffsetmask(int block_size)
{
  unsigned int mask;
  switch(block_size)
  {
    case 1:
      mask = 0x00000000;
      break;
    case 4:
      mask = 0x00000003;
      break;
    case 16:
      mask = 0x0000000f;
      break;
    default:
      break;
  }
  return mask;
}



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

  //initialize blocks, fill valid and dirty bits with zero

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
  int plus = 0;
  int minus = 0;

  int Ioffset = blockoffset(BLOCK_WORDS);
  unsigned int Iblockmask = blockindexmask(BLOCK_WORDS,I_CACHE_SIZE);
  int Iindex = blockindexoffset(BLOCK_WORDS,I_CACHE_SIZE);
  unsigned int offsetmask = blockoffsetmask(BLOCK_WORDS);

  /* Check inputs */
  if(cache == NULL)
  {
    printf("Put in a real cache.\n");
    return 0;
  }
    diff = cycleCount - oldCount_iRead;
    //printf("cyclCountafter = %i\n",cycleCount);
    if(diff >= iPenalty)
    {
      for(i=oldBlockoffset_i;i<BLOCK_WORDS;i++) //going from offset to offset size
      {
        cache->blocks[oldBlockaddress_i]->boffset[oldBlockoffset_i+plus]->data = memory[oldAddress_i+plus];
        plus++;
      }
      plus = 0;
      iPenalty = 0;
    }
    if(diff < iPenalty)
    {
      cycleCount = cycleCount + diff;
      for(i=oldBlockoffset_i;i<BLOCK_WORDS;i++) //going from offset to offset size
      {
        cache->blocks[oldBlockaddress_i]->boffset[oldBlockoffset_i+plus]->data = memory[oldAddress_i+plus];
        plus++;
      }
      plus = 0;
      iPenalty = 0;
    }

  int block_address = ((address >> Ioffset) & Iblockmask); //get block address
  unsigned int tag = (address >> Ioffset >> Iindex); //get tag
  int blockoffset = (address & offsetmask);
  iPenalty = ((BLOCK_WORDS-1) - blockoffset)*2;

  if(DEBUG){printf("Block: %i\nTag: %i\nBlock Offset: %i\n", block_address,tag,blockoffset);}

  if(cache->blocks[block_address]->valid == 1 && cache->blocks[block_address]->tag == tag) //cache hit
  {
    cache->hits++;
    return cache->blocks[block_address]->boffset[blockoffset]->data;
  }
  else //cache miss
  {
    cache->misses++;
    cache->reads++;

    if(BLOCK_WORDS != 0)
    {
      for(i=blockoffset;i>=0;i--) //going from offset to zero, putting only what you need in cache
      {
        cache->blocks[block_address]->boffset[blockoffset-minus]->data = memory[address-minus];
        minus++;
      }
        cycleCount = cycleCount + 8 + (((BLOCK_WORDS-1)-((BLOCK_WORDS-1)-blockoffset))*2);
        oldCount_iRead = cycleCount;
        oldBlockaddress_i = block_address;
        oldBlockoffset_i = blockoffset;
        oldAddress_i = address;
    }
  }
    cache->blocks[block_address]->tag = tag;
    cache->blocks[block_address]->valid = 1;

    return cache->blocks[block_address]->boffset[blockoffset]->data;
}

unsigned int d_CacheRead(Cache cache, unsigned int address)
{
  int i = 0;
  int plus = 0;
  int minus = 0;
  /* Check inputs */
  if(cache == NULL){printf("Put in a real cache.\n");return 0;}

  int Doffset = blockoffset(BLOCK_WORDS);
  unsigned int Dblockmask = blockindexmask(BLOCK_WORDS,D_CACHE_SIZE);
  int Dindex = blockindexoffset(BLOCK_WORDS,D_CACHE_SIZE);
  unsigned int offsetmask = blockoffsetmask(BLOCK_WORDS);

  int block_address = ((address >> Doffset) & Dblockmask); //get block address
  unsigned int tag = (address >> Doffset >> Dindex); //get tag
  int blockoffset = (address & offsetmask);
  dPenalty = ((BLOCK_WORDS-1) - blockoffset)*2;

//start of WB fill
    if(cache->write_policy == 1)
    {
      diff = cycleCount - oldCount_dRead1;
      if(diff >= dPenalty)
      {
        for(i=oldBlockoffset_d1;i<BLOCK_WORDS;i++) //going from offset to offset size
        {
          cache->blocks[oldBlockaddress_d1]->boffset[oldBlockoffset_d1+plus]->data = memory[oldAddress_d1+plus];
          plus++;
        }
        plus = 0;
        dPenalty = 0;
      }
      if(diff < dPenalty)
      {
        cycleCount = cycleCount + diff;
        for(i=oldBlockoffset_d1;i<BLOCK_WORDS;i++) //going from offset to offset size
        {
          cache->blocks[oldBlockaddress_d1]->boffset[oldBlockoffset_d1+plus]->data = memory[oldAddress_d1+plus];
          plus++;
        }
        plus = 0;
        dPenalty = 0;
      }
  }
//end of WB fill

if(cache->write_policy == 0)
{
  diff = cycleCount - oldCount_dRead;
  if(diff >= dPenalty)
  {
    for(i=oldBlockoffset_d;i<BLOCK_WORDS;i++) //going from offset to offset size
    {
      cache->blocks[oldBlockaddress_d]->boffset[oldBlockoffset_d+plus]->data = memory[oldAddress_d+plus];
      plus++;
    }
    plus = 0;
    dPenalty = 0;
  }
  if(diff < dPenalty)
  {
    cycleCount = cycleCount + diff;
    for(i=oldBlockoffset_d;i<BLOCK_WORDS;i++) //going from offset to offset size
    {
      cache->blocks[oldBlockaddress_d]->boffset[oldBlockoffset_d+plus]->data = memory[oldAddress_d+plus];
      plus++;
    }
    plus = 0;
    dPenalty = 0;
  }
}

  if(DEBUG){printf("BLOCK ADDRESS: %i\nTAG: %i\n", block_address,tag);}

  if(cache->blocks[block_address]->valid == 1 && cache->blocks[block_address]->tag == tag)
  {
    cache->hits++;
    return cache->blocks[block_address]->boffset[blockoffset]->data;
  }
  else //cache miss
  {
    cache->misses++;
    cache->reads++; //memory read

    if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 1) //check to see if data in cache is dirty
    {
      cache->blocks[block_address]->dirty = 0; //mark data as not dirty dirty
      for(i=blockoffset;i>=0;i--) //put new data into cache
      {
        cache->blocks[block_address]->boffset[blockoffset]->data = memory[address]; //put data from memory into cache
        minus++;
      }
      cycleCount = cycleCount + 6 + ((BLOCK_WORDS-1)-((BLOCK_WORDS-1)-blockoffset)*2);
      oldCount_dRead1 = cycleCount;
      oldBlockaddress_d1 = block_address;
      oldBlockoffset_d1 = blockoffset;
      oldAddress_d1 = address;
      WB_full = false; //buffer is emptied
    }
    else if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 0)
    {
      for(i=blockoffset;i>=0;i--) //put data in memory
      {
        cache->blocks[block_address]->boffset[blockoffset]->data = memory[address]; //put data from memory into cache
        minus++;
      }
      WB_full = true;
      //cycleCount = cycleCount + 6 + ((BLOCK_WORDS-1)-((BLOCK_WORDS-1)-blockoffset)*2);
      oldCount_dRead1 = cycleCount;
      oldBlockaddress_d1 = block_address;
      oldBlockoffset_d1 = blockoffset;
      oldAddress_d1 = address;
    }
    else if(cache->write_policy == 0)
    {
      //perform main memory read
      for(i=blockoffset;i>=0;i--)
      {
        cache->blocks[block_address]->boffset[blockoffset-minus]->data = memory[address-minus];
        minus++;
      }
      cycleCount = cycleCount + 6 + ((BLOCK_WORDS-1)-((BLOCK_WORDS-1)-blockoffset)*2);
      oldCount_dRead = cycleCount;
      oldBlockaddress_d = block_address;
      oldBlockoffset_d = blockoffset;
      oldAddress_d = address;
    }
    cache->blocks[block_address]->valid = 1;
    cache->blocks[block_address]->tag = tag;
  }
  return cache->blocks[block_address]->boffset[blockoffset]->data;
}

int d_WriteCache(Cache cache, unsigned int address, unsigned int data)
{
  /* Validate inputs */
  if(cache == NULL){printf("Put in a real cache.\n");return 0;}

  int Doffset = blockoffset(BLOCK_WORDS);
  unsigned int Dblockmask = blockindexmask(BLOCK_WORDS,D_CACHE_SIZE);
  int Dindex = blockindexoffset(BLOCK_WORDS,D_CACHE_SIZE);
  unsigned int offsetmask = blockoffsetmask(BLOCK_WORDS);

  int block_address = ((address >> Doffset) & Dblockmask); //get block address
  unsigned int tag = (address >> Doffset >> Dindex); //get tag
  int blockoffset = (address & offsetmask);

  if(DEBUG){printf("BLOCK ADDRESS: %i\tTAG: %i\tOFFSET: %i\n",block_address,tag,blockoffset);}

  if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 1)
  {
    cache->blocks[block_address]->boffset[blockoffset]->data = data; //put new data in cache
    cache->blocks[block_address]->tag = tag; //change tag
    memory[address] = data; //putting dirty data in "buffer"
    WB_full = true; //put data in buffer
  }

  if(cache->write_policy == 1 && cache->blocks[block_address]->dirty == 0)
  {
    cache->blocks[block_address]->dirty = 1; //mark new data as dirty
    cache->blocks[block_address]->boffset[blockoffset]->data = data; //put new data in cache
    cache->blocks[block_address]->tag = tag; //change tag
    memory[address] = data;
  }

  diff = cycleCount - oldCount_WT;
  //printf("diff: %i\tBUFFER: %i\tADDRESS: %i\t",diff, WT_[0],address);
    if(cache->write_policy == 0)
    {
      cache->blocks[block_address]->boffset[blockoffset]->data = data;//put new data in cache
      cache->blocks[block_address]->tag = tag; //change tag
      if(diff >= 6 && WT_full == true)
      {
        //put buffer data in main memory
        WT_full = false; //buffer is now empty
      }
      if(diff < 6 && WT_full == true)
      {
        cycleCount = cycleCount + diff; //add penalty for memory write
        WT_full = false; //empty buffer into memory
      }
      memory[address] = data;
      WT_full = true; //mark buffer as full
      oldCount_WT = cycleCount;
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
