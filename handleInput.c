#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./headers/sender.h"
#include "./headers/receiver.h"
#include "./headers/functions.h"
#include "./headers/wallets.h"
#include "./headers/tree.h"

#define MAX_str 31

void handleInput(struct hashNode* senderHashTable, struct hashNode* receiverHashTable, struct wallet* userWallet, int max, int senderEntries, int receiverEntries, int bucketSize, int walletSize, char* lastDate, char* lastTime){
  char* line = NULL;
  char* token, *txtWord, *sender, *receiver, *value, *inputDate, *inputTime;
  char** word;
  size_t size, bufsize;
  int count = 0, len = 0, input = 0, inputLines, senderPos, receiverPos, walletId, amount, bitcoinId, c;
  time_t rawtime;
  FILE *inputFile;
  struct tm * timeinfo;
  char dbuffer[15];
  char tbuffer[10];
  char **buffer;
  struct treeNode* tree;
  char* cDate = malloc(sizeof(char*));
  char* cTime = malloc(sizeof(char*));
  word = malloc(MAX_str*sizeof(char*));


  while(1){
    if (getline(&line, &size, stdin) == -1) {
      if(getline(&line, 0, stdin) == -1){
        printf("No line\n");
      }
    }
    if(input != 0){
      for(int i=0; i<input; i++){
        free(word[i]);
      }
    }
    input = 0;
    token = strtok(line, " \t\n");

    while(token != NULL){
      len = strlen(token);
      word[input] = malloc(sizeof(char*));
      strcpy(word[input],token);
      input++;
      if(input == (MAX_str + 1)){
        break;
      }
      token = strtok(NULL, " \t\n");
    }
    if(input == 0){
      printf("No line\n");
      continue;
    }
    if(strncmp(word[0],"/requestTransaction",19) == 0 && strlen(word[0]) == 19){
      if(input >= 4){
        senderPos = userExists(userWallet,word[1],walletSize);
        if(senderPos == -1){
          printf("The user %s doesn't exist\n",word[1]);
          continue;
        }
        receiverPos = userExists(userWallet,word[2],walletSize);
        if(receiverPos == -1){
          printf("The user %s doesn't exist\n",word[2]);
          continue;
        }
        if(strcmp(word[1],word[2]) == 0){
          printf("The sender can't be the same person with the receiver\n");
          continue;
        }
        amount = atoi(word[3]);
        if(checkBalance(userWallet[walletSize].balance,amount) < 0){
          printf("Not enough balance in your wallet\n");
          continue;
        }
        if(input ==  6){
          if(!dateValidation(word[4])){
            printf("Wrong date form\n");
            continue;
          }
          if(!timeValidation(word[5])){
            printf("Wrong time form\n");
            continue;
          }
          if(checkLastDate(lastDate,word[4]) == 1){
            max += 1;
            senderBalance(userWallet, amount, senderPos);
            receiverBalance(userWallet, amount, receiverPos);
            strcpy(cDate,word[4]);
            strcpy(cTime,word[5]);
            insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
            insertInReceiverHashTable(receiverHashTable, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
            updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
            printf("Transaction with id %d completed\n",max);
            strcpy(lastDate,word[4]);
            strcpy(lastTime,word[5]);
          }
          else if(checkLastDate(lastDate,word[4]) == 2){
            if(checkLastTime(lastTime,word[5])){
              max += 1;
              senderBalance(userWallet, amount, senderPos);
              receiverBalance(userWallet, amount, receiverPos);
              strcpy(cDate,word[4]);
              strcpy(cTime,word[5]);
              insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
              insertInReceiverHashTable(receiverHashTable, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
              updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
              printf("Transaction with id %d completed\n",max);
              strcpy(lastDate,word[4]);
              strcpy(lastTime,word[5]);
            }
            else{
              printf("Wrong date and time\n");
              continue;
            }
          }
          else{
            printf("Wrong date and time\n");
            continue;
          }

        }
        else if(input == 5){
          if(checkIfIsTime(word[4])){
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(dbuffer,sizeof(dbuffer),"%d-%m-%Y",timeinfo);
            max += 1;
            senderBalance(userWallet, amount, senderPos);
            receiverBalance(userWallet, amount, receiverPos);
            strcpy(cTime,word[4]);
            insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, dbuffer, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
            insertInReceiverHashTable(receiverHashTable, dbuffer, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
            updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
            printf("Transaction with id %d completed\n",max);
            strcpy(lastDate,dbuffer);
            strcpy(lastTime,word[4]);
          }
          else if(checkIfIsDate(word[4])){
            if(checkLastDate(lastDate,word[4]) >= 1){
              time(&rawtime);
              timeinfo = localtime(&rawtime);
              strftime(tbuffer,sizeof(tbuffer),"%H:%M",timeinfo);
              max += 1;
              senderBalance(userWallet, amount, senderPos);
              receiverBalance(userWallet, amount, receiverPos);
              strcpy(cDate,word[4]);
              insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
              insertInReceiverHashTable(receiverHashTable, cDate, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
              updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
              printf("Transaction with id %d completed\n",max);
              strcpy(lastDate,word[4]);
              strcpy(lastTime,tbuffer);
            }
            else{
              printf("Wrong date and time\n");
              continue;
            }
          }
          else{
            printf("Wrong date and time\n");
            continue;
          }
        }
        else if(input == 4){
          time(&rawtime);
          timeinfo = localtime(&rawtime);
          strftime(dbuffer,sizeof(dbuffer),"%d-%m-%Y",timeinfo);
          strftime(tbuffer,sizeof(tbuffer),"%H:%M",timeinfo);
          max += 1;
          senderBalance(userWallet, amount, senderPos);
          receiverBalance(userWallet, amount, receiverPos);
          insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, dbuffer, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
          insertInReceiverHashTable(receiverHashTable, dbuffer, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
          updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
          printf("Transaction with id %d completed on %s %s\n",max,dbuffer,tbuffer);
          strcpy(lastDate,dbuffer);
          strcpy(lastTime,tbuffer);
        }
      }
      else if(input < 4){
        printf("Missing arguments\n");
        continue;
      }
    }
    else if(strncmp(word[0],"/requestTransactions",20) == 0){
      ssize_t read = 0;
      inputLines = 0;
      if(input == 2){
        inputFile = fopen(word[1], "r");

        if(inputFile){

          while ((c = getc(inputFile)) != EOF){
            if(c == '\n'){
              inputLines++;
            }
          }
          buffer = malloc(inputLines*sizeof(char*));
          for(int i=0; i<inputLines; i++){
            buffer[i] = NULL;
          }
          rewind(inputFile);

          for(int i=0; i<inputLines; i++){
            getline(&buffer[i], &bufsize, inputFile);
          }
          for(int i=0; i<inputLines; i++){
            sender = strtok(buffer[i]," \n\t");
            if(sender == NULL){
              printf("Missing sender\n");
              continue;
            }
            senderPos = userExists(userWallet,sender,walletSize);
            if(senderPos == -1){
              printf("One user(sender) in the inputFile doesn't exist\n");
              continue;
            }
            receiver = strtok(NULL," \n\t");
            if(receiver == NULL){
              printf("Missing receiver\n");
              continue;
            }
            receiverPos = userExists(userWallet,receiver,walletSize);
            if(receiverPos == -1){
              printf("One user(receiver) in the inputFile doesn't exist\n");
              continue;
            }
            if(strcmp(sender,receiver) == 0){
              printf("The sender can't be the same person with the receiver\n");
              continue;
            }
            value = strtok(NULL," \n\t");
            if(value == NULL){
              printf("Missing amount\n");
              continue;
            }

            //check if this is the last word fo the line
            inputDate = strtok(NULL," \n\t");
            //if input == 3
            if(inputDate == NULL){
              if(lastChar(value) == ';'){
                value[strlen(value) - 1] = 0;
                amount = atoi(value);
                if(checkBalance(userWallet[senderPos].balance,amount) < 0){
                  printf("Not enough balance in your wallet\n");
                  continue;
                }
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                strftime(dbuffer,sizeof(dbuffer),"%d-%m-%Y",timeinfo);
                strftime(tbuffer,sizeof(tbuffer),"%H:%M",timeinfo);
                max += 1;
                senderBalance(userWallet, amount, senderPos);
                receiverBalance(userWallet, amount, receiverPos);
                insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, dbuffer, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                insertInReceiverHashTable(receiverHashTable, dbuffer, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                printf("Transaction with id %d completed on %s %s\n",max,dbuffer,tbuffer);
                strcpy(lastDate,dbuffer);
                strcpy(lastTime,tbuffer);
              }
              else{
                printf("Missing ; at the end of the input\n");
                continue;
              }
            }
            //if input == 4
            else if((inputTime = strtok(NULL," \n\t")) == NULL){
              if(lastChar(inputDate) == ';'){
                amount = atoi(value);
                if(checkBalance(userWallet[senderPos].balance,amount) < 0){
                  printf("Not enough balance in your wallet\n");
                  continue;
                }
                inputDate[strlen(inputDate) - 1] = 0;
                if(!dateValidation(inputDate)){
                  printf("Wrong date form\n");
                  continue;
                }
                if(checkIfIsTime(inputDate)){
                  time(&rawtime);
                  timeinfo = localtime(&rawtime);
                  strftime(dbuffer,sizeof(dbuffer),"%d-%m-Y",timeinfo);
                  max += 1;
                  senderBalance(userWallet, amount, senderPos);
                  receiverBalance(userWallet, amount, receiverPos);
                  strcpy(cTime,inputDate);
                  insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, dbuffer, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                  insertInReceiverHashTable(receiverHashTable, dbuffer, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                  updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                  printf("Transaction with id %d completed\n",max);
                  strcpy(lastDate,dbuffer);
                  strcpy(lastTime,inputDate);
                }
                else if(checkIfIsDate(inputDate)){
                  if(checkLastDate(lastDate,inputDate) >= 1){
                    time(&rawtime);
                    timeinfo = localtime(&rawtime);
                    strftime(tbuffer,sizeof(tbuffer),"%H:%M",timeinfo);
                    max += 1;
                    senderBalance(userWallet, amount, senderPos);
                    receiverBalance(userWallet, amount, receiverPos);
                    strcpy(cDate,inputDate);
                    insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                    insertInReceiverHashTable(receiverHashTable, cDate, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                    updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                    printf("Transaction with id %d completed\n",max);
                    strcpy(lastDate,inputDate);
                    strcpy(lastTime,tbuffer);
                  }
                  else{
                    printf("Wrong date and time\n");
                    continue;
                  }
                }
                else{
                  printf("Wrong date and time\n");
                  continue;
                }
              }
              else{
                printf("Missing ; at the end of the input\n");
                continue;
              }
            }
            //if input == 5
            else{
              if(lastChar(inputTime) == ';'){
                amount = atoi(value);
                if(checkBalance(userWallet[senderPos].balance,amount) < 0){
                  printf("Not enough balance in your wallet\n");
                  continue;
                }
                if(!dateValidation(inputDate)){
                  printf("Wrong date form\n");
                  continue;
                }
                inputTime[strlen(inputTime) - 1] = 0;
                if(!timeValidation(inputTime)){
                  printf("Wrong time form\n");
                  continue;
                }
                if(checkLastDate(lastDate,inputDate) == 1){
                  max += 1;
                  senderBalance(userWallet, amount, senderPos);
                  receiverBalance(userWallet, amount, receiverPos);
                  strcpy(cDate,inputDate);
                  strcpy(cTime,inputTime);
                  insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                  insertInReceiverHashTable(receiverHashTable, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                  updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                  printf("Transaction with id %d completed\n",max);
                  strcpy(lastDate,inputDate);
                  strcpy(lastTime,inputTime);
                }
                else if(checkLastDate(lastDate,inputDate) == 2){
                  if(checkLastTime(lastTime,inputTime)){
                    max += 1;
                    senderBalance(userWallet, amount, senderPos);
                    receiverBalance(userWallet, amount, receiverPos);
                    strcpy(cDate,inputDate);
                    strcpy(cTime,inputTime);
                    insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                    insertInReceiverHashTable(receiverHashTable, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                    updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                    printf("Transaction with id %d completed\n",max);
                    strcpy(lastDate,inputDate);
                    strcpy(lastTime,inputTime);
                  }
                  else{
                    printf("Wrong date and time\n");
                    continue;
                  }
                }
                else{
                  printf("Wrong date and time\n");
                  continue;
                }
              }
              else{
                printf("Missing ; at the end of the input\n");
                continue;
              }
            }
          }
        }
        else{
          printf("Something is wrong with the input file");
          continue;
        }

        free(buffer);
        fclose(inputFile);
        continue;
      }
      else if(input == 3 || input == 1){
        printf("Missing arguments\n");
      }
      else if(input >= 4){
        senderPos = userExists(userWallet,word[1],walletSize);
        if(senderPos == -1){
          printf("The user %s doesn't exist\n",word[1]);
          continue;
        }
        receiverPos = userExists(userWallet,word[2],walletSize);
        if(receiverPos == -1){
          printf("The user %s doesn't exist\n",word[2]);
          continue;
        }
        if(strcmp(word[1],word[2]) == 0){
          printf("The sender can't be the same person with the receiver\n");
          continue;
        }
        if(input == 6){
          amount = atoi(word[3]);
          if(checkBalance(userWallet[walletSize].balance,amount) < 0){
            printf("Not enough balance in your wallet\n");
            continue;
          }
          if(lastChar(word[5]) == ';'){
            if(!dateValidation(word[4])){
              printf("Wrong date form\n");
              continue;
            }
            word[5][strlen(word[5]) - 1] = 0;
            if(!timeValidation(word[5])){
              printf("Wrong time form\n");
              continue;
            }

            if(checkLastDate(lastDate,word[4]) == 1){
              max += 1;
              senderBalance(userWallet, amount, senderPos);
              receiverBalance(userWallet, amount, receiverPos);
              strcpy(cDate,word[4]);
              strcpy(cTime,word[5]);
              insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
              insertInReceiverHashTable(receiverHashTable, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
              updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
              printf("Transaction with id %d completed\n",max);
              strcpy(lastDate,word[4]);
              strcpy(lastTime,word[5]);
            }
            else if(checkLastDate(lastDate,word[4]) == 2){
              if(checkLastTime(lastTime,word[5])){
                max += 1;
                senderBalance(userWallet, amount, senderPos);
                receiverBalance(userWallet, amount, receiverPos);
                strcpy(cDate,word[4]);
                strcpy(cTime,word[5]);
                insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                insertInReceiverHashTable(receiverHashTable, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                printf("Transaction with id %d completed\n",max);
                strcpy(lastDate,word[4]);
                strcpy(lastTime,word[5]);
              }
              else{
                printf("Wrong date and time\n");
                continue;
              }
            }
            else{
              printf("Wrong date and time\n");
              continue;
            }
          }

          else{
            printf("Missing ; at the end of the line\n");
          }
        }
        else if(input == 5){
          amount = atoi(word[3]);
          if(checkBalance(userWallet[walletSize].balance,amount) < 0){
            printf("Not enough balance in your wallet\n");
            continue;
          }
          if(lastChar(word[4]) == ';'){
            word[4][strlen(word[4]) - 1] = 0;
            if(!dateValidation(word[4])){
              printf("Wrong date form\n");
              continue;
            }

            if(checkIfIsTime(word[4])){
              time(&rawtime);
              timeinfo = localtime(&rawtime);
              strftime(dbuffer,sizeof(dbuffer),"%d-%m-%Y",timeinfo);
              max += 1;
              senderBalance(userWallet, amount, senderPos);
              receiverBalance(userWallet, amount, receiverPos);
              strcpy(cTime,word[4]);
              insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, dbuffer, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
              insertInReceiverHashTable(receiverHashTable, dbuffer, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
              updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
              printf("Transaction with id %d completed\n",max);
              strcpy(lastDate,dbuffer);
              strcpy(lastTime,word[4]);
            }
            else if(checkIfIsDate(word[4])){
              if(checkLastDate(lastDate,word[4]) >= 1){
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                strftime(tbuffer,sizeof(tbuffer),"%H:%M",timeinfo);
                max += 1;
                senderBalance(userWallet, amount, senderPos);
                receiverBalance(userWallet, amount, receiverPos);
                strcpy(cDate,word[4]);
                insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                insertInReceiverHashTable(receiverHashTable, cDate, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                printf("Transaction with id %d completed\n",max);
                strcpy(lastDate,word[4]);
                strcpy(lastTime,tbuffer);
              }
              else{
                printf("Wrong date and time\n");
                continue;
              }
            }
            else{
              printf("Wrong date and time\n");
              continue;
            }
          }
          else{
            printf("Missing ; at the end of the line\n");
          }
        }
        else if(input == 4){
          if(lastChar(word[3]) == ';'){
            word[3][strlen(word[3]) - 1] = 0;
            amount = atoi(word[3]);
            if(checkBalance(userWallet[walletSize].balance,amount) < 0){
              printf("Not enough balance in your wallet\n");
              continue;
            }
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(dbuffer,sizeof(dbuffer),"%d-%m-%Y",timeinfo);
            strftime(tbuffer,sizeof(tbuffer),"%H:%M",timeinfo);
            max += 1;
            senderBalance(userWallet, amount, senderPos);
            receiverBalance(userWallet, amount, receiverPos);
            insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, dbuffer, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
            insertInReceiverHashTable(receiverHashTable, dbuffer, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
            updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
            printf("Transaction with id %d completed on %s %s\n",max,dbuffer,tbuffer);
            strcpy(lastDate,dbuffer);
            strcpy(lastTime,tbuffer);
          }
        else{
          printf("Missing ; at the end of the line\n");
        }
      }
    }

      // start the loop for each line
      while(read = getline(&line, &size, stdin) != EOF){
        for(int i=0; i<input; i++){
          free(word[i]);
        }
        input = 0;
        token = strtok(line, " \t\n");
        while(token != NULL){
          len = strlen(token);
          word[input] = malloc(sizeof(char*));
          strcpy(word[input],token);
          input++;
          if(input == (MAX_str + 1)){
            break;
          }
          token = strtok(NULL, " \t\n");
        }
        if(line[0] == '\n'){
          free(line);
          break;
        }

        if(input < 3){
          printf("Missing arguments\n");
        }
        else if(input >= 3){
          senderPos = userExists(userWallet,word[0],walletSize);
          if(senderPos == -1){
            printf("The user %s doesn't exist\n",word[0]);
            continue;
          }
          receiverPos = userExists(userWallet,word[1],walletSize);
          if(receiverPos == -1){
            printf("The user %s doesn't exist\n",word[1]);
            continue;
          }
          if(strcmp(word[0],word[1]) == 0){
            printf("The sender can't be the same person with the receiver\n");
            continue;
          }
          if(input == 5){
            amount = atoi(word[2]);
            if(checkBalance(userWallet[walletSize].balance,amount) < 0){
              printf("Not enough balance in your wallet\n");
              continue;
            }
            if(lastChar(word[4]) == ';'){
              if(!dateValidation(word[3])){
                printf("Wrong date form\n");
                continue;
              }
              word[4][strlen(word[4]) - 1] = 0;
              if(!timeValidation(word[4])){
                printf("Wrong time form\n");
                continue;
              }
              if(checkLastDate(lastDate,word[3]) == 1){
                max += 1;
                senderBalance(userWallet, amount, senderPos);
                receiverBalance(userWallet, amount, receiverPos);
                strcpy(cDate,word[3]);
                strcpy(cTime,word[4]);
                insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                insertInReceiverHashTable(receiverHashTable, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                printf("Transaction with id %d completed\n",max);
                strcpy(lastDate,word[3]);
                strcpy(lastTime,word[4]);
              }
              else if(checkLastDate(lastDate,word[3]) == 2){
                if(checkLastTime(lastTime,word[4])){
                  max += 1;
                  senderBalance(userWallet, amount, senderPos);
                  receiverBalance(userWallet, amount, receiverPos);
                  strcpy(cDate,word[3]);
                  strcpy(cTime,word[4]);
                  insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                  insertInReceiverHashTable(receiverHashTable, cDate, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                  updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                  printf("Transaction with id %d completed\n",max);
                  strcpy(lastDate,word[3]);
                  strcpy(lastTime,word[4]);
                }
                else{
                  printf("Wrong date and time\n");
                  continue;
                }
              }
              else{
                printf("Wrong date and time\n");
                continue;
              }
            }
            else{
              printf("Missing ; at the end of the line\n");
              continue;
            }
          }
          else if(input == 4){
            amount = atoi(word[2]);
            if(checkBalance(userWallet[walletSize].balance,amount) < 0){
              printf("Not enough balance in your wallet\n");
              continue;
            }
            if(lastChar(word[3]) == ';'){
              word[3][strlen(word[3]) - 1] = 0;
              if(!dateValidation(word[3])){
                printf("Wrong date form\n");
                continue;
              }

              if(checkIfIsTime(word[3])){
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                strftime(dbuffer,sizeof(dbuffer),"%d-%m-Y",timeinfo);
                max += 1;
                senderBalance(userWallet, amount, senderPos);
                receiverBalance(userWallet, amount, receiverPos);
                strcpy(cTime,word[3]);
                insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, dbuffer, cTime, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                insertInReceiverHashTable(receiverHashTable, dbuffer, word[3], max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                printf("Transaction with id %d completed\n",max);
                strcpy(lastDate,dbuffer);
                strcpy(lastTime,word[3]);
              }
              else if(checkIfIsDate(word[3])){
                if(checkLastDate(lastDate,word[3]) >= 1){
                  time(&rawtime);
                  timeinfo = localtime(&rawtime);
                  strftime(tbuffer,sizeof(tbuffer),"%H:%M",timeinfo);
                  max += 1;
                  senderBalance(userWallet, amount, senderPos);
                  receiverBalance(userWallet, amount, receiverPos);
                  strcpy(cDate,word[3]);
                  insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, cDate, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
                  insertInReceiverHashTable(receiverHashTable, word[3], tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
                  updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
                  printf("Transaction with id %d completed\n",max);
                  strcpy(lastDate,word[3]);
                  strcpy(lastTime,tbuffer);
                }
                else{
                  printf("Wrong date and time\n");
                  continue;
                }
              }
              else{
                printf("Wrong date and time\n");
                continue;
              }
            }
            else{
              printf("Missing ; at the end of the line\n");
            }
          }
          else if(input == 3){
            if(lastChar(word[2]) == ';'){
              word[2][strlen(word[2]) - 1] = 0;
              amount = atoi(word[2]);
              if(checkBalance(userWallet[walletSize].balance,amount) < 0){
                printf("Not enough balance in your wallet\n");
                continue;
              }
              time(&rawtime);
              timeinfo = localtime(&rawtime);
              strftime(dbuffer,sizeof(dbuffer),"%d-%m-%Y",timeinfo);
              strftime(tbuffer,sizeof(tbuffer),"%H:%M",timeinfo);
              max += 1;
              senderBalance(userWallet, amount, senderPos);
              receiverBalance(userWallet, amount, receiverPos);
              insertInSenderHashTable(senderHashTable, userWallet, senderPos, walletSize, dbuffer, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, senderEntries, bucketSize);
              insertInReceiverHashTable(receiverHashTable, dbuffer, tbuffer, max, userWallet[senderPos].userId, userWallet[receiverPos].userId, amount, receiverEntries, bucketSize);
              updateSenderBitcoin(userWallet[senderPos].bitcoin, &userWallet[receiverPos].bitcoin, amount);
              printf("Transaction with id %d completed on %s %s\n",max,dbuffer,tbuffer);
              strcpy(lastDate,dbuffer);
              strcpy(lastTime,tbuffer);
            }
            else{
              printf("Missing ; at the end of the line\n");
            }
          }
        }
      }
    }
    else if(strncmp(word[0],"/findEarnings",13) == 0){
      if(input > 1){
        int user = userExists(userWallet,word[1],walletSize);
        if(user == -1){
          printf("User doesn't exist\n");
          continue;
        }
      }
      if(input == 2){
        findAll(receiverHashTable,receiverEntries,word[1],bucketSize);
        findAllHistory(receiverHashTable,receiverEntries,word[1],bucketSize);
      }
      else if(input == 4){
        if(checkIfIsTime(word[2])){
          if(!timeValidation(word[2])){
            printf("Wrong time form\n");
            continue;
          }
          if(!checkIfIsTime(word[3])){
            printf("You need to enter a second time\n");
            continue;
          }
          else{
            if(!timeValidation(word[3])){
              printf("Wrong time form\n");
              continue;
            }
          }
        }
        else if(checkIfIsDate(word[2])){
          if(!dateValidation(word[2])){
            printf("Wrong date form\n");
            continue;
          }
          if(!checkIfIsDate(word[3])){
            printf("You need to enter a second date\n");
            continue;
          }
          else{
            if(!dateValidation(word[3])){
              printf("Wrong date form\n");
              continue;
            }
          }
        }

        if(checkIfIsTime(word[2])){
          findSpecific(receiverHashTable,receiverEntries,word[2],word[3]," "," ",word[1],bucketSize);
          findSpecificHistory(receiverHashTable,receiverEntries,word[2],word[3]," "," ",word[1],bucketSize);
        }
        else if(checkIfIsDate(word[2])){
          findSpecific(receiverHashTable,receiverEntries," "," ",word[2],word[3],word[1],bucketSize);
          findSpecificHistory(receiverHashTable,receiverEntries," "," ",word[2],word[3],word[1],bucketSize);
        }
      }
      else if(input == 6){
        if(!timeValidation(word[2]) || !timeValidation(word[4])){
          printf("Wrong time form\n");
          continue;
        }
        if(!dateValidation(word[5])){
          printf("Wrong date form\n");
          continue;
        }
        if(!dateValidation(word[3]) || !dateValidation(word[5])){
          printf("Wrong date form\n");
          continue;
        }

        findSpecific(receiverHashTable,receiverEntries,word[2],word[4],word[3],word[5],word[1],bucketSize);
        findSpecificHistory(receiverHashTable,receiverEntries,word[2],word[4],word[3],word[5],word[1],bucketSize);

      }
      else{
        printf("Missing arguments\n");
        continue;
      }
    }
    else if(strncmp(word[0],"/findPayments",13) == 0){
      if(input > 1){
        int user = userExists(userWallet,word[1],walletSize);
        if(user == -1){
          printf("User doesn't exist\n");
          continue;
        }
      }
      if(input == 2){
        findAll(senderHashTable,senderEntries,word[1],bucketSize);
        findAllHistory(senderHashTable,senderEntries,word[1],bucketSize);
      }
      else if(input == 4){
        if(checkIfIsTime(word[2])){
          if(!timeValidation(word[2])){
            printf("Wrong time form\n");
            continue;
          }
          if(!checkIfIsTime(word[3])){
            printf("You need to enter a second time\n");
            continue;
          }
          else{
            if(!timeValidation(word[3])){
              printf("Wrong time form\n");
              continue;
            }
          }
        }
        else if(checkIfIsDate(word[2])){
          if(!dateValidation(word[2])){
            printf("Wrong date form\n");
            continue;
          }
          if(!checkIfIsDate(word[3])){
            printf("You need to enter a second date\n");
            continue;
          }
          else{
            if(!dateValidation(word[3])){
              printf("Wrong date form\n");
              continue;
            }
          }
        }

        if(checkIfIsTime(word[2])){
          findSpecific(senderHashTable,senderEntries,word[2],word[3]," "," ",word[1],bucketSize);
          findSpecificHistory(senderHashTable,senderEntries,word[2],word[3]," "," ",word[1],bucketSize);
        }
        else if(checkIfIsDate(word[2])){
          findSpecific(senderHashTable,senderEntries," "," ",word[2],word[3],word[1],bucketSize);
          findSpecificHistory(senderHashTable,senderEntries," "," ",word[2],word[3],word[1],bucketSize);
        }
      }
      else if(input == 6){
        if(!timeValidation(word[2]) || !timeValidation(word[4])){
          printf("Wrong time form\n");
          continue;
        }
        if(!dateValidation(word[5])){
          printf("Wrong date form\n");
          continue;
        }
        if(!dateValidation(word[3]) || !dateValidation(word[5])){
          printf("Wrong date form\n");
          continue;
        }

        findSpecific(senderHashTable,senderEntries,word[2],word[4],word[3],word[5],word[1],bucketSize);
        findSpecificHistory(senderHashTable,senderEntries,word[2],word[4],word[3],word[5],word[1],bucketSize);

      }
      else{
        printf("Missing arguments\n");
        continue;
      }
    }
    else if(strncmp(word[0],"/walletStatus",13) == 0){
      if(input == 2){
        walletId = userExists(userWallet,word[1],walletSize);
        if(walletId == -1){
          printf("The user %s doesn't exist\n",word[1]);
          continue;
        }
        printf("Wallet balance for %s is %d$\n",userWallet[walletId].userId,userWallet[walletId].balance);
      }
      else{
        printf("Wrong amount of arguments\n");
      }
    }
    else if(strncmp(word[0],"/bitCoinStatus",14) == 0){
      if(input == 2){
        bitcoinId = atoi(word[1]);
        int foundBit = 0;
        for(int i=0; i<walletSize; i++){
          if(duplicateBitcoins(userWallet[i].bitcoin, bitcoinId, walletSize)){
            tree = getBitCoinTree(userWallet, walletSize, bitcoinId);
            printf("%d %d %d\n",bitcoinId,countTransactions(tree),nonUsedAmount(userWallet,walletSize,bitcoinId));
            foundBit = 1;
            break;
          }
        }
        if(foundBit == 0){
          printf("This bitcoin doesn't exist\n");
        }
      }
      else{
        printf("Wrong amount of arguments\n");
      }
    }
    else if(strncmp(word[0],"/traceCoin",10) == 0){
      if(input == 2){
        bitcoinId = atoi(word[1]);
        int foundBit = 0;
        for(int i=0; i<walletSize; i++){
          if(duplicateBitcoins(userWallet[i].bitcoin, bitcoinId, walletSize)){
            tree = getBitCoinTree(userWallet, walletSize, bitcoinId);
            printLevelOrder(tree);
            foundBit = 1;
            break;
          }
        }
        if(foundBit == 0){
          printf("This bitcoin doesn't exist\n");
        }
      }
      else{
        printf("Wrong amount of arguments\n");
      }
    }
    else if(strncmp(word[0],"/exit",5) == 0){
      // free(line);
      free(word);
      free(cDate);
      free(cTime);
      return;
    }
    else{
      printf("Wrong input\n");
    }
  }
}
