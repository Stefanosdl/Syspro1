#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/tree.h"
#include "./headers/sender.h"
#include "./headers/wallets.h"


void checkAmountOfBitcoin(struct treeNode* root, char* sender, int* value){
  if(!root){
    return;
  }
  // if node is leaf node
  if(!root->left && !root->right){
    //check if we find the id we want
    if(strcmp(root->id,sender) == 0){
      *value += root->value;
    }
  }
  checkAmountOfBitcoin(root->left,sender,value);
  checkAmountOfBitcoin(root->right,sender,value);
}

void traverseTree(struct treeNode* root, struct transactionNode* transaction, char* sender, char* receiver, int* amount){
    if(root == NULL){
      return;
    }
    // if node is leaf node
    if(!root->left && !root->right && root->value > 0){
      //check if we find the id we want
      if(strcmp(root->id,sender) == 0){
        //create the new leaf nodes
          struct treeNode* leftNode = malloc(sizeof(struct treeNode));
          struct treeNode* rightNode = malloc(sizeof(struct treeNode));
          leftNode->id = receiver;
          rightNode->id = sender;
          leftNode->left = NULL;
          leftNode->right = NULL;
          rightNode->left = NULL;
          rightNode->right = NULL;
          //check if value of bitcoin is not enough
          if(root->value >= *amount){
            leftNode->value = *amount;
            rightNode->value = root->value - *amount;
            *amount -= root->value;
          }
          else{
            leftNode->value = root->value;
            rightNode->value = 0;
            *amount -= root->value;
          }
          root->left = leftNode;
          root->left->transaction = transaction;
          root->right = rightNode;
        return;
      }
    }
    // if left child exists, check for leaf
    // recursively
    if(root->left){
      traverseTree(root->left,transaction,sender,receiver,amount);
    }

    // if right child exists, check for leaf
    // recursively
    if(root->right){
     traverseTree(root->right,transaction,sender,receiver,amount);
    }
}

void printLevelOrder(struct treeNode* root){
    int h = height(root);
    int i;
    for (i=1; i<=h; i++){
      printGivenLevel(root, i);
    }
}

/* Print nodes at a given level */
void printGivenLevel(struct treeNode* root, int level){
  if(root == NULL){
    return;
  }
  if(level == 1){
    if(root->left != NULL){
      printf("%d %s %s %d %s %s\n",root->left->transaction->tranId,root->id,
      root->left->transaction->id,root->left->value,root->left->transaction->date,
      root->left->transaction->time);
    }
  }
  else if(level > 1){
    printGivenLevel(root->left, level-1);
    printGivenLevel(root->right, level-1);
  }
}

/* Compute the "height" of a tree -- the number of
    nodes along the longest path from the root node
    down to the farthest leaf node.*/
int height(struct treeNode* node){
  if (node == NULL){
    return 0;
  }
  else{
    /* compute the height of each subtree */
    int lheight = height(node->left);
    int rheight = height(node->right);

    /* use the larger one */
    if (lheight > rheight){
      return(lheight+1);
    }
    else {
      return(rheight+1);
    }
  }
}

int countTransactions(struct treeNode* root){
  if (root == NULL || (root->left == NULL && root->right == NULL)){
    return 0;
  }
  return 1 + countTransactions(root->left) + countTransactions(root->right);
}

void deleteTree(struct treeNode* node){
  if(node == NULL){
    return;
  }

    deleteTree(node->left);
    deleteTree(node->right);
    free(node);
}
