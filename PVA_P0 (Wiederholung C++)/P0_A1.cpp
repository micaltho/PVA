//////////////////////////////////////////////////////////////////////////////////////////
//  Module: Parallele und verteilte Algorithmen
//  Created by Michael Althoff on 06.10.22.
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//  Aufgabe 1: „Klassische“ Variante: mit Schleifenanweisungen (for, while, do) ohne Verwendung der Algorithmen aus der Standardbibliothek
//////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

//Überladener Operator für nicht-Berücksichtigung von Groß- und Kleinbuchstaben
struct Compare {
    bool operator()(char a, char b) const {
        return std::tolower(a) < std::tolower(b);
    }
};

//Funktion zum Füllen eines Vektors mit Zufallsbuchstaben
void CreateRandomLetters(int n, std::vector<char>& v) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 25);

    for (int i = 0; i < n; i++) {
        char c = dis(gen) + (dis(gen) % 2 == 0 ? 'a' : 'A');
        v.push_back(c);
    }
}

//Ausgabe Vektor
void ShowVector(const std::vector<char>& v) {
    for (const auto& c : v) {
        std::cout << c << '\n';
    }
}

//Buchstaben sortieren Vektor
void SortVector(std::vector<char>& v) {
    std::sort(v.begin(), v.end(), Compare());
}

//Ausgabe sortierte Buchstaben
void ShowSortedVector(const std::vector<char>& v) {
    for (const auto& c : v) {
        std::cout << c << '\n';
    }
}

int main() {
    int n;
    std::cout << "Hello, Char World!" << '\n';
    std::cout << "Enter the number of characters: ";
    std::cin >> n;

    std::vector<char> v;
    auto t1 = std::chrono::high_resolution_clock::now();
    CreateRandomLetters(n, v);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Fill vector used time was: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << "ms" << '\n';
    std::cout << "Vector" << '\n';
    ShowVector(v);

    t1 = std::chrono::high_resolution_clock::now();
    SortVector(v);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Sort vector used time was: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << "ms" << '\n';
    std::cout << "Sorted Vector" << '\n';
    ShowSortedVector(v);

    return 0;
}
