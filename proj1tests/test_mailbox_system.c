/** Name: Zhaojie Jiang
 ** Class: CMSC 421 
 ** Assignment: Project 1
 **  
 ** File: test_mailbox_system.c
 **/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<linux/kernel.h>
#include<sys/syscall.h>

#define _init 333
#define _shutdown 334
#define _create 335
#define _destroy 336
#define _count 337
#define _send 338
#define _recv 339
#define _length 340

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

long syscall();

long slmbx_init(unsigned int ptrs, unsigned int prob){
  return syscall(_init, ptrs, prob);
}

long slmbx_shutdown(void){
  return syscall(_shutdown);
}

long slmbx_create(unsigned int id, int protected){
  return syscall(_create, id, protected);
}

long slmbx_destroy(unsigned int id){
  return syscall(_destroy, id);
}

long slmbx_count(unsigned int id){
  return syscall(_count, id);
}

long slmbx_send(unsigned int id, const unsigned char *msg, unsigned int len){
  return syscall(_send, id, msg, len);
}

long slmbx_recv(unsigned int id, unsigned char *msg, unsigned int len){
  return syscall(_recv, id, msg, len);
}

long slmbx_length(unsigned int id){
  return syscall(_length, id);
}

// print list
//void print();

/******************************************
 ******* this section is for queue ********
 ******************************************/
//void print_queue(queue *list);

//void print_message(Qnode *ptr);

//unsigned int get_length(unsigned char * buffer);

/******************************************
 **** main and Global varaible started ****
 ******************************************/

// Global Variable
//skiplist *LIST; // skip list
//unsigned int MAX_LEVEL; // max level of the skip list
//unsigned int LIST_PROB; // probability that a node will be promoted
                                //to having additional pointer in skiplist.

int main(int argc, char *argv[]){

  long rv;
  
  // test slmb_init()
  printf("\n*****slmbx_init()*****\n");
  
  printf("\npass ptr = 0\n");
  printf("%s \n",strerror(-slmbx_init(0, 4)));

  
  printf("\nenter prob other than 2, 4, 8, 16 - 3 times\n");
  printf("%s \n",strerror(-slmbx_init(5, 5)));
  printf("%s \n",strerror(-slmbx_init(8, 7)));
  printf("%s \n",strerror(-slmbx_init(10, 9)));
  
  printf("\nenter prob 2, 4, 8 or 16.\n");
  
  printf("%s \n",strerror(-slmbx_init(10, 2)));
  
  //test slmb_create()
  printf("\n*****slmbx_create*****\n");
  printf("\nInsert: 3, 6, 9, 11, 4, 1, 7\n");

  unsigned int arr[] = {3, 6, 9, 11, 4, 1, 7};
  unsigned int i;
  
  for (i = 0; i < sizeof(arr) / sizeof(arr[0]); i++){
    rv = slmbx_create(arr[i], 0);
    if(rv == 0)
      printf("%u is added\n", arr[i]);
    else{
      printf("%s \n",strerror(-rv));
    }
  }
  
  // printf("\n");
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
  // print();
  /*
  
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
      printf("%s\n", strerror(rv));
  }
  
  printf("\nmessage to other id\n");
  rv = slmbx_send(11, message[2], 6);
  if(rv == 0)
      printf("message send success\n");
  else
      printf("%s\n", strerror(rv));

  printf("\nsent message to invalid id\n");
  rv = slmbx_send(99, message[1], 7);
  if(rv == 0)
      printf("message send success\n");
   else
      printf("%s\n", strerror(rv));

  printf("\nsent 0 length message to id 3\n");
  rv = slmbx_send(3, "", 1);
  if(rv == 0)
      printf("message send success\n");
   else
      printf("%s\n", strerror(rv));

  print();

  // testing slmbx_recv()
  printf("\n*****slmbx_recv()*****\n");
  unsigned char* userMessage = malloc(sizeof(char));
 
  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("%s message received success\n", userMessage);
   else
      printf("%s\n", strerror(rv));

  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("message received success\n");
   else
      printf("%s\n", strerror(rv));

  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("message received success\n");
   else
      printf("%s\n", strerror(rv));

  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("message received success\n");
   else
      printf("%s\n", strerror(rv));

  rv = slmbx_recv(44, userMessage, 5);
  if(rv == 0)
      printf("message received success\n");
   else
      printf("%s\n", strerror(rv));

  print();
  
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

  print();

  rv = slmbx_length(7);
  if(rv < 10)
    printf("\nlenght of first message in 7 is %u\n", rv);
  else
     printf("%s\n", strerror(rv));

  printf("\ncall id 44 where 44 is not there:\n");
  rv = slmbx_length(44);
  printf("%s\n", strerror(rv));

  rv = slmbx_length(11);
  printf("lenght of first message is %u\n", rv);

  rv =slmbx_length(1);
  printf("Error: %s\n", strerror(rv));

  printf("**************************");
  printf("\n*****slmbx_count()*****\n");
  printf("*************************\n");
  
  rv = slmbx_count(7);
  printf("number of message in id 7: %u\n", rv);

  printf("id 44 where no message \n");
  rv = slmbx_count(44);
  printf("Error: %s\n", strerror(rv));

  printf("**************************");
  printf("\n*****slmbx_destroy*****\n");
  printf("*************************\n");
  
  print();
  printf("call id 7\n");
  printf("%s", strerror(slmbx_destroy(7)));
  
  printf("call count in id 7\n");
  printf("%s\n", strerror(slmbx_count(7)));
  printf("%s\n", strerror(slmbx_length(7)));
  print();

  printf("\n*****slmbx_shutdown()*****\n");
  printf("%s", strerror(slmbx_shutdown()));
  //  print();
  
  printf("end\n");
  */

  return 0;
}   
/*************************************************
 ***** This section function is for testing ******
 *************************************************/
/*
void print(){

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
*/
