/////////////////////////////////////////////////
//  main_sequentiell.cpp
//  Created by Michael Althoffon 13.10.21.
////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;

vector<string> tokenize(string str, char delim)
{
    vector<string> result;
    stringstream ss(str);
    string word;
    while (getline(ss, word, delim)) {
        result.push_back(word);
    }
    return result;
}

int main()
{
    ifstream text("karlmay.txt");
    if (!text.is_open()) {
        cerr << "Could not open the file " << endl;
        return 1;
    }
    stringstream ss;
    ss << text.rdbuf();
    string result = ss.str();
    text.close();

    auto t1 = std::chrono::high_resolution_clock::now();
    vector<string> tokens = tokenize(result, ' ');
    auto t2 = std::chrono::high_resolution_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    cout << "Sequentiell: " << ms.count() << " ms" << endl;

    ofstream file("sequentiell.txt");
    if (!file.is_open()) {
        cerr << "Could not create output file" << endl;
        return 1;
    }
    file << ms.count();
    file.close();

    return 0;
}
