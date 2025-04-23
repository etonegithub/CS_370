#include <iostream>
#include <string>
using namespace std;

int main() {
    // declare vars
    string input = "";
    int first_break_index, second_break_index;
    double num1, num2;
    char op;


    while (getline(cin, input) && input != "exit") {
        // find whitespaces in input
        first_break_index = input.find(" ");
        second_break_index = first_break_index + 2;

        // split input by whitespaces
        num1 = stod(input.substr(0, first_break_index));
        op = input.substr(first_break_index + 1, 1)[0];
        num2 = stod(input.substr(second_break_index + 1));
        
        // perform operation
        switch(op) {
            case '+':
                cout << num1 + num2 << endl;
                break;
            case '-':
                cout << num1 - num2 << endl;
                break;
            case '*':
                cout << num1 * num2 << endl;
                break;
            case '/':  
                cout << num1 / num2 << endl;
                break;
        }
    }
}