/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: Bryan
 * This file is the starter project for the Random Writer problem.
 */

#include <iostream>
#include <fstream>
#include <string>
#include "simpio.h"
#include "map.h"
#include "vector.h"
#include "random.h"
using namespace std;

string getFile();
Map<string, Vector<char> > GetMap(string filename, int sd);
string RandomWritter(Map<string, Vector<char> > map);

int SEED;

int main() {
    string filename = getFile();
	SEED = getInteger("Please input seed (1-10): ");
	Map<string, Vector<char> > myMap = GetMap(filename, SEED);
	string myText = RandomWritter(myMap);
	cout << myText << endl;
    return 0;
}

// Get file name from user input
string getFile() {
	string filename;
	while (true) {
		filename = getLine("Please input the filename: ");
		ifstream infile;
		infile.open(filename.c_str());
		if (infile.fail())
			cout << "Cannot open file!" << endl;
		else
			break;
	}
	return filename;
}

// Create Map from file
Map<string, Vector<char> > GetMap(string filename, int sd) {
	Map<string, Vector<char> > markov;
	string sample;
	char ch;
	ifstream inp;
	inp.open(filename.c_str());
	// Read file char by char
	while (inp.get(ch))
		sample = sample + ch;
	unsigned index = 0;
	while (index <= (sample.length()-sd)) {
		string key = sample.substr(index, sd);
		Vector<char> value;
		// If the map already contains the key, update its associated vector
		// and overwrite previous map record
		if (markov.containsKey(key)) {
			value = markov.get(key);
			value.add(sample[index + sd]);
			markov.put(key, value);
		}
		// If the map does not contain the key, insert it
		else {
			value.add(sample[index + sd]);
			markov.put(key, value);
		}
		index++;
	}
	return markov;
}

// Create new passage based on the map
string RandomWritter(Map<string, Vector<char> > map) {
	Map<string, Vector<char> >::iterator itMap;
	string initial, result;
	int max = 0;
	
	// Find the key that appears most frequently to start with
	for (itMap = map.begin(); itMap != map.end(); itMap++) {
		if (map.get(*itMap).size() > max) {
			initial = *itMap;
			max = map.get(*itMap).size();
		}
	}
	result = initial;
	string keynow = initial;
	// Create new passage until meets the key where its associated vector is empty
	// or the length of the result is over 2,000 chars
	while (true) {
		Vector<char> vec = map.get(keynow);
		if (vec.isEmpty() || result.length() == 2000)
			break;
		int randomIndex = randomInteger(0, vec.size()-1);
		char nextCh = vec.get(randomIndex);
		result += nextCh;
		keynow = result.substr(result.length()-SEED, SEED);
	}
	return result;
}