//
// Created by Centauria V. CHEN on 2021/5/7.
//

#ifndef PHOENIXKILLER_UTIL_H
#define PHOENIXKILLER_UTIL_H

#include "termcolor/termcolor.hpp"

#include <iostream>
#include <string>
#include <vector>

typedef std::ostream &(*Color)(std::ostream &);

using namespace std;

string md5_file(const string &);

string exec(const string &cmd);

vector<string> locate(const string &filename);

void prompt(Color color);

template<typename T, typename... Ts>
void prompt(Color color, T info, Ts... args) {
    cout << color << info;
    prompt(termcolor::reset, args...);
}

void pprint_vector(const string &info, const vector<string> &vec);

bool contains(const vector<string> &list, const string &item);

bool contains(const vector<string> &list, const vector<string> &item_list);

#endif //PHOENIXKILLER_UTIL_H
