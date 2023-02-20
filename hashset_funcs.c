#include "hashset.h"
#include <stdlib.h>
#include <string.h>
// hashset_funcs.c: utility functions for operating on hash sets. Most
// functions are used in the hashset_main.c which provides an
// application to work with the functions.

// PROVIDED: Compute a simple hash code for the given character
// string. The code is "computed" by casting the first 8 characters of
// the string to a numbers and returning their xor. The empty string
// has hash code 0. Longer strings will have numbers which are the
// integer interpretation of up to their first 8 bytes.  ADVANTAGE:
// constant time to compute the hash code. DISADVANTAGE: poor
// distribution for long strings; all strings with same first 8 chars
// hash to the same location.
int hashcode(char key[]){ //Returns a num associated with the key that is passed in
// A hash function is utilized here to optimize time complexity and efficiency
  int hc = 0;
  for(int i=0; key[i]!='\0'; i++){
    hc = hc*31 + key[i];
  }
  return hc;
}\

void hashset_init(hashset_t *hs, int table_size){
    hs -> table_size = table_size; 
    hs -> elem_count = 0;
    hs -> order_first = NULL;
    hs -> order_last = NULL;
    // Allocates memory of size hashnode_t * table_size for hs -> table
    hs -> table = malloc(hs -> table_size * sizeof(hashnode_t*));
    for(int i = 0; i < hs -> table_size; i++){
      // Initializes each bucket to NULL
      hs -> table[i] = NULL;
    }
    return;
}
// Initialize the hash set 'hs' to have given size and elem_count
// 0. Ensures that the 'table' field is initialized to an array of
// size 'table_size' and is filled with NULLs. Also ensures that the
// first/last pointers are initialized to NULL
int hashset_contains(hashset_t *hs, char elem[]){
  // Determines what bucket the elem should be placed in
  int num = hashcode(elem) % hs->table_size;
  if(num < 0){ // Ensures that num is positive
    num = num * -1;
  }
  hashnode_t *p = hs -> table[num];
  while(p != NULL){
    if(strcmp(p->elem, elem) == 0){ // Checks if elem is found in hashset
      return 1;
    }
    else{
      p = p -> table_next; // Iterates through the table 
    }
    
  }
  return 0;
}

// Returns 1 if the parameter `elem` is in the hash set and 0
// otherwise. Uses hashcode() and field `table_size` to determine
// which index in table to search.  Iterates through the list at that
// table index using strcmp() to check for `elem`. NOTE: The
// `hashcode()` function may return positive or negative
// values. Negative values are negated to make them positive. The
// "bucket" (index in hs->table) for `elem` is determined by with
// 'hashcode(key) modulo table_size'.

int hashset_add(hashset_t *hs, char elem[]){
  if(hs == NULL){ // Checks if hs exists
    hashset_init(hs, HASHSET_DEFAULT_TABLE_SIZE);
  }
  if(hashset_contains(hs, elem) == 1){ // Checks if the elem attempting to be added is a duplicate
    return 0;
  }
  int num = hashcode(elem) % hs->table_size;  // Determines what bucket the elem should be placed in
  if(num < 0){ // Ensures that num is positive
    num = num * -1;
  }
  hashnode_t* node = malloc(sizeof(hashnode_t)); 
  hashnode_t *p = hs -> table[num];


if(hs -> order_first == NULL){ // If this is the first element being placed, then it is both the 
// first and last node and there should be no order_next or table_next. Along with this, the elem_count
// should be equivalent to 1. Should also be first in the bucket.
  strcpy(node -> elem, elem);
  hs -> order_first = node;
  hs -> order_last = node;
  hs -> table[num] = node;
  node -> order_next = NULL;
  node -> table_next = NULL;
  hs -> elem_count = 1;
  return 1;
}
if(p != NULL){ // If this is not the first element being placed in the bucket then it should be inserted
// at the start of the bucket and all of the links should be redone. The elem count should also be incrememnted
// and order_last should be equivalent to the new node. Order_next should still be NULL because this
// is the last element being added
  strcpy(node -> elem, elem);
  node -> order_next = NULL;
  hs -> order_last -> order_next = node;
  hs -> order_last = node;
  node -> table_next = p;
  hs -> table[num] = node;
  hs -> elem_count++;
  return 1;
}
else{
  // If this is the first element being placed into the bucket it should be inserted the the start and both
  // table_next and order_next should be NULL because this is the last node in both the bucket and the linked
  // list. The elem count should also be incrememnted and order_last should be equivalent to the new node.
  strcpy(node -> elem, elem);
  hs -> table[num] = node;
  node -> table_next = NULL;
  node -> order_next = NULL;
  hs -> order_last -> order_next = node;
  hs -> order_last = node;
  hs -> elem_count++;
  return 1;
}
}
// If the element is already present in the hash set, makes no changes
// to the hash set and returns 0. hashset_contains() may be used for
// this. Otherwise determines the bucket to add `elem` at via the same
// process as in hashset_contains() and adds it to the FRONT of the
// list at that table index. Adjusts the `hs->order_last` pointer to
// append the new element to the ordered list of elems. If this is the
// first element added, also adjsuts the `hs->first` pointer. Updates the
// `elem_count` field and returns 1 to indicate a successful addition.
//
// NOTE: Adding elems at the front of each bucket list allows much
// simplified logic that does not need any looping/iteration.

void hashset_free_fields(hashset_t *hs){
  hashnode_t *p = hs-> order_first;
  hashnode_t *iterator = hs -> order_first;
  // Iterates through the linked list and frees each individual node while also
  // incrementing to the next node. 
  while(iterator != NULL){
    p = iterator;
    iterator = iterator -> order_next;
    free(p);
  }
  // Sets the rest of the fields to either NULL or 0
   hs -> elem_count = 0;
   hs -> order_first = NULL;
   hs -> order_last = NULL;
   free(hs -> table);
   hs -> table = NULL;
   return;
}
// De-allocates nodes/table for `hs`. Iterates through the ordered
// list of the hash set starting at the `order_first` field and
// de-allocates all nodes in the list. Also free's the `table`
// field. Sets all relevant fields to 0 or NULL as appropriate to
// indicate that the hash set has no more usable space. Does NOT
// attempt to de-allocate the `hs` itself as it may not be
// heap-allocated (e.g. in the stack or a global).

void hashset_show_structure(hashset_t *hs){
  printf("elem_count: %d\n", hs -> elem_count);
  printf("table_size: %d\n", hs -> table_size);
  if(hs-> order_first == NULL){
    printf("order_first: NULL\n");
  }
  else{
    printf("order_first: %s\n", hs->order_first->elem);
  }
  if(hs-> order_last == NULL){
    printf("order_last : NULL\n");
  }
  else{
    printf("order_last : %s\n", hs->order_last->elem);
  }
  // Calculates the double load_factor by dividing double elem_c by double table_s
  double elem_c = hs -> elem_count;
  double table_s = hs -> table_size;
  double load_factor = elem_c / table_s;
  printf("load_factor: %.4lf\n", load_factor);
  if(hs -> table == NULL){
    for(int i = 0; i < hs -> table_size; i++){
      printf("[%2d] :\n", i);
    }
    return;
  }
  // Prints the bucket number, hashcode, current element, and next element
  for(int i = 0; i < hs -> table_size; i++){
    hashnode_t *p = hs -> table[i];
    printf("[%2d] : ", i);
    while(p != NULL){ // While there is a value at p
      printf("{%d ", hashcode(p -> elem));
      printf("%s >>", p -> elem);
      if(p -> order_next != NULL){ // If this is not the last node in the linked list 
        printf("%s} ", p -> order_next -> elem);
      }
      else{
        printf("NULL} ");
      }
      p = p -> table_next; // Increment to the next value in the bucket
    }
    printf("\n");
  }
  return;
 }
// Displays detailed structure of the hash set. Shows stats for the
// hash set as below including the load factor (element count divided
// by table_size) to 4 digits of accuracy.  Then shows each table
// array index ("bucket") on its own line with the linked list of
// elems in the bucket on the same line. 
// 
// EXAMPLE:
// elem_count: 4
// table_size: 5
// order_first: Rick
// order_last : Tinyrick
// load_factor: 0.8000
// [ 0] : {7738144525137111380 Tinyrick >>NULL} 
// [ 1] : 
// [ 2] : 
// [ 3] : {125779953153363 Summer >>Tinyrick} {1801677138 Rick >>Morty} 
// [ 4] : {521644699469 Morty >>Summer} 
//
// NOTES:
// - Uses format specifier "[%2d] : " to print the table indices
// - Nodes in buckets have the following format:
//   {1415930697 IceT >>Goldenfold}
//    |          |       |        
//    |          |       +-> order_next->elem OR NULL if last node
//    |          +->`elem` string     
//    +-> hashcode("IceT"), print using format "%ld" for 64-bit longs
// 

void hashset_write_elems_ordered(hashset_t *hs, FILE *out){
  hashnode_t *p = hs -> order_first;
  int count = 1;
  while(p != NULL){
    fprintf(out, "  %d %s\n", count,  p -> elem); // Prints each element in the order in which it is added
    p = p -> order_next; // Increment to the next value in the linked list
    count++;
  }
}
// Outputs all elements of the hash set according to the order they
// were added. Starts at the `order_first` field and iterates through
// the list defined there. Each element is printed on its own line
// preceded by its add position with 1 for the first elem, 2 for the
// second, etc. Prints output to `FILE *out` which should be an open
// handle. NOTE: the output can be printed to the terminal screen by
// passing in the `stdout` file handle for `out`.

void hashset_save(hashset_t *hs, char *filename){
  FILE *fp = fopen(filename, "w"); // Opens a file to write to
  if(fp == NULL){
    printf("ERROR: could not open file '%s'\n", filename);
    return;
  }
  fprintf(fp, "%d %d\n", hs -> table_size, hs -> elem_count); // Write the table_size and elem_count to the file
  hashset_write_elems_ordered(hs, fp); // Write the elements in order to the file
  fclose(fp);
}
// Writes the given hash set to the given `filename` so that it can be
// loaded later.  Opens the file and writes its 'table_size' and
// 'elem_count' to the file. Then uses the hashset_write_elems_ordered()
// function to output all elems in the hash set into the file.
// EXAMPLE FILE:
// 
// 5 6
//   1 Rick
//   2 Morty
//   3 Summer
//   4 Jerry
//   5 Beth
//   6 Tinyrick
// 
// First two numbers are the 'table_size' and 'elem_count' field and
// remaining text is the output of hashset_write_elems_ordered();
// e.g. insertion position and element.

int hashset_load(hashset_t *hs, char *filename){
  FILE *fp = fopen(filename, "r"); // Opens a file to read to
  if (fp == NULL){
    printf("ERROR: could not open file '%s'\n", filename);
    return 0;
  }
  char val[128];
  // The two following values are 2 values that are being scanned for in fp
  int upd_table; 
  int upd_elem;
  fscanf(fp, "%d %d", &upd_table, &upd_elem);
  // Frees the current hashset_t hs
  hashset_free_fields(hs);
  // Initializes the new hashset_t hs with the values from upd_table
  hashset_init(hs, upd_table);
  for(int i = 0; i < upd_elem; i++){
    fscanf(fp, "%*d %s", val);
    hashset_add(hs, val); // Adds the values from the file into the new hashset_t hs
  }
  fclose(fp);
  return 1;
}
// Loads a hash set file created with hashset_save(). If the file
// cannot be opened, prints the message
// 
// ERROR: could not open file 'somefile.hs'
//
// and returns 0 without changing anything. Otherwise clears out the
// current hash set `hs`, initializes a new one based on the size
// present in the file, and adds all elems from the file into the new
// hash set. Ignores the indices at the start of each line and uses
// hashset_add() to insert elems in the order they appear in the
// file. Returns 0 on successful loading. This function does no error
// checking of the contents of the file so if they are corrupted, it
// may cause an application to crash or loop infinitely.

int next_prime(int num){
// int range = num/2;
for(int i = 2; i < num/2; i++){
  if (num % i == 0){ //Checks if the number is divisible by any number between 2 and half of itself
    if(num % 2 == 0){
      num++; // If there is no remainder when divided by 2, means the number is even so it is incremented 1
    }
    else{
      num += 2; // If there is a remainder when divided by 2, means it is odd so it is incremented by 2
    }
  }
}
return num;
}
// If 'num' is a prime number, returns 'num'. Otherwise, returns the
// first prime that is larger than 'num'. Uses a simple algorithm to
// calculate primeness: check if any number between 2 and (num/2)
// divide num. If none do, it is prime. If not, tries next odd number
// above num. Loops this approach until a prime number is located and
// returns this. Used to ensure that hash table_size stays prime which
// theoretically distributes elements better among the array indices
// of the table.

void hashset_expand(hashset_t *hs){
  int num = next_prime(2 * hs-> table_size + 1); //Size of new table
  hashset_t *new = malloc(sizeof(hashset_t)); // Allocate memory of size hashset_t for new
  hashset_init(new, num); // Initialize new hashset with size num
  hashnode_t *prev_node = hs -> order_first; 

// Iterate through hs and add each node to new
  while(prev_node != NULL){
    hashset_add(new, prev_node -> elem);
    prev_node = prev_node -> order_next;
  }
  // Free the current values present in the fields of hs
  hashset_free_fields(hs);

// Reinitialize hs
  hashset_init(hs, num);

// Create a new node to loop through new
  hashnode_t *new_node = new -> order_first;

// Iterate through new and add each node to hs
  while(new_node != NULL){
    hashset_add(hs, new_node -> elem);
    new_node = new_node -> order_next;
  }
  hashset_free_fields(new);
  free(new);
}
// Allocates a new, larger area of memory for the `table` field and
// re-adds all current elems to it. The size of the new table is
// next_prime(2*table_size+1) which keeps the size prime.  After
// allocating the new table, all table entries are initialized to NULL
// then the old table is iterated through and all elems are added to
// the new table according to their hash code. The memory for the old
// table is de-allocated and the new table assigned to the hash set
// fields "table" and "table_size".  This function increases
// "table_size" while keeping "elem_count" the same thereby reducing
// the load of the hash table. Ensures that the memory associated with
// the old table is free()'d. Makes NO special effort to preserve old
// nodes: re-adds everything into the new table and then frees the old
// one along with its nodes. Uses functions such as hashset_init(),
// hashset_add(), hashset_free_fields() to accomplish the transfer.