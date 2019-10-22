#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/sender.h"
#include "./headers/receiver.h"
#include "./headers/functions.h"
#include "./headers/wallets.h"
#include "./headers/tree.h"
#include "./headers/handleInput.h"


int main(int argc, char *argv[]){

  int c, bitCoinValue, senderEntries, receiverEntries,
      bucketSize, bitFile, bitcoin, tranFile, senderPos,
      receiverPos, max;
  int bitLines = 0, traLines = 0, pos = 0, tValue = 0, tranSize = 0;
  int *transactionIds;
  FILE *bitCoinFile, *transactionsFile;
  size_t bufsize;
  char *txtWord, *sender, *receiver, *value;
  char **buffer, **buffera;
  struct wallet *userWallet;
  char *lastDate, *lastTime;

  if(argc != 13){
    printf("Some arguments are missing\n");
    exit(0);
  }
  //Handle the arguments from the command line
  for(int arg = 1; arg < argc; arg++){
    //if we are not at the last arg
    if(arg < argc - 1){
      if(strncmp(argv[arg], "-a", 2) == 0){
        bitFile = arg + 1;
      }
      else if(strncmp(argv[arg], "-t", -2) == 0){
        tranFile = arg + 1;
      }
      else if(strncmp(argv[arg], "-v", -2) == 0){
        bitCoinValue = atoi(argv[arg + 1]);
      }
      else if(strncmp(argv[arg], "-h1", -3) == 0){
        senderEntries = atoi(argv[arg + 1]);
      }
      else if(strncmp(argv[arg], "-h2", -3) == 0){
        receiverEntries = atoi(argv[arg + 1]);
      }
      else if(strncmp(argv[arg], "-b", -2) == 0){
        bucketSize = atoi(argv[arg + 1]);
        bucketSize = bucketSize / sizeof(struct bucket);
      }
    }
  }

  bitCoinFile = fopen(argv[bitFile], "r");
  if(bitCoinFile){
    //count the number of lines
    while ((c = getc(bitCoinFile)) != EOF){
      if(c == '\n'){
        bitLines++;
      }
    }
    buffer = malloc(bitLines*sizeof(char*));
    for(int i=0; i<bitLines; i++){
      buffer[i] = NULL;
    }
    rewind(bitCoinFile);
    //store every line of the file to a buffer
    for(int i=0; i<bitLines; i++){
      getline(&buffer[i], &bufsize, bitCoinFile);
    }

    userWallet = malloc(bitLines*sizeof(struct wallet));

    //get the words from each line
    for(int i = 0; i<bitLines; i++){
      txtWord = strtok(buffer[i]," \n\t");
      //check for duplicate name
      if(duplicateNames(userWallet, txtWord, pos)){
        printf("The name %s found again.Rejecting that user\n",txtWord);
      }
      else{
        //insert data
        userWallet[pos].userId = txtWord;
        userWallet[pos].balance = 0;
        userWallet[pos].bitcoin = NULL;

        //get every line of file
        txtWord = strtok(NULL, " \n\t");
        while(txtWord != NULL){
          bitcoin = atoi(txtWord);
          for(int j=0; j<pos; j++){
            //check for duplicate bitcoinId
            if(duplicateBitcoins(userWallet[j].bitcoin, bitcoin, pos)){
              printf("The bitcoin %d found again.The program will now exit\n",bitcoin);
              for(int i=0; i<bitLines; i++){
                deleteWallet(&userWallet[i].bitcoin);
              }
              free(userWallet);
              free(buffer);
              fclose(bitCoinFile);
              exit(0);
            }
          }
          userWallet[i].balance += bitCoinValue;
          //gia kapoio logo diavazei to keno sto telos tis grammis
          if(bitcoin == 0)  break;
          //insert bitcoin to the wallet
          initializeBitcoinList(&userWallet[pos].bitcoin,bitcoin,bitCoinValue,userWallet[pos].userId);
          txtWord = strtok(NULL, " \n\t");
        }
        pos++;
      }
    }

    for(int i=0; i<bitLines; i++){
      if(userWallet[i].balance == 0){
        userWallet[i].userId[strlen(userWallet[i].userId) - 1] = 0;
      }
    }

  }
  else{
    printf("Something is wrong with the bitCoinBalancesFile\n");
    exit(0);
  }
  //Initialize the hashTables
  struct hashNode* senderHashTable = malloc(senderEntries*sizeof(struct hashNode));
  for(int i=0; i<senderEntries; i++){
    senderHashTable[i].bucket = NULL;
  }
  struct hashNode* receiverHashTable = malloc(receiverEntries*sizeof(struct hashNode));
  for(int i=0; i<receiverEntries; i++){
    receiverHashTable[i].bucket = NULL;
  }

  transactionsFile = fopen(argv[tranFile], "r");
  if(transactionsFile){
    //count the number of lines
    while ((c = getc(transactionsFile)) != EOF){
      if(c == '\n'){
        traLines++;
      }
    }
    buffera = malloc(traLines*sizeof(char*));
    for(int i=0; i<traLines; i++){
      buffera[i] = NULL;
    }
    transactionIds = malloc(traLines*sizeof(int));
    rewind(transactionsFile);

    for(int i=0; i<traLines; i++){
      getline(&buffera[i], &bufsize, transactionsFile);
    }
    for(int i=0; i<traLines; i++){
      txtWord = strtok(buffera[i]," \n\t");
      transactionIds[tranSize] = atoi(txtWord);
      if(uniqueTransactions(transactionIds, transactionIds[tranSize], tranSize) < 0){
        printf("Found a duplicate transactionId\n");
        continue;
      }
      //get the sender
      sender = strtok(NULL," \n\t");
      senderPos = userExists(userWallet,sender,pos);
      if(senderPos == -1){
        printf("One user(sender) in the transactionsFile doesn't exist\n");
        continue;
      }
      //get the receiver
      receiver = strtok(NULL," \n\t");
      receiverPos = userExists(userWallet,receiver,pos);
      if(receiverPos == -1){
        printf("One user(receiver) in the transactionsFile doesn't exist\n");
        continue;
      }
      if(strcmp(sender,receiver) == 0){
        printf("The sender can't be the same person with the receiver\n");
        continue;
      }
      //get the value
      value = strtok(NULL," \n\t");
      tValue = atoi(value);
      if(checkBalance(userWallet[senderPos].balance,tValue) < 0){
        printf("Not enough balance in %s's wallet\n",sender);
        continue;
      }

      //get the date
      txtWord = strtok(NULL," \n\t");
      lastDate = txtWord;
      if(!dateValidation(txtWord)){
        printf("Wrong date form\n");
        continue;
      }
      //get the time
      txtWord = strtok(NULL," \n\t");
      lastTime = txtWord;
      if(!timeValidation(txtWord)){
        printf("Wrong time form\n");
        continue;
      }

      // searchAndDeleteEmptyBitcoins(&userWallet[senderPos].bitcoin);
      //start the transactions
      senderBalance(userWallet, tValue, senderPos);
      receiverBalance(userWallet, tValue, receiverPos);
      insertInSenderHashTable(senderHashTable, userWallet, senderPos, pos, lastDate, lastTime, transactionIds[tranSize], sender, receiver, tValue, senderEntries, bucketSize);
      insertInReceiverHashTable(receiverHashTable, lastDate, lastTime, transactionIds[tranSize], sender, receiver, tValue, receiverEntries, bucketSize);
      updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, tValue);

      tranSize++;
    }
  }
  else{
    printf("Something is wrong with the transactionsFile\n");
    exit(0);
  }

  max = getTransactionMax(transactionIds,tranSize);
  handleInput(senderHashTable, receiverHashTable, userWallet, max, senderEntries, receiverEntries, bucketSize, pos, lastDate, lastTime);

  // print(receiverHashTable,senderEntries, bucketSize);
  // print(senderHashTable,senderEntries, bucketSize);
  // for(int i=0; i<bitLines; i++){
  //   printf(" %s\n",userWallet[i].userId);
  //   while(userWallet[i].bitcoin != NULL){
  //     printf(" bitcoin %d value %d\n",userWallet[i].bitcoin->bitcoinId,userWallet[i].bitcoin->amount);
  //     userWallet[i].bitcoin = userWallet[i].bitcoin->next;
  //   }
  // }

  //free memory
  for(int i=0; i<traLines; i++){
    free(buffera[i]);
  }
  free(buffera);

  for(int i=0; i<bitLines; i++){
    free(buffer[i]);
  }
  free(buffer);
  free(transactionIds);
  fclose(bitCoinFile);
  fclose(transactionsFile);

  struct bucket* head, *btemp;
  struct transactionNode *stemp;
  for(int i=0; i<receiverEntries; i++){
    head = receiverHashTable[i].bucket;
    while(head != NULL){
      for(int j=0; j<bucketSize; j++){
          while(head[j].transaction != NULL){
            stemp = head[j].transaction;
            head[j].transaction = head[j].transaction->next;
            free(stemp->date);
            free(stemp->time);
            free(stemp);
          }
      }
      btemp = head;
      head = head->next;
      free(btemp);
    }
  }
  for(int i=0; i<senderEntries; i++){
    head = senderHashTable[i].bucket;
    while(head != NULL){
      for(int j=0; j<bucketSize; j++){
          while(head[j].transaction != NULL){
            stemp = head[j].transaction;
            head[j].transaction = head[j].transaction->next;
            free(stemp->date);
            free(stemp->time);
            free(stemp);
          }
      }
      btemp = head;
      head = head->next;
      free(btemp);
    }
  }
  free(receiverHashTable);
  free(senderHashTable);

  for(int i=0; i<bitLines; i++){
    // if(userWallet[i].bitcoin->treeHead != NULL){
    //   deleteTree(userWallet[i].bitcoin->treeHead);
    // }
    deleteWallet(&userWallet[i].bitcoin);
  }
  free(userWallet);
  // deleteHashTable(senderHashTable, senderEntries, bucketSize);
  // deleteHashTable(receiverHashTable, receiverEntries, bucketSize);
}
