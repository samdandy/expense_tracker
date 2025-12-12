#ifndef UTILS_H
#define UTILS_H
#include <string>

#include <vector>

#include <sstream>

#include <iostream>

using namespace std;
string to_lower(const string & str) {
  string lower_str = str;
  for (char & c: lower_str) {
    c = tolower(c);
  }
  return lower_str;
}

vector < string > split(const string & str, char delimiter) {
  vector < string > tokens;
  stringstream ss(str);
  string token;

  while (getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

double parse_amount(const string & line) {
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

#endif