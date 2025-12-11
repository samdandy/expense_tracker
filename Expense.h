#ifndef EXPENSE_H
#define EXPENSE_H
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <sstream>

using namespace std;

mutex file_mutex;  

class Expense { 
private:
    string name;
    string date;
    double amount;
    
public:
    Expense(const string& expense_name, const string& expense_date, double expense_amount) 
        : name(expense_name), date(expense_date), amount(expense_amount) {}
    
    string get_name() const { return name; }
    string get_date() const { return date; }
    double get_amount() const { return amount; }     
};

void add_file_header(const string& file_name) {
    fstream file;
    string header_line;
    
    file.open(file_name, ios::in);
    if (file.is_open()) {
        getline(file, header_line);
        file.close();
        if (header_line == "Name,Date,Amount") {
            return; 
        }
    }
    
    file.open(file_name, ios::out | ios::app);
    if (!file.is_open()) {
          cout << "Failed to open file for writing" << endl;
        return;
    }
    file << "Name,Date,Amount" << endl;
    file.close();
}

void add_to_file(const string& file_name, const Expense& expense) {
    lock_guard<mutex> lock(file_mutex);
    fstream file;
    
    file.open(file_name, ios::out | ios::app);
    if (!file.is_open()) {
          cout << "Failed to open file for writing" << endl;
        return;
    }
    file << expense.get_name() << "," << expense.get_date() << "," << to_string(expense.get_amount()) << endl;
    file.close();
}

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
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

void total_expenses(const string& file_name) {
    lock_guard<mutex> lock(file_mutex);
    fstream file;
    string header_line;
    
    file.open(file_name, ios::in);
    if (!file.is_open()) {
          cout << "Failed to open file for reading" << endl;
        return;
    }
    
    getline(file, header_line);
    double total = 0.0;
    string line;
    
    while (getline(file, line)) {  
        if (!line.empty()) {
            total += parse_amount(line);
        }
    }
    
    file.close();
    cout << "Total expenses: $" << total << endl;
}

void average_expense(const string& file_name) {
    lock_guard<mutex> lock(file_mutex);
    fstream file;
    string header_line;
    
    file.open(file_name, ios::in);
    if (!file.is_open()) {
          cout << "Failed to open file for reading" << endl;
        return;
    }
    
    getline(file, header_line);
    double total = 0.0;
    int count = 0;
    string line;
    
    while (getline(file, line)) {
        if (!line.empty()) {
            total += parse_amount(line);
            count++;
        }
    }
    
    file.close();
    
    if (count > 0) {
        cout << "Average expense: $" << (total / count) << endl;
    } else {
        cout << "No expenses found." << endl;
    }
}

void max_expense(const string& file_name) {
    lock_guard<mutex> lock(file_mutex);
    fstream file;
    string header_line;
    
    file.open(file_name, ios::in);
    if (!file.is_open()) {
          cout << "Failed to open file for reading" << endl;
        return;
    }
    
    getline(file, header_line);
    double max_amount = 0.0;
    string line;
    
    while (getline(file, line)) {
        if (!line.empty()) {
            double amount = parse_amount(line);
            if (amount > max_amount) {
                max_amount = amount;
            }
        }
    }
    
    file.close();
    cout << "Maximum expense: $" << max_amount << endl;
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
        if (!(cin >> amount)) {
            cout << "Invalid amount! Restart expense entry with a valid number.\n";
            cin.clear();                 
            cin.ignore(10000, '\n');        
            continue;                      
        }
        expenses.emplace_back(name, date, amount);  
    }
}   

void save_expenses_to_file(vector<Expense>& expenses) {
    string file_name = enter_file_name();
    add_file_header(file_name);
    
    for (const Expense& exp : expenses) { 
        cout << "Expense: " << exp.get_name() << ", Date: " << exp.get_date() 
             << ", Amount: $" << exp.get_amount() << endl;
        add_to_file(file_name, exp);
    }
    expenses.clear();
}

void run_report(const string& file_name) { 
    vector<thread> report_threads;
    
    report_threads.emplace_back(total_expenses, ref(file_name));
    report_threads.emplace_back(average_expense, ref(file_name));
    report_threads.emplace_back(max_expense, ref(file_name));
    
    for (thread& t : report_threads) {
        t.join();
    }
}

void update_expense(vector<Expense>& expenses, const string& file_name){
    fstream file;
    file.open(file_name, ios::in);
    if (!file.is_open()) {
        cout << "Failed to open file for reading" << endl;
        return;
    }
    
    string header_line;
    vector<string> updated_lines;
    
    getline(file, header_line);
    updated_lines.push_back(header_line); 
    
    string line;
    while (getline(file, line)) {
        vector<string> vals = split(line, ',');
        
        if (vals.size() == 3) {  
            string name = vals[0];
            string date = vals[1];
            
            bool found = false;
            for (const Expense& exp : expenses) {
                if (exp.get_name() == name && exp.get_date() == date) {
                    line = name + "," + date + "," + to_string(exp.get_amount());
                    break;
                }
            }
        }
        
        updated_lines.push_back(line);
    }
    file.close();
    
   
    file.open(file_name, ios::out);
    if (!file.is_open()) {  
        throw runtime_error("Failed to open file for writing");
    }
    
    for (const string& updated_line : updated_lines) {
        file << updated_line << endl;
    }
    file.close();
}

void delete_expense(const string& file_name, vector<Expense>& expenses){
    fstream file;
    file.open(file_name, ios::in);
    if (!file.is_open()) {
        cout << "Failed to open file for reading" << endl;
        return;
    }
    
    string header_line;
    vector<string> updated_lines;
    
    getline(file, header_line);
    updated_lines.push_back(header_line); 
    
    string line;
    while (getline(file, line)) {
        vector<string> vals = split(line, ',');
        
        if (vals.size() == 3) {  
            string name = vals[0];
            string date = vals[1];
            
            bool to_delete = false;
            for (const Expense& exp : expenses) {
                if (exp.get_name() == name && exp.get_date() == date) {
                    to_delete = true;
                    break;
                }
            }
            if (to_delete) {
                continue; 
            }
        }
        
        updated_lines.push_back(line);
    }
    file.close();
    
   
    file.open(file_name, ios::out);
    if (!file.is_open()) {  
        cout << "Failed to open file for writing" << endl;
        return;
    }
    
    for (const string& updated_line : updated_lines) {
        file << updated_line << endl;
    }
    file.close();
}


void max_task(){
   cout << "Finding maximum expense..." << endl;
            string file_name = enter_file_name();
            max_expense(file_name);
}
void add_task(){
    vector<Expense> expenses;
    cout << "Adding expenses..." << endl;
            add_expense_prompt(expenses);
            cout << "Expenses added." << endl;
            save_expenses_to_file(expenses);
}
void update_task(){
    vector<Expense> expenses;
    cout << "Updating expenses..." << endl;
            add_expense_prompt(expenses);
            string file_name = enter_file_name();
            update_expense(expenses, file_name);
            cout << "Expenses updated." << endl;
}

void total_task(){
    cout << "Calculating total expenses..." << endl;
            string file_name = enter_file_name();
            total_expenses(file_name);
}

void average_task(){
    cout << "Calculating average expense..." << endl;
            string file_name = enter_file_name();
            average_expense(file_name);
}

void report_task(){
    cout << "Generating expense report..." << endl;
            string file_name = enter_file_name();
            run_report(file_name);
}

void delete_task(){
    vector<Expense> expenses;
    cout << "Deleting expenses..." << endl;
            add_expense_prompt(expenses);
            string file_name = enter_file_name();
            delete_expense(file_name, expenses);
            cout << "Expenses deleted." << endl;
}

void main_task(){
    while (true) {
        cout << "Please enter a operation: ";
        string operation;
        cin >> operation;
        if (operation == "exit" || operation == "quit") {
            cout << "Goodbye!" << endl;
            break; 
        }
        else if (operation == "add"){
            add_task();
        }
        else if (operation == "update") {
            update_task();
        }
        else if (operation == "delete") {
            delete_task();
        }
        else if (operation == "total") {
            total_task();
        }
        else if (operation == "average") {
           average_task();
        }   
        else if (operation == "max") {
            max_task();
        }
        else if (operation == "report") {
            report_task();
        }
        else {
            cout << "Invalid operation." << endl;
        }
    }
}

#endif