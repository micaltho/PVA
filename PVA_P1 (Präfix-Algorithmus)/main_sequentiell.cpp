/////////////////////////////////////////////////
//  main_sequentiell.cpp
//  Created by Michael Althoffon 13.10.21.
////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <numeric>
#include <execution>

using namespace std;

vector<string> tokenize(string str, char delim)
{
    vector<string> result;
    vector<int> char_index;
    vector<int> indizes;
    vector<int> partial;
    vector<int> partial_2;
    string word;

    char_index.resize(str.size());
    indizes.resize(str.size());

    transform(str.begin(), str.end(), char_index.begin(), [&](auto &c)
              { return (c != delim) ? 1 : 0; });

    transform(str.begin(), str.end(), indizes.begin(), [](auto &i)
              { return 1; });

    partial.resize(str.size());
    
    partial_2.resize(str.size());
    exclusive_scan(char_index.begin(), char_index.end(), partial.begin(), 0);
    exclusive_scan(indizes.begin(), indizes.end(), partial_2.begin(), 0);

    result.resize(str.size());
    word.resize(str.size());
    for_each(partial_2.begin(), partial_2.end(), [&](auto &i)
             {
                 if (char_index.at(i) == 1)
                 {
                     word.push_back(str.at(i));
                 }
                 else
                 {  
                     result.push_back(word);
                     word = "";
                 }
             });
    return result;
}

int main()
{

    // read data as string
    ifstream text("karlmay.txt");
    stringstream ss;

    if (!text.is_open())
    {
        cerr << "Could not open the file " << std::endl;
    }

    ss << text.rdbuf();
    string result = ss.str();

    //begin with tokenize an stop time
    auto t1 = std::chrono::high_resolution_clock::now();
    vector<string> tokens = tokenize(result, ' ');
    auto t2 = std::chrono::high_resolution_clock::now();

    //calculate time 
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
/*
    for_each(tokens.begin(), tokens.end(), [&](auto token)
             { cout << token << endl; });
*/       
    cout << "Sequentiell: " << ms.count() << " ms" << endl;

    std::ofstream file("sequentiell.txt");
    file << ms.count();
    file.close();
}
