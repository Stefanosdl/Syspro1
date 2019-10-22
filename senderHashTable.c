#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/sender.h"
#include "./headers/tree.h"
#include "./headers/wallets.h"
#include "./headers/functions.h"


void insertInSenderHashTable(struct hashNode* hashTable, struct wallet* userWallet, int senderPos, int walletSize, char* dat, char* tranTim, int tranId, char* senderId, char* receiverId, int value, int numOfEntries, int bucketSize) {
  struct transactionNode* sender;
  struct treeNode* root;
  struct bitcoinsList* bitcoins;
  struct bucket* bucket;
  char* temp = senderId;
  unsigned long key = 5381;
  int c,currentBitcoin;
  int amount = value;
  int currentAmount = 0;
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
    bucket[0].id = senderId;
    bucket[0].next = NULL;
    sender = malloc(sizeof(struct transactionNode));
    sender->id = receiverId;
    sender->tranId = tranId;
    sender->date = date;
    sender->time = tranTime;
    sender->value = value;
    sender->next = NULL;

    //check if 1 bitcoin holds all the amount
    bitcoins = userWallet[senderPos].bitcoin;
    while(bitcoins != NULL){
      // checkAmountOfBitcoin(bitcoins->treeHead,senderId,&currentAmount);
      currentBitcoin = amountOfBitcoin(bitcoins,value);

      //it means we found it
      if(currentBitcoin > 0){
        root = getBitCoinTree(userWallet, walletSize, currentBitcoin);
        break;
      }
      bitcoins = bitcoins->next;
    }
    //we find 1 bitcoin that holds the value
    if(currentBitcoin > 0){
      while(amount > 0){
        traverseTree(root,sender,senderId,receiverId,&amount);
      }
    }
    //get the bitcoins, 1 at a time and get the value we want from each one
    else{
      bitcoins = userWallet[senderPos].bitcoin;
      amount = value;
      while(amount > 0 && bitcoins != NULL){
          root = getBitCoinTree(userWallet, walletSize, bitcoins->bitcoinId);
          traverseTree(root,sender,senderId,receiverId,&amount);
          bitcoins = bitcoins->next;
        }
    }


    bucket[0].transaction = sender;
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
          head[i].id = senderId;
          head[i].next = NULL;
          sender = malloc(sizeof(struct transactionNode));
          sender->id = receiverId;
          sender->tranId = tranId;
          sender->date = date;
          sender->time = tranTime;
          sender->value = value;
          sender->next = NULL;


          //check if 1 bitcoin holds all the amount
          bitcoins = userWallet[senderPos].bitcoin;
          while(bitcoins != NULL){
            // checkAmountOfBitcoin(bitcoins->treeHead,senderId,&currentAmount);
            currentBitcoin = amountOfBitcoin(bitcoins,value);

            //it means we found it
            if(currentBitcoin > 0){
              root = getBitCoinTree(userWallet, walletSize, currentBitcoin);
              break;
            }
            bitcoins = bitcoins->next;
          }
          //we find 1 bitcoin that holds the value
          if(currentBitcoin > 0){
            while(amount > 0){
              traverseTree(root,sender,senderId,receiverId,&amount);
            }
          }
          //get the bitcoins, 1 at a time and get the value we want from each one
          else{
            bitcoins = userWallet[senderPos].bitcoin;
            amount = value;
            while(amount > 0 && bitcoins != NULL){
                root = getBitCoinTree(userWallet, walletSize, bitcoins->bitcoinId);
                traverseTree(root,sender,senderId,receiverId,&amount);
                bitcoins = bitcoins->next;
              }
          }

          head[i].transaction = sender;
          inserted = 1;
          break;
        }
        //if we find the id we want
        else if(strcmp(head[i].id,senderId) == 0){
          //find the last struct of sender
          struct transactionNode* cur = head[i].transaction;
          while(cur->next != NULL){
            cur = cur->next;
          }
          sender = malloc(sizeof(struct transactionNode));
          sender->id = receiverId;
          sender->tranId = tranId;
          sender->date = date;
          sender->time = tranTime;
          sender->value = value;
          sender->next = NULL;


          //check if 1 bitcoin holds all the amount
          bitcoins = userWallet[senderPos].bitcoin;
          while(bitcoins != NULL){
            // checkAmountOfBitcoin(bitcoins->treeHead,senderId,&currentAmount);
            currentBitcoin = amountOfBitcoin(bitcoins,value);

            //it means we found it
            if(currentBitcoin > 0){
              root = getBitCoinTree(userWallet, walletSize, currentBitcoin);
              break;
            }
            bitcoins = bitcoins->next;
          }
          //we find 1 bitcoin that holds the value
          if(currentBitcoin > 0){
            while(amount > 0){
              traverseTree(root,sender,senderId,receiverId,&amount);
            }
          }
          //get the bitcoins, 1 at a time and get the value we want from each one
          else{
            bitcoins = userWallet[senderPos].bitcoin;
            amount = value;
            while(amount > 0 && bitcoins != NULL){
                root = getBitCoinTree(userWallet, walletSize, bitcoins->bitcoinId);
                traverseTree(root,sender,senderId,receiverId,&amount);
                bitcoins = bitcoins->next;
              }
          }

          cur->next = sender;
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
        bucket[0].id = senderId;
        bucket[0].next = NULL;

        sender = malloc(sizeof(struct transactionNode));
        //pernao to onoma tou receiver
        sender->id = receiverId;
        sender->tranId = tranId;
        sender->date = date;
        sender->time = tranTime;
        sender->value = value;
        sender->next = NULL;


        //check if 1 bitcoin holds all the amount
        bitcoins = userWallet[senderPos].bitcoin;
        while(bitcoins != NULL){
          // checkAmountOfBitcoin(bitcoins->treeHead,senderId,&currentAmount);
          currentBitcoin = amountOfBitcoin(bitcoins,value);

          //it means we found it
          if(currentBitcoin > 0){
            root = getBitCoinTree(userWallet, walletSize, currentBitcoin);
            break;
          }
          bitcoins = bitcoins->next;
        }
        //we find 1 bitcoin that holds the value
        if(currentBitcoin > 0){
          while(amount > 0){
            traverseTree(root,sender,senderId,receiverId,&amount);
          }
        }
        //get the bitcoins, 1 at a time and get the value we want from each one
        else{
          bitcoins = userWallet[senderPos].bitcoin;
          amount = value;
          while(amount > 0 && bitcoins != NULL){
              root = getBitCoinTree(userWallet, walletSize, bitcoins->bitcoinId);
              traverseTree(root,sender,senderId,receiverId,&amount);
              bitcoins = bitcoins->next;
            }
        }

        bucket[0].transaction = sender;
        //the last node we found will point to the new one
        head->next = bucket;
        break;
      }
      //move to the next bucket
      head = head->next;
    }
  }
}


void print(struct hashNode* hashTable, int numOfEntries, int bucketSize){
  struct bucket *head, *current;
  int key;

  for(int i=0;i<numOfEntries;i++){
    head = hashTable[i].bucket;
    printf("Thesi hashTable: %d\n",i);
    while(head != NULL){
      for(int j=0; j<bucketSize; j++){
      printf("\nThesi bucket: %d\n",j);
        if(head[j].id != NULL){
          while(head[j].transaction != NULL){
            printf("UserId %s",head[j].id);
            printf(" ,sent to: ->%s     ",head[j].transaction->id);
            head[j].transaction = head[j].transaction->next;
          }
        }
      }
      printf("\n");
      head=head->next;
    }
    printf("%s\n","NULL");
  }
}

void deleteHashTable(struct hashNode* hashTable, int numOfEntries, int bucketSize){
  struct bucket* head, *btemp;
  struct transactionNode *stemp;
  for(int i=0; i<numOfEntries; i++){
    head = hashTable[i].bucket;
    while(head != NULL){
      for(int j=0; j<bucketSize; j++){
          while(head[j].transaction != NULL){
            stemp = head[j].transaction;
            head[j].transaction = head[j].transaction->next;
            free(stemp);
          }
      }
      btemp = head;
      head = head->next;
      free(btemp);
    }
  }
  free(hashTable);
}
