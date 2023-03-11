//////////////////////////////////////////////////////////////////////////////////////////
//  Module: Parallele und verteilte Algorithmen
//  Created by Michael Althoff on 06.10.22.
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//  Aufgabe 3: „Lambda“-Variante: wie 2. jedoch unter Ausnutzung von Lambda-Ausdrücken statt Funktionsobjekten
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

// Ausgabe der queue
template<typename T>
void print_queue(T q) {
    while (!q.empty()) {
        cout << q.top() << endl;
        q.pop();
    }
}

// Lambda-Funktion zum Erstellen von Zufallsbuchstaben
auto create_random_letter = []() -> char {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 51);
    const string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return letters[dis(gen)];
};

// Vergleichsfunktion für die queue
auto compare_letters = [](char a, char b) {
    if (isupper(a) && islower(b)) { return a > (b - 'a' + 'A'); }
    else if (isupper(a) && isupper(b)) { return a > b; }
    else if (islower(a) && isupper(b)) { return (a - 'a' + 'A') > b; }
    else { return a > b; }
};

int main() {
    int n;
    cout << "Hello, Char World!" << endl;
    cout << "Enter the number of characters: ";
    cin >> n;

    // Vektor anlegen und mit Zufallsbuchstaben füllen
    vector<char> v(n);
    auto tstart = chrono::high_resolution_clock::now();
    generate(v.begin(), v.end(), create_random_letter);
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tstart).count();
    cout << "Fill vector used time was: " << elapsed << "ms" << endl;
    cout << "Vector" << endl;
    for_each(v.begin(), v.end(), [](char a) { cout << a << endl; });

    // Buchstaben sortieren und in eine queue einfügen
    priority_queue<char, vector<char>, decltype(compare_letters)> sorted_letters(compare_letters);
    tstart = chrono::high_resolution_clock::now();
    for (char c : v)
        sorted_letters.push(c);
    elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tstart).count();
    cout << "Fill queue used time was: " << elapsed << "ms" << endl;
    cout << "Queue" << endl;
    print_queue(sorted_letters);

    return 0;
}
