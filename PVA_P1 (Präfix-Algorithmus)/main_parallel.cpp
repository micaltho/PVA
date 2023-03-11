/////////////////////////////////////////////////
//  main_parallel.cpp
//  Created by Michael Althoffon 13.10.21.
////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <execution>

std::vector<std::string> tokenize(const std::string& str, const char delim)
{
    std::vector<std::string> tokens;

    auto start = str.begin();
    auto end = str.begin();

    while (end != str.end())
    {
        end = std::find(start, str.end(), delim);
        tokens.push_back(std::string(start, end));
        start = std::next(end);
    }

    return tokens;
}

int main()
{
    // read data as string
    std::ifstream text("karlmay.txt");

    if (!text.is_open())
    {
        std::cerr << "Could not open the file" << std::endl;
        return 1;
    }

    std::stringstream ss;
    ss << text.rdbuf();
    std::string result = ss.str();

    //begin with tokenize an stop time
    auto t1 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> tokens = tokenize(result, ' ');
    auto t2 = std::chrono::high_resolution_clock::now();

    //calculate time 
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    
    std::cout << "Time taken: " << ms.count() << " ms" << std::endl;

    std::ofstream file("parallel.txt");
    file << ms.count();
    file.close();
    
    return 0;
}

