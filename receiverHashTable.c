#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/sender.h"
#include "./headers/receiver.h"
#include "./headers/functions.h"

void insertInReceiverHashTable(struct hashNode* hashTable, char* dat, char* tranTim, int tranId, char* senderId, char* receiverId, int value, int numOfEntries, int bucketSize){
  struct transactionNode* receiver;
  struct bucket* bucket;
  char* temp = receiverId;
  unsigned long key = 5381;
  int c;
  char* date = malloc(strlen(dat)+1);
  char* tranTime = malloc(strlen(tranTim)+1);
  strcpy(date,dat);
  strcpy(tranTime,tranTim);

  while (c = *temp++){
    key = ((key << 5) + key) + c;
  }
  key = key % numOfEntries;
  //if there are no buckets at the current position
  if(hashTable[key].bucket == NULL){
    bucket = malloc(bucketSize*sizeof(struct bucket));
    for(int i=0; i<bucketSize; i++){
      bucket[i].id = "";
      bucket[i].transaction = NULL;
    }
    //point to the first node
    bucket[0].id = receiverId;
    bucket[0].next = NULL;
    receiver = malloc(sizeof(struct transactionNode));
    receiver->id = senderId;
    receiver->tranId = tranId;
    receiver->date = date;
    receiver->time = tranTime;
    receiver->value = value;
    receiver->next = NULL;
    bucket[0].transaction = receiver;
    hashTable[key].bucket = bucket;
  }
  // if there is at least one bucket
  else{
    int inserted = 0;
    struct bucket* head = hashTable[key].bucket;
    //check if there is this id in the existing buckets
    while(head != NULL){
      //check each bucket
      for(int i=0; i<bucketSize; i++){
        //if we find an empty space inside a bucket
        if(head[i].id == '\0'){
          head[i].id = receiverId;
          head[i].next = NULL;
          receiver = malloc(sizeof(struct transactionNode));
          receiver->id = senderId;
          receiver->tranId = tranId;
          receiver->date = date;
          receiver->time = tranTime;
          receiver->value = value;
          receiver->next = NULL;
          head[i].transaction = receiver;
          inserted = 1;
          break;
        }
        //if we find the id we want
        else if(strcmp(head[i].id,receiverId) == 0){
          //find the last struct of sender
          struct transactionNode* cur = head[i].transaction;
          while(cur->next != NULL){
            cur = cur->next;
          }
          receiver = malloc(sizeof(struct transactionNode));
          receiver->id = senderId;
          receiver->tranId = tranId;
          receiver->date = date;
          receiver->time = tranTime;
          receiver->value = value;
          receiver->next = NULL;
          cur->next = receiver;
          inserted = 1;
          break;
        }
      }
      if(inserted == 1){
        break;
      }
      //it means we looped through all the current buckets but we didnt find the Id
      //so we create a new bucket at the current position of the hash table
      if(head->next == NULL){
        bucket = malloc(bucketSize*sizeof(struct bucket));
        for(int i=0; i<bucketSize; i++){
          bucket[i].id = "";
          bucket[i].transaction = NULL;
        }
        bucket[0].id = receiverId;
        bucket[0].next = NULL;

        receiver = malloc(sizeof(struct transactionNode));
        receiver->id = senderId;
        receiver->tranId = tranId;
        receiver->date = date;
        receiver->time = tranTime;
        receiver->value = value;
        receiver->next = NULL;
        bucket[0].transaction = receiver;
        //the last node we found will point to the new one
        head->next = bucket;
        break;
      }
      //move to the next bucket
      head = head->next;
    }
  }
}
