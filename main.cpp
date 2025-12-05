#include <iostream>
#include <vector>
#include "Expense.h"

using namespace std;

int main () {
    vector<Expense> expenses;
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
            add_expense_prompt(expenses);
            save_expenses_to_file(expenses);
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