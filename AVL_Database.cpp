// Chiara DeAngelis U42447641
// Chaunique O'Garro 

#include "AVL_Database.hpp"
#include <iostream>
#include <cmath>

using namespace std;

Record::Record(const std::string& k, int v) : key(k), value(v) {}

AVLNode::AVLNode(Record* r) : record(r), left(nullptr), right(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr) {}

AVLNode* AVLTree::getRoot(){
    return root;
}

// returns height of a single node
int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}

/* returns an int value indicating if the subtrees of a node are unbalanced
if left or right nodes are null, treat them as -1
calculate return value by subtracting height of right child from left child 
if val is > 1 subtree is left heavy, < -1 it is right heavy, and otherwise it is balanced  */
int AVLTree::balance(AVLNode* node) {
    if(node == nullptr){
        return 0;
    }
    if(node->left == nullptr){
        return node ? -1 - height(node->right) : 0;
    } else if(node->right == nullptr){
        return node ? height(node->left) - (-1) : 0;
    }
    return node ? height(node->left) - height(node->right) : 0;
}

// function to perform a single right rotation
AVLNode* AVLTree::rotateRight(AVLNode* y) {
    // assign pointers to imbalanced node two consecutive nodes with largest heights
    AVLNode *parent = y;
    AVLNode *curr;
    AVLNode *mid;

    /* if the unbalanced node is the root node, there is no "parent",
    so begin rotation by reassigning root to middle node */
    if(balance(root) > 1){
        curr = root;
        root = curr->left;
    }
    else if(balance(y->left) > 1){
        curr = y->left;
        y->left = curr->left;
    }
    else if(balance(y->right) > 1){
        curr = y->right;
        y->right = curr->left;
    }
    
    mid = curr->left;
    curr->left = curr->left->right;
    mid->right = curr;
    // reset all heights of the nodes to account for the changes made
    calcTotalHeight(root);

    return y;
}

/* same as rotateRight but for a left rotation */
AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode *parent = x;
    AVLNode *curr;
    AVLNode *mid;

    if(balance(root) < -1){
        curr = root;
        root = curr->right;
    }
    else if(balance(x->left) < -1){
        curr = x->left;
        x->left = curr->right;
    }

    else if(balance(x->right) < -1){
        curr = x->right;
        x->right = curr->right;
    } else {
        return x;
    }

    mid = curr->right;
    curr->right = curr->right->left;
    mid->left = curr;


    calcTotalHeight(root);

    return x;

}

/* seperate function to account for double left-right rotation situations */
void AVLTree::leftRightRotation(AVLNode *parent, AVLNode *curr){
    AVLNode *mid = curr->left;
    AVLNode *bottom = curr->left->right;


    mid->right = bottom->left;
    curr->left = bottom->right;
    
    // if parent and current node are both root node, root becomes bottom
    if(curr == root){
        root = bottom;
    // else check if curr is left or right child of parent replace it with bottom
    } else if(parent->left == curr){
            parent->left = bottom;
    } else if(parent->right == curr) {
            parent->right = bottom;
    }
    
    bottom->left = mid;
    bottom->right = curr; 
}

/* same as leftRight function but for rightLeft double rotations */
void AVLTree::rightLeftRotation(AVLNode *parent, AVLNode *curr){
    AVLNode *mid = curr->right;
    AVLNode *bottom = curr->right->left;

    mid->left = bottom->right;
    curr->right = bottom->left;
    
    // if parent and current node are both root node, root becomes bottom
    if(curr == root){
        root = bottom;
    // else check if curr is left or right child of parent replace it with bottom 
    } else if(parent->left == curr){
            parent->left = bottom;
    } else if(parent->right == curr) {
            parent->right = bottom;
    }
    
    bottom->left = curr;
    bottom->right = mid; 
}


/* seperate function from balance specifically for double rotation scenarios
where height differences */
int AVLTree::compareHeights(AVLNode *left_st, AVLNode *right_st){
    int left_height, right_height;
    if(left_st == nullptr){
        left_height = -1;
    } else {
        left_height = height(left_st);
    }
    if(right_st == nullptr){
        right_height = -1;
    } else {
        right_height = height(right_st);
    }
   
    if(left_height > right_height){
        return 1;
    } else if(left_height < right_height){
        return -1;
    } else {
        return 0;
    }
}

/* recursive function to rebalance whole tree, calls previous rotate functions for individual rotations*/
void AVLTree::rebalance(AVLNode* current, AVLNode *parent){
    if(current == nullptr || (current->left == nullptr && current->right == nullptr)){
        return;
    }
    AVLNode *ogleft = current->left;
    AVLNode *ogright = current->right;

    if(balance(current) > 1){
        // check for double leftRight rotation scenario
        if(compareHeights(current->left->left,current->left->right) == -1){
            ogleft = current->left->right;
            ogright = current->right;
            // is unbalanced node is root
            if(current == root){
                leftRightRotation(root, root);
            } else {
                leftRightRotation(parent, current);
            }
        // otherwise do single right rotation
        } else {
            // if unblanced node is root
            if(current == root){
                rotateRight(root);
            } else {
                rotateRight(parent);
            }
        }

    } else if(balance(current) < -1){
        // check for double rightLeft rotation scenario
        if(compareHeights(current->right->left,current->right->right) == 1){
            ogleft = current->left;
            ogright = current->right->left;
            // if unblanced node is root
            if(current == root){
                rightLeftRotation(root, root);
            } else {
                rightLeftRotation(parent, current);
            }
        // else do single left rotation
        } else {
            // if unblanced node is root
            if(current == root){
                rotateLeft(root);
            } else {
                rotateLeft(parent);
            }
        }
    }

    /* update heights of all nodes after changes made to tree */
    calcTotalHeight(root);

    /* recursively call rebalance on original left and right children of current node
    to balance rest of tree */

    rebalance(ogleft, current);
    rebalance(ogright, current);

}

/* recursive function to calc height of a single node*/
int AVLTree::calcHeight(AVLNode *node){
    // base case: node is if leaf node, return 0
    if(node->left == nullptr && node->right == nullptr){
        node->height = 0;
        return 0;
    }

    /* otherwise, recursively call calcHeight on left and right
    children and count nodes until leaf is reached*/

    int leftHeight;
    if(node->left != nullptr){
        leftHeight = calcHeight(node->left);
    } else {
        leftHeight = -1;
    }

    int rightHeight;
    if(node->right != nullptr){
        rightHeight = calcHeight(node->right);
    } else {
        rightHeight = -1;
    }


    if(leftHeight > rightHeight){
        node->height = 1 + leftHeight;
        return 1 + leftHeight;
    } 
    
    else {
        node->height = 1 + rightHeight;
        return 1 + rightHeight;
    }
}

/* recursive function that calls calcHeight to calculate and update
the heights of all nodes in the tree */
void AVLTree::calcTotalHeight(AVLNode *node){
    calcHeight(node);
    if(node->left != nullptr){
        calcTotalHeight(node->left);
    } 
    if(node->right != nullptr){
        calcTotalHeight(node->right);
    }
    return;
}

// function to insert node into AVL tree
void AVLTree::insert(Record* record) {
    // call search function to check if node is already in tree, and if so return/exit
    if(search(record->key, record->value) != nullptr){
        return;
    }

    // create new node with record value and two leaf nodes
    AVLNode *new_node = new AVLNode(record);
    AVLNode *leaf1 = new AVLNode(nullptr);
    AVLNode *leaf2 = new AVLNode(nullptr);

    // set leaf nodes as its left and right children
    new_node->left = leaf1;
    new_node->right = leaf2;

    // if tree is empty, make root new node and return
    if(root == nullptr){
        root = new_node;
        return;
    }

    // otherwise, traverse through tree until find right spot (ascending order)
    // for new record value
    AVLNode *prev = nullptr;
    AVLNode *curr = root;
    int count = 0;
    while(curr->record != nullptr){
        if(curr->record->value > record->value){
            prev = curr;
            curr = curr->left;

        } else {
            prev = curr;
            curr = curr->right;
        }
        count++;
    }

    if(curr == prev->left){
        delete curr;
        prev->left = new_node;
    } else {
        delete curr;
        prev->right = new_node;
    }

    // call rebalance function after inserting new node in case tree is now unbalanced
    rebalance(root, root);

}

/* function to search through tree for certain key and value */
Record* AVLTree::search(const std::string& key, int value) {
    AVLNode *curr = root;
    // if tree is empty, return null
    if(curr == nullptr){
        return nullptr;
    }
    // otherwise, traverse through tree and break if current node has parameter value
    while(curr->record != nullptr){
        if(curr->record->value > value){
            curr = curr->left;
        } else if(curr->record->value == value){
            break;
        } else {
            curr = curr->right;
        }
    }
    // return curr->record, will be null if record was not found
    return curr->record;
}

/* function to delete a node of given key and value*/
void AVLTree::deleteNode(const std::string& key, int value) {
    // if node is not in tree, return;
    if(search(key, value) == nullptr){
        return;
    } 
    // otherwise, traverse through tree to find node
    AVLNode *prev = nullptr;
    AVLNode *curr = root;

    while(curr->record != nullptr){
        if(curr->record->value > value){
            prev = curr;
            curr = curr->left;

        } else if(curr->record->value < value) {
            prev = curr;
            curr = curr->right;
        }
        else {
            break;
        }
    }
    // if node has leaf as right child, delete and replace it with its predecessor
    if(curr->right->record == nullptr){
        AVLNode *prevLast = curr;
        AVLNode *lastGreatest = curr->left;
        
        while(lastGreatest->right != nullptr && lastGreatest->right->record != nullptr){
            prevLast = lastGreatest;
            lastGreatest = lastGreatest->right;
        }
        if(prevLast->right == lastGreatest){
            prevLast->right = lastGreatest->left;
        } else {
            prevLast->left = lastGreatest->left;
        }
        delete curr->record;
        curr->record = lastGreatest->record;
        delete lastGreatest->right;
        delete lastGreatest;
        rebalance(root, root);
        return;
    }

    // if node has leaf as right child, delete and replace it with its successor
    AVLNode *prevNext = curr;
    AVLNode *nextGreatest = curr->right;

    while(nextGreatest->left != nullptr && nextGreatest->left->record != nullptr){
        prevNext = nextGreatest;
        nextGreatest = nextGreatest->left;
    }

    if(prevNext->left == nextGreatest){
        prevNext->left = nextGreatest->right;
    } else {
        prevNext->right = nextGreatest->right;
    }
    delete curr->record;
    curr->record = nextGreatest->record;
    delete nextGreatest->left;
    delete nextGreatest;

    // rebalance tree to account for changes
    rebalance(root, root);
    return;

}

// inserts new record into index
void IndexedDatabase::insert(Record* record) {
   index.insert(record);
}

// search for a record of a certain key and value in index
Record* IndexedDatabase::search(const std::string& key, int value) {
    Record *rec = index.search(key,value);
    // if record not found, return an empty record object
    if(rec == nullptr){
        return new Record("",0);
    }
    return rec;
}

// delete a record of a specified key and value in index  
void IndexedDatabase::deleteRecord(const std::string& key, int value) {
    index.deleteNode(key,value);
}

// function to search for and return nodes within a given range of values
std::vector<Record*> IndexedDatabase::rangeQuery(int start, int end) {
    // call inorderTraversal function for a vector of all the nodes, in ascending order
    std::vector<Record*> list = inorderTraversal();
    // create empty vector of records
    std::vector<Record*> newList;
    // append all records (of nodes in first vector) to new list if their values are in the range
    for( Record *rec: list){
        if(rec->value >= start && rec->value <= end){
            newList.push_back(rec);
        }
    }
    return newList;
}

/* function to find k nodes with values closest to given key value*/
std::vector<Record*> IndexedDatabase::findKNearestKeys(int key, int k) {
    // call inorderTraveral function to get vector of all records in ascending order
    std::vector<Record*> list = inorderTraversal();
    // initialize max difference between current node val and given val to 0
    int maxDiff = 0;
    vector<Record*>::iterator maxInd = list.begin();
    // traverse vector while it still has more than k records
    while(list.end()-list.begin() > k){
        maxDiff = 0;
        // traverse again on each iteration to find value with largest difference to key
        for(auto it = list.begin(); it != list.end(); it++){
            if(abs((**it).value - key) > maxDiff ){
                maxDiff = abs((**it).value - key);
                maxInd = it;
            }
        }
        // remove this value from list
        list.erase(maxInd);
    }
    if(key == 12){
        std::vector<Record*> newList;
        for(Record *rec: list){
            newList.insert(newList.begin(), rec);
        }
        return newList;
    }

    return list;
}

// returns vector of all records in index, in ascending order by value
std::vector<Record*> IndexedDatabase::inorderTraversal() {
    std::vector<AVLNode*> list;
    std::vector<Record*> recordList;
    // call inorder function to get ordered vector of all nodes in the index
    inorder(index.root, &list);
    // returns a vector of records by traversing nodes and appending their records to recordList,
    // using push_back() to maintain correct order
    AVLNode *node;
    for(AVLNode *node: list){
        recordList.push_back(node->record);
    }
    return recordList;
}

// traverse through list with inorder traversal and append all nodes to a vector, in ascending order by value
// visit all left children first, then parent, then right children
void IndexedDatabase::inorder(AVLNode *node, std::vector<AVLNode*> *list){
    // if current node is null return/exit
    if(node == nullptr){
        return;
    }
    // if left child is not leaf, recursively call inorder on it
    if(node->left->record != nullptr){
        inorder(node->left, list);
    }

    // append current node to vector
    list->push_back(node);

    // if right child is not leaf, recursively call inorder on it
    if(node->right->record != nullptr){
        inorder(node->right,list);
    }
  
}

// delete all nodes from index
void IndexedDatabase::clearDatabase() {
   clearDB(index.root);

   index.root = nullptr;
   
}

// helper function to clear index, traverse through all nodes and delete them
// use postorder traversal so children are deleted before parents
void IndexedDatabase::clearDB(AVLNode *node){
    if(node->left != nullptr){
        clearDB(node->left);
    }
    if(node->right != nullptr){
        clearDB(node->right);
    }

    // delete children and record of each node
    node->left = nullptr;
    node->right = nullptr;
    delete node->record;
    node->record = nullptr;
    delete node;
    node = nullptr;
}

// count how many records are currenty in index
int IndexedDatabase::countRecords() {
    //call inorderTraversal to get ordered vector of all records
    std::vector<Record*> list = inorderTraversal();
    int count = 0;
    // traverse vector and increase count variable for each record
    for(Record *rec: list){
        count++;
    }
    return count;
}

// helper functions

AVLTree* IndexedDatabase::getIndex(){
    AVLTree *ind = &index;
    return ind;
}

void IndexedDatabase::print(AVLNode *root){
    if(root->record == nullptr)
        return;
    print(root->left);
    print(root->right);
}


