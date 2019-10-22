struct treeNode {
  char* id;
  int value;
  struct treeNode *left,*right;
  struct transactionNode* transaction;
};

struct bitcoinsList;

void checkAmountOfBitcoin(struct treeNode* , char* , int* );
void traverseTree(struct treeNode* , struct transactionNode*, char*, char*, int*);
void printGivenLevel(struct treeNode* , int );
int height(struct treeNode* );
void printLevelOrder(struct treeNode* );
int countTransactions(struct treeNode* );
void deleteTree(struct treeNode* );
