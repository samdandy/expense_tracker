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
            cout << "Expenses added." << endl;
            save_expenses_to_file(expenses);
        }
        else if (operation == "total") {
            cout << "Calculating total expenses..." << endl;
            string file_name = enter_file_name();
            total_expenses(file_name);
        }
        else if (operation == "average") {
            cout << "Calculating average expense..." << endl;
            string file_name = enter_file_name();
            average_expense(file_name);
        }
        else if (operation == "max") {
            cout << "Finding maximum expense..." << endl;
            string file_name = enter_file_name();
            max_expense(file_name);
        }
        else if (operation == "report") {

            cout << "Generating report..." << endl;
            string file_name = enter_file_name();
            run_report(file_name);
        }
        else {
            cout << "Invalid operation." << endl;
        }
    }
    
    return 0;
}