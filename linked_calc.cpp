/*
Chaunique O'Garro U25723732
Chiara Deangelis U42447641

This programs can compute arithmetic expressions (if they are valid). This program allows for the creation of a linked list and for 
characters to be inserted into it (one character per node). Once characters have been inserted, it can check if they form a valid 
arithmetic expression. If they do, it can evaluate the expression and return the resulting value.
*/


#include "linked_calc.hpp"


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
    return (c >= '0' && c <= '9') ;
}

// Mod function from Geeks4Geeks
// https://www.geeksforgeeks.org/modulus-two-float-double-numbers/
// Finds Modulus for floats
template <typename T>
float LinkedCalc<T>::findMod(float a, float b){
    float mod;
    // Handling negative values
    if (a < 0){
        mod = -a;
    }
    else{
        mod =  a;
    }
    if (b < 0){
        b = -b;
    }
    // Finding mod by repeated subtraction
    while (mod >= b){
        mod = mod - b;
    }
    // Sign of result typically depends
    // on sign of a.
    if (a < 0){
        return -mod;
    }
    return mod;
}



//Checks to make sure insert expression is a valid arithmetic expression
/*If a number contains more than one decimal, if operators are in places that they shouldn't be (at the beginning of expression, 
at the end, after another operator), if the expression contains characters that aren't numbers or operators or decimals, or if the
expression is empty, the function will return false. Otherwise it will return true.
*/
template <typename T>
bool LinkedCalc<T>::validateExpression() {
    
    //Checks if expression is empty
    if (head == nullptr){
        return false;
    }

    //Checks if expression starts with an operator
    if(head->data == '+'|| head->data == '-' || head->data == '*' || head->data == '/'){
        return false;
    }

    //Checks if any number has more than one decimal in it
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
    if (curr == NULL){
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
    
    //Check if there are consecutive operators, if any character is not an operator/number/decimal, and if expression end with operator
    for(prev=NULL,curr=head; curr!=NULL; prev=curr,curr=curr->next){
        //Consecutive operator check
        if(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/'){
            if(prev->data == '+' || prev->data == '-' || prev->data == '*' || prev->data == '/'){
                return false;
            }
        }

        //Checks for illegal characters
        if(!(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/' || curr->data == '.' || isDigit(curr->data))){
            return false;
        }

        //Checks if expression end with an operator
        if(curr->next == NULL){
            if(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/'){
                return false;
            }
        }
    }
    
    //Returns true if all checks passed
    return true;
}

// Function to evaluate the expression represented by the linked list
template <typename T>
float LinkedCalc<T>::evaluateExpression() {
    //Exits funtion if expression is invalid
    if(!validateExpression()){
        return -1;
    }
    
    /*Finds the operator with the most precedence, does the operation, edits the list to replace the sub-expression with the result,
    repeats until there are no more operators
    */
    float result = 0;
    while(isOpsLeft(head)){
        
        //Finds the operator with the highest precedence
        Node<T> *highestOp = head;
        for(Node<T> *curr = head; curr != NULL; curr=curr->next){
            if(!(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/')){
                continue;
            }
            if(isGreaterOp(curr, highestOp)){
                highestOp = curr;
            }
        }

        //Finds the number before the operator with the highest precedence
        Node<T> *prevNum = head;
        for(Node<T> *curr = head; curr != highestOp; curr=curr->next){
            if(curr->data == '+' || curr->data == '-' || curr->data == '*' || curr->data == '/' ){
                prevNum = curr->next;
            }
        }
        //Finds the number after the operator with the hightest precedence
        Node<T> *nextNum = highestOp->next;
        

        //Calculates the value of the sub-expression and stores it in result
        result = doOperation(highestOp, returnEntireNum(prevNum), returnEntireNum(nextNum));
        

        //Finds the nodes before and after the sub-expression
        Node<T> *afterOp;
        Node<T> *beforeOp;
        for(afterOp = nextNum; afterOp != NULL; afterOp=afterOp->next){
            if(afterOp->data == '+' || afterOp->data == '-' || afterOp->data == '*' || afterOp->data == '/'){
                break;
            }
        }
        for(beforeOp = head; beforeOp != prevNum && beforeOp->next != prevNum; beforeOp=beforeOp->next){};

        //Replaces the sub-expression with result
        storeResult(result, beforeOp, afterOp);
    }


    //Returns the value of the whole expression
    return returnEntireNum(head);

}

//Checks if there are any operators left in the list. Returns true if there is an operator in the list.
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

//Modifies the list so that nodes between firstNode and lastNode are deleted and replaced with new nodes representing result
template <typename T>
void LinkedCalc<T>::storeResult(float result, Node<T> *firstNode, Node<T> *lastNode){

    //Deletes nodes in between firstNode and lastNode, links firstNode directly to lastNode
    Node<T> *temp = NULL;
    while(firstNode->next != lastNode){
        temp = firstNode->next->next;
        delete firstNode->next;
        firstNode->next = temp;
    }

    //Inserts a node containing the value 0 if result's value is 0. Returns.
    if(result == 0.0){
        Node<T>* newNode = new Node<T>('0');
        firstNode->next = newNode;
        newNode->next = lastNode;
        return;
    }

    //Splits the result into an integer part and a decimal part (integer and decimal digits are insert into list separately)
    float decimalPart = findMod(result,1.0f);
    float integerPart = result - decimalPart;
    
    //Inserts decimal digits into list
    if(decimalPart != 0){
        temp = firstNode;
        float num = decimalPart;

        //Inserts a decimal (.) into the list
        Node<T>* newNode = new Node<T>('.');
        temp->next = newNode;
        newNode->next = lastNode;
        temp = temp->next;
        
        /*Makes a new node for each digit and inserts it to the right of the last one. (Order of digit converstion: start at decimal, 
        work right)
        */
        while(num > 0){
            num *= 10;
            Node<T>* newNode = new Node<T>(num-findMod(num,1.0f)+48);
            temp->next = newNode;
            newNode->next = lastNode;
            temp = temp->next;
            num = findMod(num,1.0f);
        }
    }

    //Insert whole number digits into the list
    if(integerPart != 0){
        float num = integerPart;

        /*Makes a new node for each digit and inserts it the the left of the last one. (Order of digit converstion: start at decimal, 
        work left)
        */
        while(num > 0){
            Node<T>* newNode = new Node<T>(findMod(num +0.0f ,10.0f)+48);
            temp = firstNode->next;
            firstNode->next = newNode;
            newNode->next = temp;
            num = num - findMod(num+0.0f, 10.0f);
            num = num/10;
        }
    }

    /*If the sub-expression is at the beginning of the list, firstNode will be pointing to the start of first number in the 
    sub-espression instead of before it. The following deletes the first node if that is the case.
    */
    if(firstNode == head){
        head = head->next;
        delete firstNode;
    }

    
    return;
}


/*Takes two floats and a pointer to a node containing an operator character. Does the correct arithmetic based on what the operator is.
Returns result.
*/
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

//Compares two operator characters to see which one has the higher precedence. Returns true if curr's operator has higher precedence
template <typename T>
bool isGreaterOp(const Node<T> *curr, const Node<T> *currHighest){
    //Returns true if currHighest contains a number
    if(!(currHighest->data == '-' || currHighest->data == '+' || currHighest->data == '*' || currHighest->data == '/')){
        return true;
    }

    //Returns true if curr's operator has higher precedence
    if(currHighest->data == '-' && (curr->data == '+' || curr->data == '*' || curr->data == '/' )){
        return true;
    }
    else if(currHighest->data == '+' && (curr->data == '*' || curr->data == '/' )){
        return true;
    }
    else if(currHighest->data == '*' && curr->data == '/' ){
        return true;
    }

    //Returns false if currHighest's operator has higher precedence
    return false;
}

//Takes a pointer to a node that contains a character. Converts that character to a float and returns the float.
template <typename T>
float convertToFloat(Node<T> *current){
    float sum = current->data + 0.0;
    if(sum >= 10)
        sum -= 48;
    return sum;
}

//Converts a decimal number from a node/set of nodes into a float and returns the float
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

//Converts a number from a node/set of nodes into a float and returns the float
template <typename T>
float returnEntireNum(Node<T> *msd){
    float total = 0;

    //Converts the whole number digits into float
    while(msd != NULL && msd->data != '+' && msd->data != '-' && msd->data != '*' && msd->data != '/' && msd->data != '.'){
        total = total*10 + convertToFloat(msd);
        msd = msd->next;
    }

    //If the number contains a decimal, call returnDecimal function and add result to total
    if (msd != NULL){
        if(msd->data == '.'){
            total += returnDecimal(msd);
        }
    }

    //Returns the entire number as a float
    return total;
}

