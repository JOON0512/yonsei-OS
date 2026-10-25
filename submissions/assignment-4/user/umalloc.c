#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef long Align;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

void
freelist(void) { // Show the chain of free blocks.
  int i = 0;
  Header* p = &base;

  printf("Free list:\n");
  if(!freep) { printf("--\n"); return; } // Free list hasn't been created.
  for(p = p->s.ptr; p != &base; p = p->s.ptr) {
    printf("[%d] p = %p, p->s.size = %d bytes, p->s.ptr = %p\n",
           ++i, p, sizeof(Header) * p->s.size, p->s.ptr);
  } printf("\n");
}

void
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header*
morecore(uint nu)
{
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if(p == (char*)-1)
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
  free((void*)(hp + 1));
  return freep;
}

void*
malloc(uint nbytes)
{
  Header *p, *prevp;
  Header *worstFit = 0;
  Header *worstFitPrev = 0;
  uint nunits;
  uint maxSize = 0;

  // Validate input size: reject 0 bytes or sizes larger than 1GB
  if(nbytes == 0 || nbytes > 1024*1024*1024)
    return 0;

  // Calculate required units including header size, rounded up
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  
  // Initialize free list if this is the first malloc call
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
    if(base.s.ptr == 0 || freep == 0)
      return 0;
  }
  
  // Ensure free list is properly initialized
  if(prevp->s.ptr == 0)
    return 0;
  
  // First pass: Traverse the free list to find the largest available block (worst-fit)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
    if(p == 0)  // Corrupted free list check
      return 0;
      
    // Update worst-fit if current block is larger than previous maximum
    if(p->s.size >= nunits && p->s.size > maxSize) {
      maxSize = p->s.size;
      worstFit = p;
      worstFitPrev = prevp;
    }
    
    // Complete circle in free list
    if(p == freep) {
      if(worstFit == 0) {
        // No suitable block found, request more memory
        p = morecore(nunits);
        if(p == 0)
          return 0;
      } else {
        break;  // Found a suitable block, exit search
      }
    }
  }

  // If we found a worst-fit block, allocate from it
  if(worstFit) {
    p = worstFit;
    prevp = worstFitPrev;
    
    // Split the block if it's larger than needed
    if(p->s.size > nunits) {
      // Create new free block at the start of the region
      Header* newBlock = p;
      newBlock->s.size = p->s.size - nunits;
      newBlock->s.ptr = p->s.ptr;
      prevp->s.ptr = newBlock;
      
      // Setup the allocated block at the end of the region
      p = p + newBlock->s.size;
      p->s.size = nunits;
      freep = prevp;
      return (void*)(p + 1);  // Return pointer to usable memory (after header)
    } else {
      // Use entire block if it's an exact fit
      prevp->s.ptr = p->s.ptr;
      freep = prevp;
      return (void*)(p + 1);
    }
  }
  
  return 0;  // No suitable block found
}
