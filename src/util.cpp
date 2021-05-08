//
// Created by Centauria V. CHEN on 2021/5/7.
//

#include "util.h"
#include "platform.h"
#include "termcolor/termcolor.hpp"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <cstring>
#include <fstream>
#include <openssl/md5.h>
#include <boost/algorithm/string.hpp>

using namespace std;

string md5_file(const string &file_name) {
    ifstream file(file_name.c_str(), ifstream::binary);
    if (!file) {
        throw runtime_error("File " + file_name + " does not exist");
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);

    char buf[1024 * 16];
    while (file.good()) {
        file.read(buf, sizeof(buf));
        MD5_Update(&ctx, buf, file.gcount());
    }

    unsigned char result[MD5_DIGEST_LENGTH] = {0};
    MD5_Final(result, &ctx);

    char hex[33] = {0};
    memset(hex, 0x00, sizeof(hex));
    for (unsigned char i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hex + 2 * i, "%02x", result[i]);
    }
    auto md5_string = string(hex);

    return md5_string;
}

string exec(const string &cmd) {
    array<char, 128> buffer{};
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

vector<string> locate(const string &filename) {
    vector<string> result;
    auto output = exec("locate \"*/" + filename + "\"");
    boost::split(result, output, boost::is_any_of(LINESEP), boost::token_compress_on);
    result.erase(remove(result.begin(), result.end(), ""), result.end());
    return result;
}

void prompt(Color color) {
    cout << color << endl;
}

void pprint_vector(const string &info, const vector<string> &vec) {
    cout << termcolor::green << info << termcolor::reset << " {" << endl;
    for (const auto &r : vec) {
        cout << r << endl;
    }
    cout << "}" << endl;
}

bool contains(const vector<string> &list, const string &item) {
    return find(list.begin(), list.end(), item) != list.end();
}

bool contains(const vector<string> &list, const vector<string> &item_list) {
    return all_of(item_list.begin(), item_list.end(), [&list](const string &item) { return contains(list, item); });
}