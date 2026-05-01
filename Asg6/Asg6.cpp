//============================================================================
// Name        : Asg6.cpp
// Author      : Chung-Chih Li
// Version     :
// Copyright   : Your copyright notice
// Description : For IT279, Asg6, Spring 2026
//============================================================================

#include <iostream>
#include <fstream>
#include "BinomialQueue.h"
#include "BinomialQueue.cpp"
using namespace std;

int queueSize=20; 

void readIn(istream * fin, BinomialQueue<int> * BQ) {
		int num;
		while (true) {
			*fin >> num;
			if (fin->eof()) break;
			BQ->add(num);
		}
}


int main(int n, char *arg[]) {
	BinomialQueue<int> *BQ = new BinomialQueue<int>(queueSize);

	if (n==2) {
		ifstream fin;
		fin.open(arg[1]);
		readIn(&fin, BQ);
	} else
		readIn(&cin, BQ);
	BQ->print();

	int size=BQ->size();

	cout << "\nSize = " << size << endl;

	while (BQ->size() > size/2)
		BQ->remove();

	cout << endl;
	BQ->print();

	cout << "\nSize = " << BQ->size() << endl;

	cout << "\nRemove all: ";
	while (BQ->size() != 0)
		cout <<	BQ->remove() << " ";
	cout << endl << endl ;
	cout << "\nPrint empty queue:\n";
	BQ->print();
	cout << "\nThe End\n";

	delete BQ;
	BQ = nullptr;

	return 0;
}
