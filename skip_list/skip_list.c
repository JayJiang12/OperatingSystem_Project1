/** Name: Zhaojie Jiang
 ** Class: CMSC 421 
 ** Assignment: Project 1
 **  
 ** File: skip_list.c
 **/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

// node for the skiplist 
typedef struct node{
  unsigned int id; 
  char *msg; 
  
  struct node **next; 
  
}node;

// skiplist
typedef struct skiplist{ 
  unsigned int level; 
  unsigned int size; 

  struct node *header;
  struct node *tail;

}skiplist;

/* initializes the mailbox system, setting up the initial 
   state of the skip list.
   ptrs parameter is the maximum of pointers any node in the list 
   will be allowed to have.
   prob parameter inverse of the probability that a node in the list
   will be promoted to have an additional pointer in it
 */
long slmbx_init(unsigned int ptrs, unsigned int prob);

long slmbx_shutdown(void);

long slmbx_create(unsigned int id, int protected);

long slmbx_destroy(unsigned int id);

long slmbx_count(unsigned int id);

long slmbx_send(unsigned int id, const unsigned char *msg);

long slmbox_recv(unsigned int id, unsigned char *msg, unsigned int len);

long slmbx_length(unsigned int id);

// get random number
unsigned int rand_level();

// power of n
long int powerOf(unsigned int x, unsigned int n);

int find(unsigned int id);

// Global Variable
skiplist *LIST; // skip list
unsigned int MAX_LEVEL; // max level of the skip list
unsigned int LIST_PROB; // probability that a node will be promoted
                                //to having additional pointer in skiplist.

int main(){

  slmbx_init(10, 2);
  return 0;
}

/* initializes the mailbox system, setting up the initial 
   state of the skip list.
   ptrs parameter is the maximum of pointers any node in the list 
   will be allowed to have.
   prob parameter inverse of the probability that a node in the list
   will be promoted to have an additional pointer in it
 */
long slmbx_init(unsigned int ptrs, unsigned int prob){

  MAX_LEVEL = ptrs;

  // create probability for additional pointer.
  if(prob == 2 || prob == 4 || prob == 8 || prob == 16)
    LIST_PROB = prob;
  else{
    printf("Error! Wrong probability entered. Please enter 2, 4, 8 or 16.");
    return 0;
  }

  LIST  = malloc(sizeof(skiplist));
  node *header = malloc(sizeof(node));
  
  LIST->level = 1;
  LIST->size = 0;  

  LIST->header = header;
  header->next = malloc(sizeof(node));

  int i;
  for(i = 0; i < MAX_LEVEL; i++)
    header->next[i] = LIST->header;

  return 0;
}

long slmbx_shutdown(void){
  return 0;
}


// create a new mailbox with given id
long slmbx_create(unsigned int id, int protected){

  if (id == 0 || id == powerOf(2, 32) - 1){
    printf("Invalid id is entered");
    return 0;
  }

  node *update[MAX_LEVEL + 1];
  node *x = LIST->header;

  int i, level;

  // update order of id
  for(i = LIST->level; i >= 1; i--){
    while(x->next[i]->id < id)
      x = x->next[i];
    update[i] = x;
  }

  x = x->next[1];

  // check if id is already there
  if(id == x->id){
    printf("duplicates id");
    return 0;
  }

  // check if need to add new pointer
  else{
    level = rand_level();
    if(level > LIST->level){
      for(i = LIST->level + 1; i<= level; i++){
	update[i] = LIST->header;
      }
      LIST->level = level;
    }

    x = malloc(sizeof(node));
    x -> id = id;
    // x -> msg = msg;
    x -> next = malloc(sizeof(node));

    for(i = 1; i <= level; i++){
      x->next[i] = update[i]->next[i];
      update[i]->next[i] = x;
    }		       
  }    
  
  return 0;
}

long slmbx_destroy(unsigned int id){
  return 0;
}

long slmbx_count(unsigned int id){
  return 0;
}
  

long slmbx_send(unsigned int id, const unsigned char *msg){
  return 0;
}

long slmbox_recv(unsigned int id, unsigned char *msg, unsigned int len){
  return 0;
}

long slmbx_length(unsigned int id){

  return 0;
}

unsigned int rand_level(){
  unsigned int level = 1;
  while(rand() < RAND_MAX / powerOf(LIST_PROB, 2) && level < MAX_LEVEL){
	level++;
    }
   return level;
 }	

// power of n
long int powerOf(unsigned int x, unsigned int n){
  int i;
  long int number = 1;
  for (i = 0; i < n; ++i)
    number *= x;

  return number;
}

int find(unsigned int id){
  node *x = LIST->header;
  int i;

  // going for top to down
  for(i = LIST->level; i >=1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }
  if(x->next[1]->id == id)
    return 0;
  else
    return 1;
}
  
