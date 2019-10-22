struct hashNode {
  struct bucket* bucket;
};

struct bucket {
  char *id;
  struct transactionNode* transaction;
  struct bucket* next;
};

struct transactionNode {
  char *id;
  int tranId;
  char* date;
  char* time;
  int value;
  struct transactionNode* next;
};

struct wallet;
struct treeNode;

void insertInSenderHashTable(struct hashNode* , struct wallet*, int, int, char*, char*, int, char*, char*, int, int, int);
void deleteHashTable(struct hashNode* , int, int);
void print(struct hashNode* , int , int);
