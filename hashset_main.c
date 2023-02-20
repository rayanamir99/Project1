// main function for a linked list manager, student version

#include <stdio.h>
#include <string.h>
#include "hashset.h"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }

  printf("Hashset Application\n");
  printf("Commands:\n");
  printf("  hashcode <elem>  : prints out the numeric hash code for the given key (does not change the hash set)\n");
  printf("  contains <elem>  : prints the value associated with the given element or NOT PRESENT\n");
  printf("  add <elem>       : inserts the given element into the hash set, reports existing element\n");
  printf("  print            : prints all elements in the hash set in the order they were addded\n");
  printf("  structure        : prints detailed structure of the hash set\n");
  printf("  clear            : reinitializes hash set to be empty with default size\n");
  printf("  save <file>      : writes the contents of the hash set to the given file\n");
  printf("  load <file>      : clears the current hash set and loads the one in the given file\n");
  printf("  next_prime <int> : if <int> is prime, prints it, otherwise finds the next prime and prints it\n");
  printf("  expand           : expands memory size of hash set to reduce its load factor\n");
  printf("  quit             : exit the program\n");
  
  char cmd[128];
  hashset_t hash;
  int success;
  hashset_init(&hash, HASHSET_DEFAULT_TABLE_SIZE);

  while(1){
    printf("HS>> ");                 // print prompt
    success = fscanf(stdin,"%s",cmd); // read a command
    if(success==EOF){                 // check for end of input
      printf("\n");                   // found end of input
      break;                          // break from loop
    }

    if( strcmp("quit", cmd)==0 ){     // check for exit command
      if(echo){
        printf("quit\n");
      }
      break;                          // break from loop
    }

    else if( strcmp("hashcode", cmd)==0 ){ // Checks if cmd == "hashcode"
      fscanf(stdin,"%s",cmd);           // Scans in an input for hashcode
      if(echo){ 
        printf("hashcode %s\n",cmd);
      }
      printf("%d \n", hashcode(cmd)); //Print hashcode
    }

    else if( strcmp("contains", cmd)==0 ){ // Checks if cmd == "contains"
      fscanf(stdin,"%s",cmd);            // Scans in an input for contains
      if(echo){
        printf("contains %s\n",cmd);
      }
      success = hashset_contains(&hash, cmd); // Success is equivalent to 1 or 0 depending on if it fails or not
      if(!success){                      // check for success
        printf("NOT PRESENT\n"); // If fail print NOT PRESENT
      }
      else{
        printf("FOUND: %s\n", cmd); // If succeeds print FOUND followed by the value inputted
      }
    }

      else if( strcmp("add", cmd)==0 ){ // Checks if cmd == "add"
      fscanf(stdin,"%s",cmd);            // Scans in an input for add
      if(echo){
        printf("add %s\n",cmd);
      }
      success = hashset_add(&hash, cmd); // Success is equivalent to 1 or 0 depending on if it fails or not
      if(!success){                      // If failed, print add failed
        printf("Elem already present, no changes made\n");
      }
    }
    else if( strcmp("structure", cmd)==0 ){ // Checks if cmd == "structure"   
      if(echo){
        printf("structure\n");
      }
      hashset_show_structure(&hash); // Shows the structure of the hash passed in 
    }

    else if( strcmp("print", cmd)==0 ){ // Checks if cmd == "print"
      if(echo){
        printf("print\n");
      }
      hashset_write_elems_ordered(&hash, stdout); //Prints all elements in hash in the order it was added to the terminal
    }
    else if( strcmp("save", cmd)==0 ){ //Checks if cmd == "save"
    fscanf(stdin, "%s", cmd); // Scans in a file input for save
      if(echo){
        printf("save %s\n", cmd);
      }
      hashset_save(&hash, cmd); // Writes the contents of hash to a file
    }
    else if( strcmp("next_prime", cmd)==0 ){ //Checks if cmd == "next_prime"
      int num;
      fscanf(stdin, "%d", &num); // Scans in an int input for next_prime
      if(echo){
        printf("next_prime %d\n", num);
      }
      int prime_num = next_prime(num); //If num is prime, prints it, else finds the next prime num
      printf("%d\n", prime_num);
    }
    else if( strcmp("load", cmd)==0 ){ //Checks if cmd == "load"
      fscanf(stdin, "%s", cmd); // Scans in a file input for load
      if(echo){
        printf("load %s\n", cmd);
      }
      success = hashset_load(&hash, cmd); // Success is equivalent to 1 or 0 depending on if it fails or not
      if(!success){                      // check for success
        printf("load failed\n"); // If fail print NOT PRESENT
      }
    }

    else if( strcmp("clear", cmd)==0 ){ // Checks if cmd == "clear"
      if(echo){
        printf("clear\n");
      }
      hashset_free_fields(&hash); //Frees hash
      hashset_init(&hash, HASHSET_DEFAULT_TABLE_SIZE); //Initializes a new hash
    }
    else if( strcmp("expand", cmd)==0 ){ // Checks if cmd == "expands"
      if(echo){
        printf("expand\n");
      }
      hashset_expand(&hash); //Frees hash
    }

    else{                                 // unknown command
      if(echo){
        printf("%s\n",cmd);
      }
      printf("unknown command %s\n",cmd);
    }
  }  

  hashset_free_fields(&hash);            
  return 0;
}
