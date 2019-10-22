#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/wallets.h"
#include "./headers/sender.h"

int getTransactionMax(int *transactionIds,int traLines){
  int max = 0;
  for(int i=0; i<traLines; i++){
    if(transactionIds[i] > max){
      max = transactionIds[i];
    }
  }
  return max;
}

int uniqueTransactions(int *transactionIds,int id,int pos){
  for(int i=0; i<pos; i++){
    if(transactionIds[i] == id){
      return -1;
    }
  }
  return 1;
}

int duplicateNames(struct wallet* allUsers, char* currentUser, int size){
  for(int i=0; i<size; i++){
    if(strcmp(allUsers[i].userId,currentUser) == 0){
      return 1;
    }
  }
  return 0;
}

int duplicateBitcoins(struct bitcoinsList* allBitcoins, int bitcoin, int size){
  while(allBitcoins != NULL){
    if(allBitcoins->bitcoinId == bitcoin){
      return 1;
    }
    allBitcoins = allBitcoins->next;
  }
  return 0;
}

int checkIfIsDate(char* date){
  char *endptr;
  int base = 10;
  int day;
  char buf[4];

  day = strtol(date, &endptr, base);

  if(endptr[0] == '-'){
    return 1;
  }
  return 0;
}

int dateValidation(char* date){
    char *endptr;
    int base = 10;
    int year, month, day;
    char buf[4];
    day = strtol(date, &endptr, base);
    //check day range here, return 0 on error
    sprintf(buf,"%d",day);
    if(strlen(buf) > 2){
      printf("Day must be a 2 digit number\n");
      return 0;
    }
    else{
      sprintf(buf,"%2d", day);
      if(strlen(buf) != 2){
        printf("Day must be a 2 digit number\n");
        return 0;
      }
    }
    if(endptr[0] == '-'){
        month = strtol(endptr + 1, &endptr, base);
        //check month range
        sprintf(buf,"%d",month);
        if(strlen(buf) > 2){
          printf("Month must be a 2 digit number\n");
          return 0;
        }
        else{
          sprintf(buf,"%2d", month);
          if(strlen(buf) != 2){
            printf("Month must be a 2 digit number\n");
            return 0;
          }
        }
    }
    else{
        printf("Missing month\n");
        return 0;
    }
    if (endptr[0] == '-') {
        year = strtol(endptr + 1, &endptr, base);
        //check year range
        sprintf(buf,"%d",year);
        if(strlen(buf) != 4){
          printf("Year must be a 4 digit number\n");
          return 0;
        }
    }
    else{
        printf("Missing year\n");
        return 0;
    }
    return 1;
}

int checkLastDate(char* lastDate, char* date){
  char *fendptr, *sendptr;
  int base = 10;
  int fyear, fmonth, fday;
  int syear, smonth, sday;
  char buf[4];

  fday = strtol(lastDate, &fendptr, base);
  sday = strtol(date, &sendptr, base);
  fmonth = strtol(fendptr + 1, &fendptr, base);
  smonth = strtol(sendptr + 1, &sendptr, base);
  fyear = strtol(fendptr + 1, &fendptr, base);
  syear = strtol(sendptr + 1, &sendptr, base);

  if(syear > fyear){
    return 1;
  }
  else if(syear == fyear){
    if(smonth > fmonth){
      return 1;
    }
    else if(smonth == fmonth){
      if(sday > fday){
        return 1;
      }
      else if(sday == fday){
        return 2;
      }
    }
  }
  return 0;
}

int checkLastTime(char* lastTime, char* tTime){
  char *fendptr, *sendptr;
  int base = 10;
  int fhours, fminutes;
  int shours, sminutes;
  char buf[2];

  fhours = strtol(lastTime, &fendptr, base);
  shours = strtol(tTime, &sendptr, base);
  fminutes = strtol(fendptr + 1, &fendptr, base);
  sminutes = strtol(sendptr + 1, &sendptr, base);

  if(shours > fhours){
    return 1;
  }
  else if(shours == fhours){
    if(sminutes > fminutes){
      return 1;
    }
  }
  return 0;
}

int dateComparison(char* first, char* second, char* tranDate){
  char *fendptr, *sendptr, *tendptr;
  int base = 10;
  int fyear, fmonth, fday;
  int syear, smonth, sday;
  int tyear, tmonth, tday;
  char buf[4];

  fday = strtol(first, &fendptr, base);
  sday = strtol(second, &sendptr, base);
  tday = strtol(tranDate, &tendptr, base);
  fmonth = strtol(fendptr + 1, &fendptr, base);
  smonth = strtol(sendptr + 1, &sendptr, base);
  tmonth = strtol(tendptr + 1, &tendptr, base);
  fyear = strtol(fendptr + 1, &fendptr, base);
  syear = strtol(sendptr + 1, &sendptr, base);
  tyear = strtol(tendptr + 1, &tendptr, base);


  if((tyear > fyear && tyear < syear)){
    return 1;
  }
  else if(tyear == fyear && tyear < syear){
    if(tmonth > fmonth){
      return 1;
    }
    else if(tmonth == fmonth){
      if(tday >= fday){
        return 1;
      }
    }
  }
  else if(tyear > fyear && tyear == syear){
    if(tmonth < smonth){
      return 1;
    }
    else if(tmonth == smonth){
      if(tday <= sday){
        return 1;
      }
    }
  }
  else if(tyear == fyear && tyear == syear){
    if(tmonth > fmonth && tmonth < smonth){
      return 1;
    }
    else if(tmonth == fmonth && tmonth < smonth){
      if(tday >= fday){
        return 1;
      }
    }
    else if(tmonth > fmonth && tmonth == smonth){
      if(tday >= sday){
        return 1;
      }
    }
    else if(tmonth == fmonth && tmonth == smonth){
      if(tday > fday && tday < sday){
        return 1;
      }
      else if(tday == fday && tday < sday){
        return 1;
      }
      else if(tday > fday && tday == sday){
        return 1;
      }
      else if(tday == fday && tday == sday){
        return 2;
      }
    }
  }

  return 0;
}

int checkIfIsTime(char* tTime){
  char *endptr;
  int base = 10;
  int hours;
  char buf[2];

  hours = strtol(tTime, &endptr, base);

  if(endptr[0] == ':'){
    return 1;
  }
  return 0;
}


int timeValidation(char* tTime){
  char *endptr;
  int base = 10;
  int hours, minutes;
  char buf[2];

  hours = strtol(tTime, &endptr, base);
  //check hour range here, return 0 on error
  sprintf(buf,"%d",hours);
  if(strlen(buf) > 2){
    printf("Hours must be a 2 digit number\n");
    return 0;
  }
  else{
    sprintf(buf,"%2d", hours);
    if(strlen(buf) != 2){
      printf("Hours must be a 2 digit number\n");
      return 0;
    }
  }
  if(endptr[0] == ':'){
      minutes = strtol(endptr + 1, &endptr, base);
      //check year range
      sprintf(buf,"%d",minutes);
      if(strlen(buf) > 2){
        printf("Minutes must be a 2 digit number\n");
        return 0;
      }
      else{
        sprintf(buf,"%2d", hours);
        if(strlen(buf) != 2){
          printf("Hours must be a 2 digit number\n");
          return 0;
        }
      }
  }
  else{
      printf("Missing Minutes\n");
      return 0;
  }
  return 1;
}

int timeComparison(char* first, char* second, char* tranTime){
  char *fendptr, *sendptr, *tendptr;
  int base = 10;
  int fhours, fminutes;
  int shours, sminutes;
  int thours, tminutes;
  char buf[2];

  fhours = strtol(first, &fendptr, base);
  shours = strtol(second, &sendptr, base);
  thours = strtol(tranTime, &tendptr, base);
  fminutes = strtol(fendptr + 1, &fendptr, base);
  sminutes = strtol(sendptr + 1, &sendptr, base);
  tminutes = strtol(tendptr + 1, &tendptr, base);

  if((thours > fhours && thours < shours)){
    return 1;
  }
  else if(thours == fhours && thours < shours){
    if(tminutes >= fminutes){
      return 1;
    }
  }
  else if(thours > fhours && thours == shours){
    if(tminutes <= sminutes){
      return 1;
    }
  }
  else if(thours == fhours && thours == shours){
    if(tminutes >= fminutes && tminutes <= sminutes){
      return 1;
    }
  }

  return 0;
}

void findAll(struct hashNode* hashTable, int numOfEntries, char* user, int bucketSize){
  int c;
  char* temp = user;
  struct bucket* bucket;
  struct transactionNode* cur;
  unsigned long key = 5381;
  int exists = 0;
  int amount = 0;

  while (c = *temp++){
    key = ((key << 5) + key) + c;
  }

  key = key % numOfEntries;

  bucket = hashTable[key].bucket;
  while(bucket != NULL){
    for(int i=0; i<bucketSize; i++){
      if(bucket[i].id != NULL){
        if(strcmp(bucket[i].id,user) == 0){
          exists = 1;
          cur = bucket[i].transaction;
          while(cur != NULL){
            amount += cur->value;
            cur = cur->next;
          }
          if(exists == 1){
            printf("%d\n",amount);
            break;
          }
        }
      }
    }
    bucket = bucket->next;
  }
}

void findAllHistory(struct hashNode* hashTable, int numOfEntries, char* user, int bucketSize){
  int c;
  char* temp = user;
  struct bucket* bucket;
  struct transactionNode* cur;
  unsigned long key = 5381;
  int exists = 0;

  while (c = *temp++){
    key = ((key << 5) + key) + c;
  }

  key = key % numOfEntries;

  bucket = hashTable[key].bucket;
  while(bucket != NULL){
    for(int i=0; i<bucketSize; i++){
      if(bucket[i].id != NULL){
        if(strcmp(bucket[i].id,user) == 0){
          exists = 1;
          cur = bucket[i].transaction;
          while(cur != NULL){
            printf("%d %s %s %d %s %s\n",cur->tranId,cur->id,user,cur->value,cur->date,cur->time);
            cur = cur->next;
          }
          if(exists == 1){
            break;
          }
        }
      }
    }
    bucket = bucket->next;
  }
}



void findSpecificHistory(struct hashNode* hashTable, int numOfEntries, char* time1, char* time2, char* year1, char* year2, char* user, int bucketSize){
  int c;
  char* temp = user;
  struct bucket* bucket;
  struct transactionNode* cur;
  unsigned long key = 5381;
  int exists = 0;

  while (c = *temp++){
    key = ((key << 5) + key) + c;
  }

  key = key % numOfEntries;

  bucket = hashTable[key].bucket;
  while(bucket != NULL){
    for(int i=0; i<bucketSize; i++){
      if(bucket[i].id != NULL){
        if(strcmp(bucket[i].id,user) == 0){
          exists = 1;
          cur = bucket[i].transaction;
          while(cur != NULL){
            if(time1[0] != ' ' && time2[0] != ' ' && year1[0] != ' ' && year2[0] != ' '){
              if(dateComparison(year1,year2,cur->date) == 1){
                printf("%d %s %s %d %s %s\n",cur->tranId,user,cur->id,cur->value,cur->date,cur->time);
              }
              else if(dateComparison(year1,year2,cur->date) == 2){
                if(timeComparison(time1,time2,cur->time)){
                  printf("%d %s %s %d %s %s\n",cur->tranId,user,cur->id,cur->value,cur->date,cur->time);
                }
              }
            }
            else if(time1[0] == ' ' && time2[0] == ' '){
              if(dateComparison(year1,year2,cur->date)){
                printf("%d %s %s %d %s %s\n",cur->tranId,user,cur->id,cur->value,cur->date,cur->time);
              }
            }
            else if(year1[0] == ' ' && year2[0] == ' '){
              if(timeComparison(time1,time2,cur->time)){
                printf("%d %s %s %d %s %s\n",cur->tranId,user,cur->id,cur->value,cur->date,cur->time);
              }
            }
            cur = cur->next;
          }
          if(exists == 1){
            break;
          }
        }
      }
    }
    bucket = bucket->next;
  }
}

void findSpecific(struct hashNode* hashTable, int numOfEntries, char* time1, char* time2, char* year1, char* year2, char* user, int bucketSize){
  int c;
  char* temp = user;
  struct bucket* bucket;
  struct transactionNode* cur;
  unsigned long key = 5381;
  int exists = 0;
  int amount = 0;

  while (c = *temp++){
    key = ((key << 5) + key) + c;
  }

  key = key % numOfEntries;

  bucket = hashTable[key].bucket;
  while(bucket != NULL){
    for(int i=0; i<bucketSize; i++){
      if(bucket[i].id != NULL){
        if(strcmp(bucket[i].id,user) == 0){
          cur = bucket[i].transaction;
          while(cur != NULL){
            if(time1[0] != ' ' && time2[0] != ' ' && year1[0] != ' ' && year2[0] != ' '){
              if(dateComparison(year1,year2,cur->date) == 1){
                exists = 1;
                amount += cur->value;
              }
              else if(dateComparison(year1,year2,cur->date) == 2){
                if(timeComparison(time1,time2,cur->time)){
                  exists = 1;
                  amount += cur->value;
                }
              }
            }
            else if(time1[0] == ' ' && time2[0] == ' '){
              if(dateComparison(year1,year2,cur->date)){
                exists = 1;
                amount += cur->value;
              }
            }
            else if(year1[0] == ' ' && year2[0] == ' '){
              if(timeComparison(time1,time2,cur->time)){
                exists = 1;
                amount += cur->value;
              }
            }

            cur = cur->next;
          }
          if(exists == 1){
            printf("%d\n",amount);
            break;
          }
        }
      }
    }
    if(exists == 1){
      break;
    }
    bucket = bucket->next;
  }
}

char lastChar(char* c){
   if(c == NULL || *c == '\0') return 0;
    return c[strlen(c)-1];
}
