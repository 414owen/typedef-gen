#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

bool infile = false, outfile = false, eof = false;
ifstream in;
ofstream out;

string get() {
	string line;
	eof |= !getline(infile ? in : cin, line);
	return line;
}

void put(string s) {
	(outfile ? out : cout) << s;
}

void run() {
	do {
		string line = get();
		vector<std::string> strs;
		boost::split(strs, line, boost::is_any_of("\t "));
	} while (!eof);
}

int main(int argc, char *argv[]) {
	infile = argc > 1;
	outfile = argc > 2;
	if (argc > 1) in.open(argv[1]);
	if (argc > 2) out.open(argv[2]);
	run();
}

