/** Name: Zhaojie Jiang
 ** Class: CMSC 421 
 ** Assignment: Project 1
 **  
 ** File: mailbox_system.c
 **/

#include<linux/string.h>
#include<linux/errno.h>
#include<linux/kernel.h>
#include<linux/unistd.h>
#include<linux/slab.h>
#include<linux/random.h>
#include<linux/types.h>
#include<linux/uidgid.h>
#include<linux/mutex.h>
#include<linux/cred.h>
#include<linux/printk.h>

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

asmlinkage long slmbx_init(unsigned int ptrs, unsigned int prob);

asmlinkage long slmbx_shutdown(void);

asmlinkage long slmbx_create(unsigned int id, int protected);

asmlinkage long slmbx_destroy(unsigned int id);

asmlinkage long slmbx_count(unsigned int id);

asmlinkage long slmbx_send(unsigned int id, const unsigned char *msg, unsigned int len);

asmlinkage long slmbx_recv(unsigned int id, unsigned char *msg, unsigned int len);

asmlinkage long slmbx_length(unsigned int id);

// get random number
unsigned int rand_level(void);

void free_node(node *x);
  
// x power of n
long int powerOf(unsigned int x, unsigned int n);

//uid_t geteuid(void);
uid_t getuid(void);

// Global Variable
skiplist *LIST; // skip list
unsigned int MAX_LEVEL; // max level of the skip list
unsigned int LIST_PROB; // probability that a node will be promoted
                                //to having additional pointer in skip_list
/* initializes the mailbox system, setting up the initial 
   state of the skip list.
   ptrs parameter is the maximum of pointers any node in the list 
   will be allowed to have.
   prob parameter inverse of the probability that a node in the list
   will be promoted to have an additional pointer in it
 */
asmlinkage long slmbx_init(unsigned int ptrs, unsigned int prob){

  // declarate value
  node *header;
  unsigned int i;
 
  // code starting

 
  if(current_uid().val != 0)
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
  
  LIST = kmalloc(sizeof(skiplist), GFP_KERNEL);
  
  header = kmalloc(sizeof(node), GFP_KERNEL);
  header->id = powerOf(2, 32) - 1;
  
  LIST->header = header;
  header->next = kmalloc(sizeof(node*), GFP_KERNEL);
  
  for(i = 0; i < MAX_LEVEL; i++){
    header->next[i] = LIST->header;
  }
  
  LIST->level = 1;
  LIST->size = 0;
  
  return 0;
}

asmlinkage long slmbx_shutdown(void){

  //unsigned int i;
  
  if(current_uid().val !=0)
    return -EPERM;

  /*
  for(i = 0; i <= LIST->level; i++){
    node *x = LIST->header->next[i];
    slmbx_destroy(x->id);
  }
  */
  kfree(LIST);
  
  return 0;
}


// create a new mailbox with given id
asmlinkage long slmbx_create(unsigned int id, int protected){

  node *update[MAX_LEVEL + 1];
  node *x;
  int i, level;
  queue *msg;
  
  if (id == 0 || id == powerOf(2, 32) - 1)
    return -EINVAL;

  if(LIST == NULL)
    return -ENODEV;

  printk("a\n");
  x = LIST->header;
  printk("b\n");
  // update order of id
  // go through least from top to bottom
  for(i = LIST->level; i >= 1; i--){
    printk("%u in for\n", i);
    while(x->next[i]->id < id){
      x = x->next[i];
      printk("%u in while\n", i);
    }
    
    update[i] = x;
  }

   printk("c\n");
  x = x->next[1];
  // check if id is already there
  if(id == x->id){
    return -EEXIST;
  }
  // check if need to add new pointer
  else{
    printk("d\n");
    level = rand_level();
    if(level > LIST->level){
      for(i = LIST->level + 1; i<= level; i++){
	update[i] = LIST->header;
      }
      LIST->level = level;
    }
     printk("e\n");
     x = kmalloc(sizeof(node), GFP_KERNEL);
     x -> id = id; 
     x -> protected = protected;
     x -> userID = current_uid().val;
     x -> next = kmalloc(sizeof(node), GFP_KERNEL);
      printk("f\n");
     for(i = 1; i <= level; i++){
       x->next[i] = update[i]->next[i];
       update[i]->next[i] = x;
       msg = kmalloc(sizeof(queue), GFP_KERNEL);
       x -> msg = msg;
       x -> msg -> head = NULL;
       x -> msg -> tail = NULL;
       //kfree(msg);
    }
  }
  printk("end\n");
  return 0;
}

asmlinkage long slmbx_destroy(unsigned int id){

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

   
  if( x -> protected != 0){
    if( x -> userID != current_uid().val)
	return -EPERM;
    }
    

    for(i = 1; i <= LIST->level; i++){
      if(update[i]->next[i] != x)
    	break;
      update[i]->next[i] = x->next[i];
     }

    /*
    while(x){
      unsigned char * message = malloc(sizeof(char));
      slmbx_recv(id, message, 0);
      free(message);
    }
    */
    free_node(x);

    while(LIST->level > 1 && LIST->header->next[LIST->level] == LIST->header)
    LIST->level--;
    return 0;
  }
  else{
    return -ENOENT;
  }
}

// return number of number of message in mailbox by id
asmlinkage long slmbx_count(unsigned int id){

  node *x;
  Qnode *ptr;
  long count = 0;
  int i;

  x = LIST->header;
  ptr = NULL;
  
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }

  x = x->next[1];

  if(x->id == id){

    
    if( x -> protected != 0){
      if( x -> userID != current_uid().val)
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
asmlinkage long slmbx_send(unsigned int id, const unsigned char *msg, unsigned int len){

  node *x;
  unsigned int i;
  Qnode *ptr;
  
  x = LIST->header;
  
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }
  x = x->next[1];
  if(x->id == id){

    
     if( x -> protected != 0){
      if( x -> userID != current_uid().val)
	return -EPERM;
    }
   
     
    // create a new queue pointer
     ptr = kmalloc(sizeof(Qnode), GFP_KERNEL);

    if(ptr == NULL){
      return -ENOMEM;
    }

    if(sizeof(msg) >= len){
      ptr->data = kmalloc(sizeof(Qnode), GFP_KERNEL);
      ptr->length = len;

      for (i = 0; i < ptr->length; i++){
	 ptr->data[i] = msg[i];
       }
    }
    else{
      return -EINVAL;
    }

      
    ptr->next = NULL;

    if(x->msg == NULL){
      kfree(ptr);
      return -ENOENT;
    }

    else if(x->msg->head == NULL && x->msg-> tail == NULL){
      x->msg->head = x->msg->tail = ptr;
      return 0;
    }
    else if( x->msg->head == NULL || x->msg->tail == NULL){
      kfree(ptr);
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

asmlinkage long slmbx_recv(unsigned int id, unsigned char *msg, unsigned int len){

  node *x;
  int i;
  Qnode* ptr;
  Qnode *head;
  
  x = LIST->header;
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }

  x = x->next[1];
  if(x->id == id){

    
    if( x -> protected != 0){
      if( x -> userID != current_uid().val)
	return -EPERM;
    }
    
    
    // create a new queue pointer and Header
    ptr = NULL;
    head = NULL;
    
    if(x->msg == NULL){
      return -ENOENT;
    }
    
    else if(x->msg->head == NULL && x->msg-> tail == NULL){
      return -ESRCH;
    }
    else if( x->msg->head == NULL || x->msg->tail == NULL){
      kfree(ptr);
      return -ENOMEM;
    }
    else{
  
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
      head->length = 0;
      kfree(head);
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

asmlinkage long slmbx_length(unsigned int id){

  node *x;
  int i;

  x = LIST->header;
  
  for(i = LIST->level; i >= 1; i--){
    while (x->next[i]->id < id)
      x = x->next[i];
  }

  x = x->next[1];
  if(x->id == id){

    
    if( x -> protected != 0){
      if( x -> userID != current_uid().val)
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
  unsigned int level;
  unsigned int i;

  level = 1;
  
  get_random_bytes(&i, sizeof(unsigned int));
  i = i%10000;
  while(i  < 10000 / powerOf(LIST_PROB, level) && level < MAX_LEVEL){
    level++;
  }
  return level;

}

void free_node(node *x){
  if(x){
    kfree(x->next);
    kfree(x);
  }
}

// x power of n
long int powerOf(unsigned int x, unsigned int n){
  unsigned int i;
  long int number;

  number = 1;

  for (i = 0; i < n; ++i)
    number = number * x;

  return number;
 
}
