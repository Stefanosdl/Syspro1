#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/functions.h"
#include "./headers/wallets.h"
#include "./headers/tree.h"

void initializeBitcoinList(struct bitcoinsList** head, int bitcoin, int bitCoinValue, char* owner){
  struct bitcoinsList* newNode;
  struct bitcoinsList* last;
  struct treeNode* treeHead;

  newNode = malloc(sizeof(struct bitcoinsList));
  newNode->bitcoinId = bitcoin;
  newNode->amount = bitCoinValue;
  newNode->next = NULL;

  treeHead = malloc(sizeof(struct treeNode));
  treeHead->id = owner;
  treeHead->value = bitCoinValue;
  treeHead->left = NULL;
  treeHead->right = NULL;
  treeHead->transaction = NULL;

  newNode->treeHead = treeHead;
  // If the bitcoin list is empty, then make the new node as head
  if(*head == NULL){
    *head = newNode;
  }
  else{
    last = *head;
    while(last->next != NULL){
      last = last->next;
    }
    last->next = newNode;
  }
}


void updateSenderBitcoin(struct bitcoinsList* head, struct bitcoinsList** receiver, int value){
  struct bitcoinsList* temp = head;
  int flag = 0;
  //check if we find a bitcoin that holds the whole value we want
  while(temp != NULL){
    if(temp->amount >= value){
      temp->amount -= value;
      updatereceiverBitcoin(receiver, value, temp->bitcoinId);
      flag = 1;
      break;
    }
    temp = temp->next;
  }
  if(!flag){
    int amount = 0;
    int currentValue = value;
    while(head != NULL){
      if(head->amount >0){
        //if we didnt find the amount we want
        if(amount < value){
          //if the value that is left is more or equal than the bitcoin's amount
          //then take it all
          if(currentValue >= head->amount){
            amount += head->amount;
            currentValue -= head->amount;
            updatereceiverBitcoin(receiver, head->amount, head->bitcoinId);
            head->amount = 0;
          }
          //else the value that is left is less than the amount of the bitcoin
          //then take only the amount we need from that bitcoin
          else{
            updatereceiverBitcoin(receiver, currentValue, head->bitcoinId);
            head->amount -= currentValue;
            currentValue = 0;
            amount = value;
          }
        }
        else{
          break;
        }
      }
      head = head->next;
    }
  }
}

void updatereceiverBitcoin(struct bitcoinsList** head, int value, int bitcoinId){
  struct bitcoinsList* temp = *head;
  struct bitcoinsList* last;
  int flag = 0;
  //loop through the bitcoin list of receiver
  //and check if that bitcoin exists
  while(temp != NULL){
    //if we find it
    if(temp->bitcoinId == bitcoinId){
      temp->amount += value;
      flag = 1;
      break;
    }
    temp = temp->next;
  }
  //if we dont find that bitcoin
  //we add it to the receiver's bitcoin list
  if(!flag){
    struct bitcoinsList* newNode;
    newNode = malloc(sizeof(struct bitcoinsList));
    newNode->bitcoinId = bitcoinId;
    newNode->amount = value;
    newNode->next = NULL;
    newNode->treeHead = NULL;
    // If the bitcoin list is empty, then make the new node as head
    if(*head == NULL){
      *head = newNode;
    }
    else{
      //find the last node
        last = *head;
        while(last->next != NULL){
          last = last->next;
        }
        last->next = newNode;
    }
  }
}


void searchAndDeleteEmptyBitcoins(struct bitcoinsList** head){
  struct bitcoinsList* temp = *head, *prev = NULL;


  //If head node itself holds the key to be deleted or multiple occurrences of key
  while(temp != NULL && temp->amount == 0){
    *head = temp->next;
    free(temp);
    temp = *head;
  }
  //loop through all the nodes
  while(temp != NULL){
    //Search for the key to be deleted, keep track of the
    //previous node as we need to change 'prev->next'
    while(temp != NULL && temp->amount != 0){
      prev = temp;
      temp = temp->next;
    }
    //it means we find the node to be deleted
    if(temp != NULL){
      prev->next = temp->next;
      free(temp);
      temp = prev->next;
    }
  }
}

void senderBalance(struct wallet* currentWallet, int value, int pos){
  currentWallet[pos].balance -= value;
}

void receiverBalance(struct wallet* currentWallet, int value, int pos){
  currentWallet[pos].balance += value;
}

int userExists(struct wallet* allUsers, char* id, int pos){
  for(int i=0; i<pos; i++){
    if(strcmp(allUsers[i].userId,id) == 0){
      return i;
    }
  }
  return -1;
}

int checkBalance(int balance, int value){
  return balance - value;
}

int amountOfBitcoin(struct bitcoinsList* head, int value){
  while(head != NULL){
    if(head->amount >= value){
      return head->bitcoinId;
    }
    head = head->next;
  }
  return -1;
}


struct treeNode* getBitCoinTree(struct wallet* head, int walletSize, int bitcoinId){
  struct bitcoinsList* bitcoin;
  for(int i=0; i<walletSize; i++){
    bitcoin = head[i].bitcoin;
    while(bitcoin != NULL){
      if(bitcoin->bitcoinId == bitcoinId && (bitcoin->treeHead != NULL)){
        return bitcoin->treeHead;
      }
      bitcoin = bitcoin->next;
    }
  }
}

int nonUsedAmount(struct wallet* head, int walletSize, int bitcoinId){
  struct bitcoinsList* bitcoin;
  for(int i=0; i<walletSize; i++){
    bitcoin = head[i].bitcoin;
    while(bitcoin != NULL){
      if(bitcoin->bitcoinId == bitcoinId && (bitcoin->treeHead != NULL)){
        return bitcoin->amount;
      }
      bitcoin = bitcoin->next;
    }
  }
}

void deleteWallet(struct bitcoinsList** head){
   struct bitcoinsList* current = *head;
   struct bitcoinsList* next;

   while (current != NULL){
       next = current->next;
       deleteTree(current->treeHead);
       free(current);
       current = next;
   }

   *head = NULL;
}
