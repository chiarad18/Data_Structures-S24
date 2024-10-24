#include "AVL_Database.hpp"
#include <iostream>

using namespace std;

void print(AVLNode *root){

    cout<<endl;
    cout<<root->record->key<<endl;

    if(root->left == nullptr && root->right == nullptr){
        return;
    } 

    if(root->left != nullptr){
        print(root->left);
    }

    if(root->right != nullptr){
        print(root->right);
    }
    cout<<endl;
}

int main(){
    
    auto r1=new Record("a", 10);
    auto r2=new Record("b", 20);
    auto r3=new Record("d", 40);
    auto r4=new Record("e", 50);
    auto r5=new Record("fg", 70);
    auto r6=new Record("r", 88);


    AVLNode *node1 = new AVLNode(r1);
    auto node2 = new AVLNode(r2);
    auto node3 = new AVLNode(r3);
    auto node4 = new AVLNode(r4);

    node1->right = node2;
    node2->left = node3;

    AVLTree *tree = new AVLTree();
    tree->root = node1;


    print(tree->root);


    tree->calcTotalHeight(tree->root);
    cout<<"after calcHeight: a height: "<<node1->height<<"  b height: "<< node2->height<<"  d height: "<<node3->height<<  " e height: "<<node4->height<<endl;

    cout<<"root is "<<tree->root->record->key<<endl;


    tree->rebalance(tree->root, tree->root);

    cout<<"made it out of rebalance"<<endl;
    print(tree->root);

}




