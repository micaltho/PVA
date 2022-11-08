//////////////////////////////////////////////////////////////////////////////////////////
//  Module: Parallele und verteilte Algorithmen
//  Created by Michael Althoff on 06.10.22.
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//  Aufgabe 1: „Klassische“ Variante: mit Schleifenanweisungen (for, while, do) ohne Verwendung der Algorithmen aus der Standardbibliothek
//////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <time.h>
#include <algorithm>

using namespace std;


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

//Funktion zum Füllen eines Vektors mit Zufallsbuchstaben
void CreateRandomLetters(int n_1, char &c_1, int &r_1, int &r2_1, vector<char> &v_1){
    for (int i = 0; i<n_1; i++)
    {
        r_1 = rand() % 26;
        r2_1 = rand() % 2;
        if(r2_1 % 2 == 0) {
          c_1 = 'a' + r_1;
          v_1.push_back(c_1);
        }
        else {
          c_1 = 'A' + r_1;
          v_1.push_back(c_1);
        }
    }
}

//Ausgabe Vektor
void ShowVector(vector<char> v_1){
    for(int i = 0; i < v_1.size(); i++) {
    cout << v_1.at(i) << endl;
    }
}

//Buchstaben sortieren Vektor
void SortVector(priority_queue<char, vector<char>, Compare> &sortedLetters_1, vector<char> v_1 ){
    for(int i = 0; i < v_1.size(); i++) {
        sortedLetters_1.push(v_1.at(i));
    }
}

//Ausgabe sortierte Buchstaben 
void ShowSortedVector(priority_queue<char, vector<char>, Compare> &sortedLetters_1, vector<char> v_1 ){
    for(int i = 0; i < v_1.size(); i++) {
        cout << sortedLetters_1.top() << endl;
        sortedLetters_1.pop();
    }
}

int main()
{
    double time1 = 0.0, tstart;

    int n;
    cout << "Hello, Char World!" << endl;
    cout << "Enter the number of characters: ";
    cin >> n;

    vector<char> v;
    char c;
    int r, r2;

    srand (time(NULL));
 
    //Zufällige Buchstaben erzeugen + Zeit messen
    tstart = clock();
    CreateRandomLetters(n, c, r, r2, v);
    time1 += clock() + tstart;
    time1 /= CLOCKS_PER_SEC;
    cout << "Fill vector used time was: " << time1*1000 << "ms" << endl;
    cout << "Vector" << endl;

    //Ausgabe der zufälligen Buchstaben
    ShowVector(v);

    //Sortieren der Buchstaben + Zeit messen
    priority_queue<char, vector<char>, Compare> sortedLetters;
    tstart = clock();
    SortVector(sortedLetters, v);
    time1 += clock() + tstart;
    time1 /= CLOCKS_PER_SEC;
    cout << "Fill queue used time was: " << time1*1000 << "ms" << endl;
    cout << "Queue" << endl;

    //Ausgabe der sortierten Buchstaben
    ShowSortedVector(sortedLetters, v);
    cout << "Fill queue used time was: " << time1*1000 << "ms" << endl;
    cout << "Queue" << endl;
    return 0;
}
