/** Name: Zhaojie Jiang
 ** Class: CMSC 421 
 ** Assignment: Project 1
 **  
 ** File: skip_list.c
 **/

#include<stdio.h>
#include<string.h>
#include<unistd.h>

// random number generator 
static unsigned int next_random = 9001;

static unsigned int generate_random_int(void);
static void seed_random(unsigned int seed);

// linked list
typedef strcut node;
void LinkedListInsert(node_t *head, node_t *newNode);

int main(){

  
  node_t *head = NULL;
  unsigned int * skip_list = malloc(sizeof(unsigned int));

  // create a random number
  seed_random(32768);
 
  unsigned int random_int = generate_random_int;

  skip_list[0] = random_int;
  
  printf("%u\n", random_int);

  return 0;
}

// generation random number from 0 - 32768
static unsigned int generate_random_int(void) {

   next_random = next_random * 1103515245 + 12345;

   return (next_random / 65536) % 32768;
}
   
static void seed_random(unsigned int seed) {
    next_random = seed;
}

// create a node structure
typedef struct node{

  unsigned int data;
  char message;

  struct node *next;
  struct node *mailbox;
} node_t;

// insert node to the sorted link list
void LinkedListInsert(node_t **head, node_t * newNode){
  node_t * current;

  if(head == NULL || (*head->data)->data >= newNode-> data ){
    newNode -> next = *head;
    *head = new_node;
  }

  else{
    current = *head;
    while(current->next != NULL && current->next->data < newNode->data){
      current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
  }
}
