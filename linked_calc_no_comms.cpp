#include "linked_calc.hpp"
using namespace std;

// Default constructor definition
template <typename T>
LinkedCalc<T>::LinkedCalc() : head(nullptr) {}

// Destructor to deallocate memory
template <typename T>
LinkedCalc<T>::~LinkedCalc() {
    Node<T>* current = head;
    Node<T>* nextNode;
    while(current != nullptr){
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

// Function to insert a new node at the end of the linked list
template <typename T>
void LinkedCalc<T>::insert(const T& value) {
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Helper function to determine if a character is a digit
template <typename T>
bool LinkedCalc<T>::isDigit(const T& c) {
    return (c >= '0' && c <= '9');

}

// mod function from Geeks4Geeks
// https://www.geeksforgeeks.org/modulus-two-float-double-numbers/

template <typename T>
float LinkedCalc<T>::findMod(float a, float b){
    float mod;
    if (a < 0){
        mod = -a;
    }
    else{
        mod =  a;
    }
    if (b < 0){
        b = -b;
    }
    while (mod >= b){
        mod = mod - b;
    }
    if (a < 0){
        return -mod;
    }
    return mod;
}


template <typename T>
bool LinkedCalc<T>::validateExpression() {
    if(head == NULL){
        return false;
    }

    if(head->data == '+'|| head->data == '-' || head->data == '*' || head->data == '/' || head->data == '%'){
        return false;
    }

    Node<T> *prev, *curr;
    int count;
    for(prev=head,curr=head; curr!= NULL; curr=curr->next){
        if(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/'){
            count = 0;
            for(; prev!=curr; prev=prev->next){
                if(prev->data == '.'){
                    count++;
                }
            }
            if(count > 1){
                return false;
            }
        }
    }
    if(curr == NULL){
        count =0;
        for(; prev!=curr; prev=prev->next){
            if(prev->data == '.'){
                count++;
            }
        }
        if(count > 1){
            return false;
        }
    }

    for(prev=NULL,curr=head; curr!=NULL; prev=curr,curr=curr->next){
        if(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/'){
            if(prev->data == '+' || prev->data == '-' || prev->data == '*' || prev->data == '/'){
                return false;
            }
        }

        if(!(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/' || curr->data == '.' || isDigit(curr->data))){
            return false;
        }

        if(curr->next == NULL){
            if(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/'){
                return false;
            }
        }
    }
    
    return true;
   
}

// Function to evaluate the expression represented by the linked list
template <typename T>
float LinkedCalc<T>::evaluateExpression() {
    if(!validateExpression()){
        return -1;
    }
    
    float result = 0;
    //while list is not empty
    while(isOpsLeft(head)){
        Node<T> *highestOp = head;
        for(Node<T> *curr = head; curr != NULL; curr=curr->next){
            if(!(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/')){
                continue;
            }
            if(isGreaterOp(curr,highestOp)){
                highestOp = curr;
            }
        }

        Node<T> *prevNum = head;
        for(Node<T> *curr = head; curr != highestOp; curr=curr->next){
            if(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/' ){
                prevNum = curr->next;
            }
        }

        Node<T> *nextNum = highestOp->next;
        result = doOperation(highestOp, returnEntireNum(prevNum), returnEntireNum(nextNum));
        cout<<"result: "<<result<<endl;


        Node<T> *afterOp;
        Node<T> *beforeOp;
        //set afterOp equal to next (consecutive) operator
        for(afterOp = nextNum; afterOp != NULL; afterOp=afterOp->next){
            if(afterOp->data == '+' || afterOp->data == '-' || afterOp->data == '*' || afterOp->data == '/'){
                break;
            }
        }
    
        for(beforeOp = head; beforeOp != NULL && beforeOp != prevNum && beforeOp->next != prevNum; beforeOp=beforeOp->next);

        storeResult(result, beforeOp, afterOp);

    }

    cout<<"made it to end of evaluateExpression, return value is: "<<returnEntireNum(head)<<endl;
    return returnEntireNum(head);

}

template <typename T>
bool isOpsLeft(Node<T> *list_head){
    Node<T> *temp;
    for(temp = list_head; temp!=NULL; temp=temp->next){
        if(temp->data == '+' || temp->data == '-' || temp->data == '*' || temp->data == '/' ){
            return true;
        }
    }
    return false;
}

template <typename T>
void LinkedCalc<T>::storeResult(float result, Node<T> *firstNode, Node<T> *lastNode){
    Node<T> *temp = NULL;

    //check if firstNode is head of list
    while(firstNode->next != lastNode){
        temp = firstNode->next->next;
        delete firstNode->next;
        firstNode->next = temp;
    }

    if(result == 0.0){
        Node<T>* newNode = new Node<T>('0');
        firstNode->next = newNode;
        newNode->next = lastNode;
        return;
    }

    float decimalPart = findMod(result,1.0f);
    float integerPart = result - decimalPart;
   
    if(decimalPart != 0){
        temp = firstNode;
        float num = decimalPart;

        Node<T>* newNode = new Node<T>('.');
        temp->next = newNode;
        newNode->next = lastNode;
        temp = temp->next;
        
        while(num > 0){
            num *= 10;
            Node<T>* newNode = new Node<T>(num-findMod(num+0.0f,1.0f)+48);
            temp->next = newNode;
            newNode->next = lastNode;
            temp = temp->next;
            num = findMod(num,1.0f);
        }
    }
    if(integerPart != 0){
        float num = integerPart;
        while(num > 0){
            Node<T>* newNode = new Node<T>(findMod(num +0.0f ,10.0f)+48);
            temp = firstNode->next;
            firstNode->next = newNode;
            newNode->next = temp;
            num = num - findMod(num+0.0f, 10.0f);
            num /= 10;
        }
    }

    if(firstNode == head){
        head = head->next;
        delete firstNode;
    }
    return;
}

template <typename T>
float doOperation(Node<T> *op, float leftNum, float rightNum){
    if(op->data == '+'){
        return leftNum + rightNum;
    }
    else if(op->data == '-'){
        return leftNum - rightNum;
    }
    else if(op->data == '*'){
        return leftNum * rightNum;
    }
    else{
        return leftNum / rightNum;
    }
}


template <typename T>
bool isGreaterOp(const Node<T> *curr, const Node<T> *currHighest){
    if(!(currHighest->data == '+' || currHighest->data == '-'|| currHighest->data == '*' || currHighest->data == '/')){
        return true;
    }
    if(currHighest->data == '-' && (curr->data == '+' || curr->data == '*' || curr->data == '/' )){
        return true;
    }
    else if(currHighest->data == '+' && (curr->data == '*' || curr->data == '/' )){
        return true;
    }
    else if(currHighest->data == '*' && curr->data == '/' ){
        return true;
    }
    return false;
}

template <typename T>
float convertToFloat(Node<T> *current){
    float sum = current->data + 0.0;
    if(sum >= 10)
        sum -= 48;
    return sum;
}

template <typename T>
float returnDecimal(Node<T> *decimal_point){
    int divisor = 10;
    float total = 0;
    Node<T> *right = decimal_point->next;
    while(right != NULL && right->data != '+' && right->data != '-' && right->data != '*' && right->data != '/'){
        total += convertToFloat(right)/divisor;
        divisor *= 10;
        right = right->next;
    }
    return total;
}


template <typename T>
float returnEntireNum(Node<T> *msd){

    float total = 0;

    while(msd != NULL && msd->data != '+' && msd->data != '-' && msd->data != '*' && msd->data != '/' && msd->data != '.'){
        total = total*10 + convertToFloat(msd);
        if(msd->next == NULL){
            return total;
        }
        msd = msd->next;
    }
    if( msd->data == '.'){
        total += returnDecimal(msd);
    }
    return total;
}

