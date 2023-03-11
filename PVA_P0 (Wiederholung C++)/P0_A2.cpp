//////////////////////////////////////////////////////////////////////////////////////////
//  Module: Parallele und verteilte Algorithmen
//  Created by Michael Althoff on 06.10.22.
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//  Aufgabe 2: Auslassung von Schleifenanweisungen und Verwendung der Algorithmen und Funktionsobjekten.
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>

// Überladener Operator zur vereinfachten Ausgabe
class Functor {
public:
    void operator()(char a) {
        std::cout << a << '\n';
    }
};

// Überladener Operator für Nicht-Berücksichtigung von Groß- und Kleinbuchstaben
class Compare {
public:
    bool operator()(char a, char b) {
        return std::tolower(a) > std::tolower(b);
    }
};

// Füllen eines Vektors mit Zufallsbuchstaben
char generate_random_char() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 25);

    return static_cast<char>(dist(gen) + 'A');
}

template<typename T>
void print_queue(T q) {
    while (!q.empty()) {
        std::cout << q.top() << '\n';
        q.pop();
    }
}

int main()
{
    Functor print;

    int n;

    std::cout << "Hello, Char World!\n";
    std::cout << "Enter the number of characters: ";

    std::cin >> n;

    // VECTOR
    std::vector<char> v(n);

    auto start = std::chrono::high_resolution_clock::now();
    std::generate(v.begin(), v.end(), generate_random_char);
    auto end = std::chrono::high_resolution_clock::now();
    double time1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1e6;

    std::cout << "Fill vector used time was: " << time1 << "s\n";
    std::cout << "Vector:\n";
    for_each(v.begin(), v.end(), print);

    // PRIORITY QUEUE
    std::priority_queue<char, std::vector<char>, Compare> sortedLetters;

    start = std::chrono::high_resolution_clock::now();
    for (char c : v)
        sortedLetters.push(c);
    end = std::chrono::high_resolution_clock::now();
    double time2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1e6;

    std::cout << "Fill queue used time was: " << time2 << "s\n";
    std::cout << "Queue:\n";

    print_queue(sortedLetters);

    return 0;
}
