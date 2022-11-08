//////////////////////////////////////////////////////////////////////////////////////////
//  Module: Parallele und verteilte Algorithmen
//  Created by Michael Althoff on 06.10.22.
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//  Aufgabe 3: „Lambda“-Variante: wie 2. jedoch unter Ausnutzung von Lambda-Ausdrücken statt Funktionsobjekten
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <time.h>
#include <algorithm>
#include <functional>

//Ausgaben der queue
template<typename T>
void print_queue(T q) {
	while (!q.empty()) {
		std::cout << q.top() << std::endl;
		q.pop();
	}
}

//Lamda-Funktion zum Füllen des Vektors mit Zufallsbuchstaben
auto CreateRandomLetters = []() -> char {
		int r;
		int r2;
		char c;
		r = rand() % 26;
		r2 = rand() % 2;
		if (r2 % 2 == 0) {
			c = 'a' + r;
			return c;
		}
		else {
			c = 'A' + r;
			return c;
		}
	};

//Ausgaben der queue
auto print = [](char a) -> void {
		std::cout << a << std::endl;
	};

auto Compare = [](char a, char b) {
		if (a > 91 & b < 91) { return ((a - 32) > b); }
		else if (a > 91 & b > 91) { return ((a - 32) > (b - 32)); }
		else if (a < 91 & b > 91) { return (a > (b - 32)); }
		else { return (a > b); }
	};

using namespace std;

int main()
{
	double time1 = 0.0, tstart;
	int n;
	cout << "Hello, Char World!" << endl;
	cout << "Enter the number of characters: ";
	cin >> n;

	//Vektor anlegen
	vector<char> v(n);

	//Zufällige Buchstaben erzeugen + Zeit messen
	srand(time(NULL));
	tstart = clock();
	generate(v.begin(), v.end(), CreateRandomLetters);
	time1 += clock() + tstart;
	time1 /= CLOCKS_PER_SEC;
	cout << "Fill vector used time was: " << time1*1000 << "ms" << endl;
	cout << "Vector" << endl;
	for_each(v.begin(), v.end(), print);

	//Sortieren der Buchstaben + Zeit messen
	priority_queue<char, vector<char>, decltype(Compare)> sortedLetters(Compare);
	tstart = clock();
	for (char c : v)
		sortedLetters.push(c);
	time1 += clock() + tstart;
	time1 /= CLOCKS_PER_SEC;

	cout << "Fill queue used time was: " << time1*1000 << "ms" << endl;
	cout << "Queue" << endl;

	print_queue(sortedLetters);

	    cout << "Fill queue used time was: " << time1*1000 << "ms" << endl;
    cout << "Queue" << endl;
	return 0;
}
