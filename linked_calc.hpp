#ifndef LINKED_CALC_HPP
#define LINKED_CALC_HPP

#include <iostream>

// Node structure
template <typename T>
struct Node {
    T data;
    Node* next;

    Node(const T& data) : data(data), next(nullptr) {}
};

// LinkedCalc class
template <typename T>
class LinkedCalc {
public:
    LinkedCalc();
    ~LinkedCalc();
    void insert(const T& value);
    bool validateExpression();
    float evaluateExpression();

private:
    Node<T>* head;
    bool isDigit(const T& c);
    float convertToFloat(Node<T>*& current);
    float findMod(float a, float b);
    void storeResult(float result, Node<T> *firstNode, Node<T> *lastNode);
};

#endif // LINKED_CALC_HPP

