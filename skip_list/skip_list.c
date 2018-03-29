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
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>

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
  unsigned int protected;
  unsigned int userID;
  
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
unsigned int rand_level(void);

void free_node(node *x);
  
// x power of n
long int powerOf(unsigned int x, unsigned int n);

/* find id 
   if find return 1 else return 0;
 */
unsigned int find(unsigned int id);

// print list
void print(void);

uid_t getuid(void);

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

  long rv;
  
  // test slmb_init()
  printf("\n*****slmbx_init()*****\n");
  
  printf("\npass ptr = 0\n");
  printf("%s \n",strerror(-slmbx_init(0, 4)));
  
  printf("\nenter prob other than 2, 4, 8, 16 - 3 times\n");
  printf("%s \n",strerror(-slmbx_init(6, 5)));
  printf("%s \n",strerror(-slmbx_init(4, 8)));
  printf("%s \n",strerror(-slmbx_init(3, 0)));

  printf("\nenter prob 2, 4, 8 or 16.\n");
  rv = slmbx_init(4, 2);
  if(rv == 0)
    printf("slmbx_init success\n");
  else
    printf("%s \n",strerror(-rv));

  
  //test slmb_create()
  printf("\n*****slmbx_create*****\n");
  printf("\nInsert:----------\n");

  unsigned int arr[] = {3, 6, 9, 11, 4, 1, 7};
  unsigned int i;
  
  for (i = 0; i < 7; i++){
    rv = slmbx_create(arr[i], 0);
    if(rv == 0)
      printf("%u is added\n", arr[i]);
    else{
      printf("%s \n",strerror(-rv));
    }
  }
  
  printf("\n");
  // print();
  
  printf("\nInsert: 55, 67, 44\n");

  rv = slmbx_create(55, 0);
  
  if(rv == 0)
    printf("%u is added\n", 55);
  else{
    printf("%s \n",strerror(-rv));
  }

  rv = slmbx_create(67, 0);
  if(rv == 0)
    printf("%u is added\n", 67);
  else{
    printf("%s \n",strerror(-rv));
  }

  rv = slmbx_create(44, 0);
  if(rv == 0)
    printf("%u is added\n", 44);
  else{
    printf("%s \n",strerror(-rv));
  }

  printf("\nInsert duplicates 55, 44\n");

  rv = slmbx_create(55, 0);

  if(rv == 0)
    printf("%u is added\n", 55);
  else{
    printf("%s \n",strerror(-rv));
  }
  
  rv = slmbx_create(44, 0);
  if(rv == 0)
    printf("%u is added\n", 44);
  else{
    printf("%s \n",strerror(-rv));
  }

  printf("\n");
  //print();
  
  // testing slmbx_send
  printf("\n*****slmbx_send()*****\n");
  unsigned char *message[] = {"abcdefgh", "123456789", "fffddd", "Maryalnd"}; 
  
  printf("sent message abcdcfgh 123456789 fffddd maryland to id 44\n");
  int j;
  for (j = 0; j < 4; j++){ 
    rv = slmbx_send(44, message[j], 7);
    if(rv == 0)
      printf("message send success\n");
    else
      printf("%s\n", strerror(-rv));
  }
  
  printf("\nmessage to other id\n");
  rv = slmbx_send(11, message[2], 6);
  if(rv == 0)
      printf("message send success\n");
  else
      printf("%s\n", strerror(-rv));

  printf("\nsent message to invalid id\n");
  rv = slmbx_send(99, message[1], 7);
  if(rv == 0)
      printf("message send success\n");
   else
      printf("%s\n", strerror(-rv));

  printf("\nsent 0 length message to id 3\n");
  rv = slmbx_send(3, "", 1);
  if(rv == 0)
      printf("message send success\n");
   else
      printf("%s\n", strerror(-rv));

  //print();
  
  // testing slmbx_recv()
  printf("\n*****slmbx_recv()*****\n");
  unsigned char* userMessage;
  userMessage = (unsigned char*)malloc(sizeof(unsigned char*));
 
  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0){
    printf("message received success\n");
    unsigned int i;
    for(i = 0; i < 5; i++){
	printf("%c", userMessage[i]);
    }
    printf("\n");
  }
   else
      printf("%s\n", strerror(-rv));
  
  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("message received success\n");
   else
      printf("%s\n", strerror(-rv));

  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("message received success\n");
   else
      printf("%s\n", strerror(-rv));

  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("message received success\n");
   else
      printf("%s\n", strerror(-rv));

  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("message received success\n");
   else
      printf("%s\n", strerror(-rv));

  //print();
  
  // testing slmbx_length
  printf("\n*****slmbx_length()*****\n");
  
  unsigned char *m[] = {"CMSC441", "Project", "Computer",
			      "Maryalnd", "Washington"}; 
  
  printf("get length of message in id\n");
  
  printf("sent message CMSC441 Project Computer\n");
  printf("Maryalnd Washington d to id 7\n");

  int k;
  for (k = 0; k < 5; k++) 
    slmbx_send(7, m[k], 7);

  // print();

  rv = slmbx_length(7);
  if(rv < 10)
    printf("\nlenght of first message in 7 is %u\n", rv);
  else
     printf("%s\n", strerror(-rv));

  printf("\ncall id 44 where 44 is not there:\n");
  rv = slmbx_length(44);
  printf("%s\n", strerror(-rv));

  rv = slmbx_length(11);
  printf("lenght of first message is %u\n", rv);

  rv =slmbx_length(1);
  printf("Error: %s\n", strerror(-rv));

  
  printf("**************************");
  printf("\n*****slmbx_count()*****\n");
  printf("*************************\n");
  
  rv = slmbx_count(7);
  printf("number of message in id 7: %u\n", rv);

  printf("id 44 where no message \n");
  rv = slmbx_count(44);
  printf("Error: %s\n", strerror(-rv));

  printf("**************************");
  printf("\n*****slmbx_destroy*****\n");
  printf("*************************\n");
  
  print();
  printf("call id 7\n");
  printf("%s", strerror(-slmbx_destroy(7)));
  
  
  printf("call count in id 7\n");
  printf("%s\n", strerror(-slmbx_count(7)));
  printf("%s\n", strerror(-slmbx_length(7)));
  print();
  

  //  printf("%s\n", strerror(-slmbx_create(11, 0)));
  // printf("%s\n", strerror(-slmbx_send(11, "asdsdfsdff", 5))); 
  // printf("%s\n", strerror(slmbx_destroy(11)));
  printf("\n*****slmbx_shutdown()*****\n");
  printf("%s", strerror(-slmbx_shutdown()));
  print();
  
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

  if(getuid() !=0)
    return -EPERM;
  
  if( ptrs == 0)
    return -EINVAL;

  MAX_LEVEL = ptrs;

  // create probability for additional pointer.
  if(prob == 2 || prob == 4 || prob == 8 || prob == 16)
    LIST_PROB = prob;
  else{
    return -EINVAL;
  }
  
  LIST  = (skiplist*)malloc(sizeof(skiplist));
  node *header = (node*)malloc(sizeof(node));
  header->id = powerOf(2, 32) - 1;
  
  LIST->header = header;
  header->next = (node**) malloc(sizeof(node*) * (MAX_LEVEL + 1));

  int i;
  for(i = 0; i < MAX_LEVEL; i++){
    header->next[i] = LIST->header;
  }

  LIST->level = 1;
  LIST->size = 0;
  
  return 0;
}

long slmbx_shutdown(void){

 
    node *current_node = LIST->header->next[1];
    while(current_node != LIST->header){
      node *next_node = current_node->next[1];
      free(current_node->next);
      free(current_node);
      current_node = next_node;
    }
    free(current_node->next);
    free(current_node);

    //  free(LIST->header->next);
    //free(LIST->header);
  free(LIST);
  
  return 0;
}


// create a new mailbox with given id
long slmbx_create(unsigned int id, int protected){
  
  if (id == 0 || id == powerOf(2, 32) - 1)
    return EINVAL;

  if(LIST == NULL)
    return ENODEV;
  
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
    return -EEXIST;
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
    

    x = (node*)malloc(sizeof(node));
    x -> id = id; 
    x -> protected = protected;
     x -> userID = getuid();
     x -> next = (node**)malloc(sizeof(node*)*(level + 1));
     
     for(i = 1; i <= level; i++){
       x->next[i] = update[i]->next[i];
       update[i]->next[i] = x;
     }
     queue *msg = (queue*)malloc(sizeof(queue));
     x -> msg = msg;
     x -> msg -> head = x -> msg -> tail = NULL;
    		           
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
  
  if(x->next[1] != NULL &&  x->next[1]->id == id){

    if( x->next[1] -> protected != 0){
      if( x->next[1] -> userID != getuid())
	return -EPERM;
    }
    
    for(i = 1; i <= LIST->level; i++){
      if(update[i]->next[i] != x->next[1])
    	break;
      update[i]->next[i] = x->next[1]->next[i];
     }

  
    // free(x->next[1]);

    // unsigned i;
    //for(i = MAX_LEVEL; i <= 0; i--){
    //    node *current_node = x->next[1];
    //while(current_node != x){
    /*

      queue* q = x->next[1]->msg;
      Qnode* temp;
    
      while(q->head != NULL){
	temp = q->head;
	q->head = temp->next;
	free(temp->data);
	free(temp);
      }
    */
      /*
      node *next_node = current_node->next[1];
      free(current_node->next);
      free(current_node);
      current_node = next_node;
    }
    free(current_node->next);
    free(current_node);    
      */
     free(x);
    
    while(LIST->level > 1 && LIST->header->next[LIST->level] == LIST->header)
    LIST->level--;
    return 0;
  }
  else{
    return -ENOENT;
  }
}

// return number of number of message in mailbox by id
long slmbx_count(unsigned int id){

  node *x = LIST->header;
  Qnode *ptr = NULL;
  long count = 0;
 
  int i;
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }

  x = x->next[1];

  if(x->id == id){

    if( x -> protected != 0){
      if( x -> userID != getuid())
	return -EPERM;
    }
 
    if(x->msg->head == NULL){
      return -ESRCH;
    }
    else{
      for(ptr = x->msg->head; ptr; ptr = ptr->next)
	count = count + 1;
    }
    return count;
  }
  else{
    return -ENOENT;
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
  x = x->next[1];
  if(x->id == id){

     if( x -> protected != 0){
      if( x -> userID != getuid())
	return -EPERM;
    }
     
    // create a new queue pointer
     Qnode *ptr = (Qnode*) malloc(sizeof(Qnode));

    if(ptr == NULL){
      return -ENOMEM;
    }

    if(sizeof(msg) >= len){
      ptr->data = (unsigned char*)malloc(sizeof(unsigned char*));
      ptr->length = len;

      unsigned int i;
      for (i = 0; i < ptr->length; i++){
	 ptr->data[i] = msg[i];
       }
    }
    else{
      return -EINVAL;
    }

      
    ptr->next = NULL;

    if(x->msg == NULL){
      free(ptr);
      return -ENOENT;
    }

    else if(x->msg->head == NULL && x->msg-> tail == NULL){
      x->msg->head = x->msg->tail = ptr;
      return 0;
    }
    else if( x->msg->head == NULL || x->msg->tail == NULL){
      free(ptr);
      return -ENOENT;
    }
    else{
      x->msg->tail->next = ptr;
      x->msg->tail = ptr;
      return 0;
    }
  }
  else{
    return -ENOENT;
  }
}

long slmbx_recv(unsigned int id, unsigned char *msg, unsigned int len){

  node *x = LIST->header;
 
  int i;
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }

  x = x->next[1];
  if(x->id == id){

    if( x -> protected != 0){
      if( x -> userID != getuid())
	return -EPERM;
    }
    
    // create a new queue pointer and Header
    Qnode *ptr = NULL;
    Qnode *head = NULL;
    
    if(x->msg == NULL){
      return -ENOENT;
    }
    
    else if(x->msg->head == NULL && x->msg-> tail == NULL){
      return -ESRCH;
    }
    else if( x->msg->head == NULL || x->msg->tail == NULL){
      free(ptr);
      return -ENOMEM;
    }
    else{
      
      // msg = (unsigned char*)malloc(sizeof(unsigned char));
      
      // copy the message to the msg
      if(x->msg->head->length <= len){
	for (i = 0; i < x->msg->head->length; i++){
	  msg[i] = x->msg->head->data[i];
	}
      }
      else{
 	for (i = 0; i < len; i++){
	  msg[i] = x->msg->head->data[i];
	}
      }
      
      head = x->msg->head;
      ptr = head->next;
      head->data = NULL;
      free(head->data);
      head->length = 0;
      free(head);
      x->msg->head = ptr;

      if(x->msg->head == NULL)
	x->msg->tail = x->msg->head;
      return 0;
    }
  }
  else{
    return -ENOENT;
  }
}

long slmbx_length(unsigned int id){

  node *x = LIST->header;
 
  int i;
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }

  x = x->next[1];
  if(x->id == id){
    
    if( x -> protected != 0){
      if( x -> userID != getuid())
	return -EPERM;
    }
     
    if(x->msg == NULL){
      return -ENOENT;
    }
    if(x->msg->head == NULL){
      return -ESRCH;
    }
    else
      return x->msg->head->length;
  }
  else{
    return -ENOENT;
  }
}

unsigned int rand_level(void){
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
  unsigned int i;

  // going for top to down
  for(i = LIST->level; i >=1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }
  if(x->next[1]->id == id)
    return 0;
  else
    return -ENOENT;  
}
    
/*************************************************
 ***** This section function is for testing ******
 *************************************************/

void print(void){

  if(LIST && LIST->header){

    printf("Skip List: \n");
    unsigned int i = 0;
  
  // for(int i = 0; i <= LIST->level; i++){
  //  node *x = LIST->header->next[i];
  node *x = LIST->header;
    printf("level %u: \n", i);
    while(x && x->next[1] != LIST->header){
    printf("%u ", x->next[1]->id);
    x = x->next[1];
    printf("->");
    print_queue(x->msg);
    //    printf("\n");
    }
    // printf("\n");
    //    }
  }
  else{
    printf("no mailbox system\n");
  }
}

void print_queue(queue *list){
  Qnode *ptr = NULL;

  if(list->head){
    for(ptr = list->head; ptr; ptr = ptr -> next){
      print_message(ptr);
    }
     printf("end of message\n");
  }
  else{
    printf("no message in mailbox\n");
  }
}

void print_message(Qnode *ptr){
  unsigned int len = sizeof(ptr);
  if(ptr){
    int i;

    for(i = 0; i < len; i++){
        printf("%c", ptr->data[i]);
    }
    printf(" - ");
  }
}
