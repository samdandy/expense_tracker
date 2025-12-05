#ifndef EXPENSE_H
#define EXPENSE_H
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

fstream file;
string header_line;

class Expense { 
    private:
        string name;
        string date;
        double amount;
    public:
        Expense(const string& expense_name, const string& expense_date, double expense_amount) {
            name = expense_name;
            date = expense_date;
            amount = expense_amount;
        };
    
    public:
        string getName() const { return name; }
        string getDate() const { return date; }
        double getAmount() const { return amount; }     
};


void add_file_header(const string& file_name) {
        file.open(file_name, ios::out | ios::app);
        getline(file, header_line);
        if (header_line == "Name,Date,Amount") {
            file.close();
            return;
        }
        if (!file.is_open()) {
                    throw runtime_error("Failed to open file");
                }
        file << "Name,Date,Amount" << endl;
        file.close();
    }

void add_to_file(const string& file_name, const Expense& expense) {
        file.open(file_name, ios::out | ios::app);
        if (!file.is_open()) {
                    throw runtime_error("Failed to open file");
                }
        file << expense.getName() << "," << expense.getDate() << "," << expense.getAmount() << endl;
        file.close();
    }

double parse_amount(const string& line) {
    size_t pos1 = line.find(",");
    size_t pos2 = line.find(",", pos1 + 1);
    string amount_str = line.substr(pos2 + 1);
    return stod(amount_str);
}

string enter_file_name() {
        string file_name;
        cout << "Enter file name: ";
        cin >> file_name;
        return file_name;
}


void total_expenses() {
        string file_name = enter_file_name();
        file.open(file_name, ios::in);
                if (!file.is_open()) {
                    throw runtime_error("Failed to open file");
                }
        getline(file, header_line);
        double total = 0.0;
        while (!file.eof()) {
            string line;
            getline(file, line);
            if (!line.empty()) {
                total += parse_amount(line);
            }
        }
        file.close();
        cout << "Total expenses: $" << total << endl;
    }


void add_expense_prompt(vector<Expense>& expenses) {
        while (true) {
            string name;
            string date;
            double amount;
            cout << "Enter expense name (or 'done' to finish): ";
            cin >> name;
            if (name == "done") {   
                break;
            }
            cout << "Enter expense date: ";
            cin >> date;
            cout << "Enter expense amount: ";
            cin >> amount;
            Expense new_expense(name, date, amount);
            expenses.push_back(new_expense);
        }
}   

void save_expenses_to_file(vector<Expense>& expenses) {
        string file_name = enter_file_name();
        add_file_header(file_name);
        for (Expense& exp: expenses) {
            cout << "Expense: " << exp.getName() << ", Date: " << exp.getDate() << ", Amount: $" << exp.getAmount() << endl;
            add_to_file(file_name, exp);
        }
        expenses.clear();
}


#endif 


