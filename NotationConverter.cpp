#include "NotationConverter.hpp"
#include <iostream>
#include <string>
using namespace std;

/* class NotationConverter inherits from NotationConverterInterface, which contatins the declarations of
 6 conversion functions, head and tail pointers to access a deque, and push and pop function declarations for the deque */
class NotationConverter : public NotationConverterInterface {
public:
    /* takes in a postfix expression, and returns the expression in infix. 
    deque behaves as a stack */
    std::string postfixToInfix(std::string inStr) override {
        int i;
        // iterate through all characters of string, stop when string is empty
        for(i=0; inStr[i]!='\0'; i++){
            // skip whitespaces
            if(inStr[i] == ' '){
                continue;
            }
            // check if char is a letter (variable), and if so convert it into a string and add it to front of deque
            if((inStr[i] >= 'a' && inStr[i] <= 'z') || (inStr[i] >= 'A' && inStr[i] <= 'Z')){
                string s = "";
                s += inStr[i];
                pushFront(s);
            }
            /* if char is an operator, pop first two elements from deque (variables), build a new string 
            with operator in between them, and push that string back into the front of deque */
            else{
                string firstTop = popFront();
                string secondTop = popFront();
                string unit = "(" + secondTop + " " + inStr[i] + " " + firstTop + ")";
                pushFront(unit);
            }
        }
        // head->data should be the only node in deque and contain the input string in infix notation
        return head->data;
    }
    
    /* takes in a postfix expression and returns the expression in prefix.
    deque behaves as a stack */
    std::string postfixToPrefix(std::string inStr) override {
        int i;
        // iterate through all characters of string, stop when string is empty
        for(i=0; inStr[i]!='\0'; i++){
            // skip whitespaces
            if(inStr[i] == ' '){
                continue;
            }
            // check if char is a letter (variable), and if so convert it into a string and add it to front of deque
            if((inStr[i] >= 'a' && inStr[i] <= 'z') || (inStr[i] >= 'A' && inStr[i] <= 'Z')){
                string s = "";
                s += inStr[i];
                pushFront(s);
            }
            /* if char is an operator, pop first two elements from deque (variables), build a new string 
            with operator before them , and push that string back into the front of deque */
            else{
                string firstTop = popFront();
                string secondTop = popFront();
                string unit = "";
                unit += inStr[i];
                unit += " " + secondTop + " " + firstTop;
                pushFront(unit);
            }
        }
        // head->data should be the only node in deque and contain the input string in prefix notation
        return head->data;
    }

    /* takes in a infix expression, and returns the expression in postfix. 
    deque behaves as a stack */
    std::string infixToPostfix(std::string inStr) override {
        int i;
        // iterate through all characters of string, stop when string is empty
        for(i=0; inStr[i]!='\0'; i++){
            // skip whitespaces
            if(inStr[i] == ' '){
                continue;
            }
            /* if char is not a closing parentheses (it is an opening parentheses, var or operator), 
            convert it to a string and push it to the front of deque */
            if(inStr[i] != ')'){
                string s = "";
                s += inStr[i];
                pushFront(s);
            }
            /* if char is a closing parentheses, pop the first three values from the deque (var, operator, var),
            and pop but don't store the remaining first node (opening parentheses). Then build a new string with the
            two vars first and the operator last, and push it to the front of deque. */
            else{
                string firstTop = popFront();
                string secondTop = popFront();
                string thirdTop = popFront();
                popFront();
                string unit = "";
                unit += thirdTop + " " + firstTop + " " + secondTop;
                pushFront(unit);
            }
        }
        // head->data should be the only node in deque and contain the input string in postfix notation
        return head->data;
    }

    /* takes in a infix expression, and returns the expression in prefix by calling
    infixToPostfix() to convert it postfix then postFixToPrefix() to convert it to prefix. */
    std::string infixToPrefix(std::string inStr) override {
        string pstfix = infixToPostfix(inStr);
        return postfixToPrefix(pstfix);
    }
    /* takes in a prefix expression, and returns the expression in infix by calling
    prefixToPostfix() to convert it postfix then postfixToInfix() to convert it to infix. */
    std::string prefixToInfix(std::string inStr) override {
        string infxStr = prefixToPostfix(inStr);
        return postfixToInfix(infxStr);
    }

    /* takes in a prefix expression, and returns the expression in postfix. 
    deque is accessed from the back, but still acts as a stack (FILO). */
    std::string prefixToPostfix(std::string inStr) override {
        int idx=0, i;
        /* iterate through all chars of string to get idx equal to its length */
        for(idx=0; inStr[idx]!='\0'; idx++);
        /* increment backwards through list, starting at idx */
        for(i=idx-1; i>=0; i--){
            // skip whitespaces
            if(inStr[i] == ' '){
                continue;
            }
            // check if char is a letter (variable), and if so convert it into a string and add it to back of deque
            if((inStr[i] >= 'a'&& inStr[i] <= 'z') || (inStr[i] >= 'A' && inStr[i] <= 'Z')){
                string s = "";
                s += inStr[i];
                pushBack(s);
            }
            /* if char is an operator, pop two elements from the back of deque (variables), build a new string 
            with operator after them, and push that string to the back of deque */
            else{
                string firstTop = popBack();
                string secondTop = popBack();
                string unit = firstTop + " " + secondTop + " " + inStr[i];
                pushBack(unit);
            }
        }
        // head->data should be the only node in deque and contain the input string in postfix notation
        return tail->data;
    }

    // deque access function to push a value to the front
    void pushFront(string str){
        Node *newNode =  new Node(str);
        // if head is null, make head the new node
        if(head == nullptr){
            head = newNode;
            tail = newNode;
        // else, insert at front and adjust head node
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    //  // deque access function to push a value to the back
    void pushBack(string str){
        Node *newNode =  new Node(str);
        // if tail is null, make head the new node
        if(tail == nullptr){
            tail = newNode;
            head = newNode;
        // else, insert at back and adjust tail node
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // deque access function to remove head node
    string popFront(){
        // return empty string if head is null
        if(head == nullptr){
            return "";
        }
        string str = head->data;
        Node *temp = head;
        // if more than one node in deque, set second node to head & delete first one
        if(head->next != nullptr){
            head = head->next;
            head->prev = nullptr;
        // else set both head and tail to null, list is now empty
        } else {
            head = nullptr;
            tail = nullptr;
        }
        delete temp;
        return str;
    }

    // deque access function to remove tail node
    string popBack(){
        if(tail == nullptr){
            return "";
        }
        string str = tail->data;
        Node *temp = tail;
        // if more than one node in deque, set second to last node to tail & delete last one
        if(tail->prev != nullptr){
            tail = tail->prev;
            tail->next = nullptr;
        // else set both tail and head to null, list is now empty
        } else {
            tail = nullptr;
            head = nullptr;
        }
        delete temp;
        return str;
    }

};




