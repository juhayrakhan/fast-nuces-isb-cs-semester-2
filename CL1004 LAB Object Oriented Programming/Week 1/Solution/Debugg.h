// Juhayra Khan 25i-0922 F

#pragma once
#include<iostream>
using namespace std;
void calculator(char o, float num1, float num2)
{

    float result;

    switch (o)
    {
    case '+':
        result = num1 + num2;
        num1 = num1 * 10;
        cout << num1 << " + " << num2 << " = " << result << endl;
        break; // num1 = 1000, num2 = 6, result = 106
    case '-':
        result = num1 + num2;
        num2 = num2 + 10;
        cout << num1 << " - " << num2 << " = " << result << endl;

        break; // num1 = 15, num2 = 13, result = 18
    case '*':
        result = num1 * num2;
        num1 = num1 - 10;
        cout << num1 << " * " << num2 << " = " << result << endl;

        break; //  num1 = -5, num2 = 6, result = 30
    case '/':
        result = num1 / num2;
        num2 = num2 - 10;
        cout << num1 << " / " << num2 << " = " << result << endl;

        break; //  num1 = 25, num2 = -5, result = 5
    default:
        cout << "Error! operator is not correct";
        break;
    }
}