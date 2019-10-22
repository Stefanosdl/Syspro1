struct wallet {
  char *userId;
  int balance;
  struct bitcoinsList *bitcoin;
};

struct bitcoinsList{
  int bitcoinId;
  int amount;
  struct bitcoinsList *next;
  struct treeNode *treeHead;
};


void initializeBitcoinList(struct bitcoinsList**, int, int, char*);
int amountOfBitcoin(struct bitcoinsList* , int );
int userExists(struct wallet* , char*, int);
int checkBalance(int , int);
void senderBalance(struct wallet* , int , int );
void receiverBalance(struct wallet* , int , int );
void updateSenderBitcoin(struct bitcoinsList* , struct bitcoinsList**, int);
void updatereceiverBitcoin(struct bitcoinsList** , int, int);
void searchAndDeleteEmptyBitcoins(struct bitcoinsList** );
struct treeNode* getBitCoinTree(struct wallet* , int, int);
int nonUsedAmount(struct wallet* , int , int);
void deleteWallet(struct bitcoinsList** );
