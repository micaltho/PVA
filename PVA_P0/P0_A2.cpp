//////////////////////////////////////////////////////////////////////////////////////////
//  Module: Parallele und verteilte Algorithmen
//  Created by Michael Althoff on 06.10.22.
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//  Aufgabe 2: „Funktionale“ Variante: Auslassung von Schleifenanweisungen und Verwendung der Algorithmen (beispielsweise std::generate, std::for_each, ...) und Funktionsobjekten.
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <time.h>
#include <algorithm>

//Überladener Operator zur vereinfachten Ausgabe
class Functor {
public:
	void operator()(char a) {
		std::cout << a << std::endl;
	}
};

//Überladener Operator für nicht-Berücksichtigung von Groß- und Kleinbuchstaben
class Compare {
public:
	bool operator()(char a, char b) {
		if (a > 91 & b < 91) { return ((a - 32) > b); }
		else if (a > 91 & b > 91) { return ((a - 32) > (b - 32)); }
		else if (a < 91 & b > 91) { return (a > (b - 32)); }
		else { return (a > b); }
	}
};

//Füllen eines Vektors mit Zufallsbuchstaben
char fill_vector() {
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
}

template<typename T>
void print_queue(T q) {
	while (!q.empty()) {
		std::cout << q.top() << std::endl;
		q.pop();
	}
}

using namespace std;

int main()
{
	Functor print;

	double time1 = 0.0, tstart;
	int n;

	cout << "Hello, Char World!" << endl;
	cout << "Enter the number of characters: ";

	cin >> n;

	//VECTOR
	vector<char> v(n);

	srand(time(NULL));
	tstart = clock();
	generate(v.begin(), v.end(), fill_vector);
	time1 += clock() + tstart;
	time1 /= CLOCKS_PER_SEC;

	cout << "Fill vector used time was: " << time1 << "s" << endl;
	cout << "Vector" << endl;
	for_each(v.begin(), v.end(), print);

	// PRIORITY QUEUE
	priority_queue<char, vector<char>, Compare> sortedLetters;

	tstart = clock();
	for (char c : v)
		sortedLetters.push(c);
	time1 += clock() + tstart;
	time1 /= CLOCKS_PER_SEC;

	cout << "Fill queue used time was: " << time1 << "s" << endl;
	cout << "Queue" << endl;

	print_queue(sortedLetters);

	cout << "Fill queue used time was: " << time1 << "s" << endl;
    cout << "Queue" << endl;
	
	return 0;
}
