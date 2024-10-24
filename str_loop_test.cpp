#include <iostream>
#include <string>

using namespace std;

int main() {

    string str = "hello";

    int i;
    for(i=0; str[i]!='\0'; i++){
        cout<<str[i]<<endl;
    }


    return 0;
}