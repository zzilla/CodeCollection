

// HTAB.CPP begins here

#ifdef __GNUC__
  #include <string.h>
#endif

#ifdef __BORLANDC__
  #include <mem.h>
#endif

#include "HTAB.h"

htab::htab(htab_match match, htab_code code, void *pointer)
{
  pmatch = match;
  pcode = code;
  ppointer = pointer;
  memset(queue, 0, sizeof(queue));
  large_block = NULL;
  blocks_in_large_block =
    sizeof(large_block->buffer) / sizeof(large_block->buffer[0]);
}

htab::~htab()
{
  while (large_block != NULL)
  {
    large_item_block *next = large_block->next;
    delete large_block;
    large_block = next;
  }
}

static unsigned char CODETAB[256] =
{
  57, 78, 31, 14, 112, 193, 142, 131, 16, 76, 125, 244, 169, 152, 108, 121,
  94, 51, 223, 10, 67, 52, 61, 54, 187, 38, 165, 148, 87, 250, 107, 104, 159,
  127, 4, 160, 146, 228, 71, 110, 84, 254, 241, 221, 36, 11, 143, 9, 111, 83,
  231, 37, 201, 234, 97, 117, 166, 24, 113, 28, 134, 145, 124, 218, 252, 194,
  181, 85, 120, 22, 21, 232, 8, 206, 203, 34, 253, 236, 89, 167, 176, 237, 33,
  88, 65, 188, 49, 5, 249, 216, 95, 168, 129, 255, 180, 17, 43, 200, 32, 190,
  197, 27, 162, 19, 40, 229, 191, 164, 132, 175, 72, 144, 150, 202, 183, 62,
  15, 170, 226, 99, 212, 220, 210, 141, 205, 42, 23, 119, 56, 140, 138, 58,
  204, 158, 60, 149, 77, 109, 213, 151, 157, 45, 41, 185, 209, 195, 137, 35,
  147, 3, 101, 217, 171, 7, 29, 172, 106, 30, 26, 75, 55, 91, 68, 214, 20, 82,
  227, 196, 245, 240, 98, 230, 247, 102, 18, 251, 182, 48, 139, 207, 47, 215,
  177, 179, 233, 163, 242, 156, 53, 136, 133, 44, 184, 12, 64, 81, 154, 243,
  59, 186, 238, 103, 92, 63, 135, 130, 115, 192, 248, 74, 50, 122, 96, 224,
  116, 66, 100, 199, 189, 174, 128, 13, 70, 6, 173, 153, 69, 246, 211, 0, 178,
  222, 161, 93, 1, 208, 39, 239, 2, 25, 46, 114, 118, 225, 105, 123, 126, 219,
  235, 73, 80, 86, 79, 198, 155, 90
};

// code used to generate CODETAB initializers
//
// #include <stdio.h>
// #include <stdlib.h>
//
// int compare(const void *, const void *)
// {
//   return rand() - RAND_MAX/2;
// }
//
// void main(void)
// {
//   unsigned char s[256];
//   int i;
//   for (i = 0; i < 256; i++)
//     s[i] = i;
//   randomize();
//   qsort(s, 256, 1, compare);
//   qsort(s, 256, 1, compare);
//   qsort(s, 256, 1, compare);
//   qsort(s, 256, 1, compare);
//   qsort(s, 256, 1, compare);
//   for (i = 0; i < 256; i++)
//     printf("%d,\n", s[i]);
// }

unsigned htab::code(const char *name)
{
  unsigned char c = 0;
  while (*name != 0)
    c = CODETAB[c ^ *name++];
  return c;
}

#include <ctype.h>

unsigned htab::codei(const char *name)
{
  unsigned char c = 0;
  while (*name != 0) {
    c = CODETAB[c ^ toupper(*name)];
    name++;
  }
  return c;
}

void * htab::first(void *item)
{
  insert_queue = (*pcode)(item, ppointer);
  insert_point = queue[insert_queue];
  insert_item = item;
  while (insert_point != NULL)
  {
    if ((*pmatch)(item, insert_point->item, ppointer))
      return insert_point->item;
    insert_point = insert_point->next;
  }
  return NULL;
}

void *htab::next(void)
{
  if (insert_point != NULL && insert_point->next != NULL &&
    (*pmatch)(insert_item, insert_point->next->item, ppointer))
  {
    insert_point = insert_point->next;
    return insert_point->item;
  }
  return NULL;
}

void htab::insert(void)
{
  if (blocks_in_large_block ==
    sizeof(large_block->buffer) / sizeof(large_block->buffer[0]))
  {
    large_item_block *b = new large_item_block;
    b->next = large_block;
    large_block = b;
    blocks_in_large_block = 0;
  }
  item_block *ib = &large_block->buffer[blocks_in_large_block++];
  if (insert_point == NULL)
  {
    ib->next = queue[insert_queue];
    queue[insert_queue] = ib;
  }
  else
  {
    ib->next = insert_point->next;
    insert_point->next = ib;
  }
  ib->item = insert_item;
}

void *htab::remove(void)
{
  for (int i = 0; i < 256; i++)
  {
    if (queue[i] != NULL)
    {
      void *p = queue[i]->item;
      queue[i] = queue[i]->next;
      return p;
    }
  }
  return NULL;
}



