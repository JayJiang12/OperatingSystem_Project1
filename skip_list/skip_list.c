/** Name: Zhaojie Jiang
 ** Class: CMSC 421 
 ** Assignment: Project 1
 **  
 ** File: skip_list.c
 **/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<linux/file.h>
//#include<asm/file.h>

// node for Queue
typedef struct Qnode{
  unsigned char *data;
  unsigned int length;
  struct Qnode *next;
}Qnode;

// list for queue
typedef struct queue{
  struct Qnode* head;
  struct Qnode* tail;
}queue;
  
// node for the skiplist 
typedef struct node{
  unsigned int id; 
  struct queue *msg; 
  
  struct node **next; 
  
}node;

// skiplist
typedef struct skiplist{ 
  unsigned int level; 
  unsigned int size; 

  struct node *header;
  struct node *tailer;

}skiplist;

/**********************************************
 ****** this section is for skiplist **********
 **********************************************/

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

long slmbx_send(unsigned int id, const unsigned char *msg, unsigned int len);

long slmbx_recv(unsigned int id, unsigned char *msg, unsigned int len);

long slmbx_length(unsigned int id);

// get random number
unsigned int rand_level();

void free_node(node *x);
  
// x power of n
long int powerOf(unsigned int x, unsigned int n);

/* find id 
   if find return 1 else return 0;
 */
unsigned int find(unsigned int id);

// print list
void print();

/******************************************
 ******* this section is for queue ********
 ******************************************/
void print_queue(queue *list);

void print_message(Qnode *ptr);

//unsigned int get_length(unsigned char * buffer);

/******************************************
 **** main and Global varaible started ****
 ******************************************/

// Global Variable
skiplist *LIST; // skip list
unsigned int MAX_LEVEL; // max level of the skip list
unsigned int LIST_PROB; // probability that a node will be promoted
                                //to having additional pointer in skiplist.

int main(){
  printf("start\n");

  // test slmb_init()
   printf("\n*****slmbx_init()*****\n");
  printf("enter prob other than 2, 4, 8, 16 - 3 times\n");
  if(slmbx_init(10, 5) == 1)
    printf("slmbx_int error\n");
  if(slmbx_init(10, 3) == 1)
    printf("slmbx_int error\n");
  if(slmbx_init(14, 45) == 1)
    printf("slmbx_int error\n");

  printf("enter prob 2, 4, 8 or 16.\n");
  if (slmbx_init(10, 2) == 0)
    printf("end of slmbx_init.\n");

  //test slmb_create()
   printf("\n*****slmbx_create*****\n");
  printf("Insert:----------\n");

  unsigned int arr[] = {3, 6, 9, 11, 4, 1, 7};
  unsigned int i;
  for (i = 0; i < sizeof(arr) / sizeof(arr[0]); i++){
 
    if(slmbx_create(arr[i], 0) == 0)
      printf("%u, is added\n", arr[i]);
    else{
      printf("%u, is not added\n", arr[i]);
    }
  }

  print();
  
  printf("Insert: 55, 67, 44\n");
  if(slmbx_create(55, 0) == 0)
      printf("%u, is added\n", 55);
  else
      printf("%u, is not added\n",55);
  if(slmbx_create(67, 0) == 0)
      printf("%u, is added\n", 67);
  else
      printf("%u, is not added\n", 67);
  if(slmbx_create(44, 0) == 0)
      printf("%u, is added\n", 44);
  else
      printf("%u, is not added\n", 44); 
  
  printf("Insert duplicates 55, 44\n");
  if(slmbx_create(55, 0) == 0)
      printf("%u, is added\n", 55);
  else
      printf("%u, is not added\n", 55);
  if(slmbx_create(44, 0) == 0)
      printf("%u, is added\n", 44);
  else
      printf("%u, is not added\n", 44);
  
  print();

  // testing slmbx_send
  printf("\n*****slmbx_send()*****\n");
  unsigned char *message[] = {"abcdefgh", "123456789", "fffddd", "Maryalnd"}; 
  
  printf("sent message abcdcfgh 123456789 fffddd maryland to id 44\n");
  int j;
  for (j = 0; j < 4; j++) 
    slmbx_send(44, message[j], 7);
   
  printf("message to other id\n");
  slmbx_send(11, message[2], 6);

  printf("invalid id\n");
  slmbx_send(99, message[1], 7);
    

  // testing slmbx_recv()
  printf("\n*****slmbx_recv()*****\n");
  unsigned char* userMessage = malloc(sizeof(char));
 
  slmbx_recv(44, userMessage, 5);
  slmbx_recv(44, userMessage, 5);
  slmbx_recv(44, userMessage, 5);
  slmbx_recv(44, userMessage, 5);
  slmbx_recv(44, userMessage, 5);
  
  
  // testing slmbx_length
  printf("\n*****slmbx_length()*****\n");
  
  unsigned char *m[] = {"CMSC441", "Project", "Computer",
			      "Maryalnd", "Washington"}; 
  int k;
  for (k = 0; k < 4; k++) 
    slmbx_send(7, m[k], 7);

  printf("get length of message in id ");
  
  printf("sent message abcdcfgh CMSC441 Project Computer\n");
  printf("Maryalnd Washington d to id 7\n");

  printf("lenght of first message is %u\n", slmbx_length(7));
  
  printf("call id 44: ");
  slmbx_length(44);

  printf("lenght of first message is %u\n", slmbx_length(11));

  printf("\n*****slmbx_count()*****\n");
  printf("number of message in id 7: %u\n", slmbx_count(7)); 

  printf("\n*****slmbx_destroy*****\n");
  print();
  printf("call id 7\n");
  slmbx_destroy(7);
  printf("call count in id 7\n");
  slmbx_count(7);
  print();
  printf("\n*****slmbx_shutdown()*****\n");
  
  
  printf("end\n");

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
    printf("Error! Wrong probability entered. Please enter 2, 4, 8 or 16.\n");
    return 1;
  }
  
  LIST  = malloc(sizeof(skiplist));
  node *header = malloc(sizeof(node));
  header->id = powerOf(2, 32) - 1;
  
  LIST->header = header;
  header->next = malloc(sizeof(node));

  int i;
  for(i = 0; i < MAX_LEVEL; i++)
    header->next[i] = LIST->header;

  LIST->level = 1;
  LIST->size = 0;
  
  return 0;
}

long slmbx_shutdown(void){
  return 0;
}


// create a new mailbox with given id
long slmbx_create(unsigned int id, int protected){
  
  if (id == 0 || id == powerOf(2, 32) - 1){
    printf("Invalid id is entered\n");
    return 1;
  }
 
  if (protected == 0){
    //    printf("everyone can access it\n");
  }
  else{
    //  printf("only owner can access it\n");
  }
  
  node *update[MAX_LEVEL + 1];
  node *x = LIST->header;

  int i, level;

  // update order of id
  // go through least from top to bottom
  for(i = LIST->level; i >= 1; i--){
    while(x->next[i]->id < id){
      x = x->next[i];
    }
    update[i] = x;
  }

  x = x->next[1];

  // check if id is already there
  if(id == x->id){
    printf("%u, duplicates id\n", id);
    return 1;
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
     x -> next = malloc(sizeof(node));
     
     for(i = 1; i <= level; i++){
       x->next[i] = update[i]->next[i];
       update[i]->next[i] = x;

     queue *msg = malloc(sizeof(queue));
     x -> msg = msg;
     x -> msg -> head = x -> msg -> tail = NULL;
    }		           
  }
  return 0;
}

long slmbx_destroy(unsigned int id){

  node *update[MAX_LEVEL + 1];
  node *x = LIST->header;
  int i;
  
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i] != NULL && x->next[i]->id < id)
      x = x->next[i];
    update[i] = x;
   }

  x = x->next[1];
  if(x != NULL &&  x->id == id){
    
     for(i = 1; i <= LIST->level; i++){
      if(update[i]->next[i] != x)
    	break;
      update[i]->next[1] = x->next[i];
     }

    
     //free_node(x);

    while(LIST->level > 1 && LIST->header->next[LIST->level] == LIST->header)
    LIST->level--;
    return 0;
  }
  else{
    printf("no id found in destroy\n");
    return -1;
  }
}

// return number of number of message in mailbox by id
long slmbx_count(unsigned int id){

  node *x = LIST->header;
  node *ptr = NULL;
  long count = 0;
 
  int i;
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }
  
  if(x->next[1]->id == id){
    if(x->next[1]->msg->head == NULL){
      return 0;
    }
    else{
      for(ptr = x->next[1]->msg->head; ptr; ptr = ptr->next)
	count = count + 1;
    }
    return count;
  }
  else{
    printf("%u id not found\n", id);
    return -1;
  }
}
  
// send a new message to the mailbox 
long slmbx_send(unsigned int id, const unsigned char *msg, unsigned int len){

  node *x = LIST->header;

  int i;
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }
  
  if(x->next[1]->id == id){

    // create a new queue pointer
    Qnode *ptr = malloc(sizeof(*ptr));

    if(ptr == NULL){
      printf("malloc() failed\n");
      return 1;
    }

    if(sizeof(msg) >= len){
      ptr->data = malloc(sizeof(Qnode));
      ptr->length = len;

      unsigned int i;
      for (i = 0; i < ptr->length; i++){
	 ptr->data[i] = msg[i];
       }
    }
    else{
      printf("length input is greater than lenght of message\n"); 
    }

      
    ptr->next = NULL;

    if(x->next[1]->msg == NULL){
      printf("Queue not initialized\n");
      free(ptr);
      return 1;
    }

    else if(x->next[1]->msg->head == NULL && x->next[1]->msg-> tail == NULL){
      x->next[1]->msg->head = x->next[1]->msg->tail = ptr;
      printf("First message added\n");
      printf("Message in Queue: \n");
      print_queue(x->next[1]->msg);
      return 0;
    }
    else if( x->next[1]->msg->head == NULL || x->next[1]->msg->tail == NULL){
      printf("Something is wrong with queue\n");
      free(ptr);
      return 1;
    }
    else{
      x->next[1]->msg->tail->next = ptr;
      x->next[1]->msg->tail = ptr;
      printf("more message added\n");
      printf("Message in Queue: \n");
      print_queue(x->next[1]->msg);
      return 0;
    }
    return 0;
  }
  else{
    printf("no id find\n");
    return 1;
  }
}

long slmbx_recv(unsigned int id, unsigned char *msg, unsigned int len){

  node *x = LIST->header;
 
  int i;
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }
  
  if(x->next[1]->id == id){

    
    // create a new queue pointer and Header
    Qnode *ptr = NULL;
    Qnode *head = NULL;

    if(x->next[1]->msg == NULL){
      printf("list is empty\n");
      return 1;
    }

    else if(x->next[1]->msg->head == NULL && x->next[1]->msg-> tail == NULL){
      printf("List is still empty\n");
      return 1;
    }
    else if( x->next[1]->msg->head == NULL || x->next[1]->msg->tail == NULL){
      printf("Something is wrong with queue\n");
      free(ptr);
      return 1;
    }
    else{

      unsigned int size = 0;
      if(x->next[1]->msg->head->length <= len){
	for (i = 0; i < x->next[1]->msg->head->length; i++){
	  msg[i] = x->next[1]->msg->head->data[i];
	}
	size = x->next[1]->msg->head->length;
      }
      else{
 	for (i = 0; i < len; i++){
	  msg[i] = x->next[1]->msg->head->data[i];
	}
	size = len;
      }
      
      head = x->next[1]->msg->head;
      ptr = head->next;
      free(head);
      x-> next[1]->msg->head = ptr;

      if(x->next[1]->msg->head == NULL)
	x->next[1]->msg->tail = x->next[1]->msg->head;

      printf("current in the queue\n");
      print_queue(x->next[1]->msg);

      printf("message copied to user space\n");
      
      unsigned q;
      printf("length: %u - message: ", size);
      for (q = 0; q < size; q++){
	printf("%c", msg[q]);
      }
      printf("\n------------------------\n");
      return 0;
    }
  }
  else{
    printf("%u id not found in recv\n", id);
  }
  return 0;
}

long slmbx_length(unsigned int id){

  node *x = LIST->header;
 
  int i;
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }
  
  if(x->next[1]->id == id){
    if(x->next[1]->msg->head == NULL){
      printf("no message in this id\n");
      return 1;
    }
    else
      return x->next[1]->msg->head->length;
  }
  else{
    printf("%u id not found\n", id);
  }
  return 0;
}

unsigned int rand_level(){
  unsigned int level = 1;
  while(rand() < RAND_MAX / powerOf(LIST_PROB, level) && level < MAX_LEVEL){
    level++;
  }
  return level;

}

void free_node(node *x){
  if(x){
    free(x->next);
    free(x);
  }
}

// x power of n
long int powerOf(unsigned int x, unsigned int n){
  int i;
  long int number = 1;
  for (i = 0; i < n; ++i)
    number *= x;

  return number;
 
}

unsigned int find(unsigned int id){
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

void print(){
  node *x = LIST->header;
  printf("head-> ");
  while(x && x->next[1] != LIST->header){
    printf("%u -> ", x->next[1]->id);
    x = x->next[1];
  }
  printf("TAIL\n");

}

/*************************************************
 ********** this section is for queue ************
 *************************************************/

void print_queue(queue *list){
  Qnode *ptr = NULL;

  if(list){
    for(ptr = list->head; ptr; ptr = ptr -> next){
      print_message(ptr);
    }
   printf("----------------------\n");
  }
}

void print_message(Qnode *ptr){
  unsigned int len = sizeof(ptr);
  if(ptr){
    int i;
    printf("Message is: ");

    for(i = 0; i < len; i++){
        printf("%c", ptr->data[i]);
    }
	printf("\n");
  }
  else
    printf("No message in this mailbox\n");
}

/*
unsigned char* copyMessage(unsigned *char a,
			    unsigned * int l){
  unsigned char* b = malloc(sizeof(char));

  unsigned int i;
  for (i = 0; i < l; i++){
    b[i] = a[i];
  }
  
  return b; 
}
*/

 
/*
unsigned int get_length(unsigned char * buffer){

    int i, c;
    i = 0;
    do{
        c = fgetc(stdin);
        if(c == '\n' || c == '\0')
            return i;
        else {                                                  
            buffer[i] = c;
            i++;
	}

    }while(1);
    
    return i;                                         
}
*/
