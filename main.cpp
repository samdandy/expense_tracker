#include <iostream>
#include <vector>
#include "Expense.h"

using namespace std;

int main () {
    vector<Expense> expenses;
    vector<Expense>* expenses_ptr = &expenses;
    while (true) {
        cout << "Please enter a operation: ";
        string operation;
        cin >> operation;
        if (operation == "exit" || operation == "quit") {
            cout << "Goodbye!" << endl;
            break; 
        }
        else if (operation == "add"){
            
            cout << "Adding expenses..." << endl;
            add_expense_prompt(expenses_ptr);
            cout << "Expenses added." << endl;
            cout << expenses_ptr << endl;
            save_expenses_to_file(expenses_ptr);
        }
        else if (operation == "total") {
            cout << "Calculating total expenses..." << endl;
            total_expenses();
        }
        else {
            cout << "Invalid operation." << endl;
        }
    }
    
    return 0;
}