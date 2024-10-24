
#include "NotationConverter.hpp"
#include "NotationConverter.cpp"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int main()

{
  const string infix1 = "(A + B) ";
  const string infix2 = "((X + B) * (Y - D))";
  const string infix3 = "(((A + B) / (X + Y)) - R)";

  const string prefix1 = "+ / * x y g h";
  const string prefix2 = "-    /  x  y  g"; // Multiple Spaces Between Letters and Operators
  const string prefix3 = "- / x y * a b";

  const string postfix1 = "X Y + A B + *";
  const string postfix2 = "V C +";
  const string postfix3 = "H W * R Q - /";

  NotationConverter nc;

  assert(nc.infixToPrefix(infix1) == "+ A B");
  cout<<"Test 1 passed"<<endl;
  assert(nc.infixToPrefix(infix2) == "* + X B - Y D");
  cout<<"Test 2 passed"<<endl;
  assert(nc.infixToPrefix(infix3) == "- / + A B + X Y R");
  cout<<"Test 3 passed"<<endl;


  assert(nc.infixToPostfix(infix1) == "A B +");
  cout<<"Test 4 passed"<<endl;
  assert(nc.infixToPostfix(infix2) == "X B + Y D - *");
  cout<<"Test 5 passed"<<endl;
  assert(nc.infixToPostfix(infix3) == "A B + X Y + / R -");
  cout<<"Test 6 passed"<<endl;


  assert(nc.prefixToPostfix(prefix1) == "x y * g / h +");
  cout<<"Test 7 passed"<<endl;
  assert(nc.prefixToPostfix(prefix2) == "x y / g -");
  cout<<"Test 8 passed"<<endl;
  assert(nc.prefixToPostfix(prefix3) == "x y / a b * -");
  cout<<"Test 9 passed"<<endl;


  assert(nc.prefixToInfix(prefix1) == "(((x * y) / g) + h)");
  cout<<"Test 10 passed"<<endl;
  assert(nc.prefixToInfix(prefix2) == "((x / y) - g)");
  cout<<"Test 11 passed"<<endl;
  assert(nc.prefixToInfix(prefix3) ==  "((x / y) - (a * b))");
  cout<<"Test 12 passed"<<endl;


  assert(nc.postfixToInfix(postfix1) =="((X + Y) * (A + B))");
  cout<<"Test 13 passed"<<endl;
  assert(nc.postfixToInfix(postfix2) =="(V + C)");
  cout<<"Test 14 passed"<<endl;
  assert(nc.postfixToInfix(postfix3) =="((H * W) / (R - Q))");
  cout<<"Test 15 passed"<<endl;


  assert(nc.postfixToPrefix(postfix1) =="* + X Y + A B");
  cout<<"Test 16 passed"<<endl;
  assert(nc.postfixToPrefix(postfix2) =="+ V C");
  cout<<"Test 17 passed"<<endl;
  assert(nc.postfixToPrefix(postfix3) =="/ * H W - R Q");
  cout<<"Test 18 passed"<<endl;
  
  cout<<("All test cases passed!")<<endl;


}

