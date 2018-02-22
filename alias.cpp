#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

bool infile = false, outfile = false;
ifstream in;
ofstream out;

int layers = 2;
// [name, alias]
vector<pair<string, string>> prims;
// [name, arity, alias]
vector<tuple<string, int, string>> types;

void err(size_t line, string s) {
	cerr << "Error on line: " << line << " " << s << endl;
	exit(1);
}

vector<string> spl(string s, string delims) {
	vector<string> res;
	boost::split(res, s, boost::is_any_of(delims));
	// remove_if(res.begin(), res.end(), empty_str);
	res.erase(std::remove(res.begin(), res.end(), ""), res.end());
	return res;
}

void interpret() {
	string line;

	size_t line_n = 0;
	while (getline(infile ? in : cin, line)) {
		vector<std::string> strs = spl(line, " ");
		if (strs.size() && strs[0] == "//") continue;
		if (strs.size() > 2) {
			string type = strs[0];
			int i;
			for (i = 1; i < strs.size(); i++) {
				string s = strs[i];
				if (s == "->") break;
				type += " " + s;
			}
			if (i != strs.size() - 2) err(line_n, "unrecognised primitive format");
			prims.push_back({type, strs.back()});
		} else if (strs.size() == 2) {
			// template
			vector<string> strl = spl(strs[0], "<>");
			if (strl.size() != 2) err(line_n, "Unrecognized template format");
			vector<string> strt = spl(strl[1], "-");
			if (strl.size() > 2) err(line_n, "Unrecognized template range");
			int start = stoi(strt[0]), end;
			if (strt.size() == 1) end = start;
			else end = stoi(strt[1]);
			for (int i = start; i <= end; i++) {
				types.push_back({strl[0], i, strs[1]});
			}
		} else if (strs.size() != 0) {
			err(line_n, "unrecognised amount of tokens");
		}
		line_n++;
	}
}

pair<string, string> new_type(vector<int> inds, tuple<string, int, string> t,
		vector<pair<string, string>> prev) {
	string name = get<0>(t);
	int arity = get<1>(t);
	string alias = get<2>(t);
	string newname = name + '<';
	string newalias = alias;
	for (int j = 0; j < arity - 1; j++) {
		newname += get<1>(prev[inds[j]]) + ',';
		newalias += get<1>(prev[inds[j]]);
	}
	newname  += get<1>(prev[inds.back()]) + '>';
	newalias += get<1>(prev[inds.back()]);
	return {newname, newalias};
}

vector<pair<string, string>> next_layer(vector<pair<string, string>> prev) {
	vector<pair<string, string>> next;

	for (auto el : types) {
		vector<int> inds(get<1>(el));
		while (true) {
			next.push_back(new_type(inds, el, prev));
			bool overflow = true;
			for (int j = 0; j < get<1>(el); j++) {
				if (inds[j] + 1 < prev.size()) {
					inds[j]++;
					overflow = false;
					break;
				} else inds[j] = 0;
			}
			if (overflow) break;
		}
	}
	return next;
}

void run() {
	int layer = 0;
	vector<pair<string, string>> prev = prims;
	for (int i = 0; i < layers; i++) {
		prev = next_layer(prev);
		for (auto a : prev)
			(outfile ? out : cout) << "typedef " <<
				get<0>(a) << ' ' << get<1>(a) << ';' << endl;
	}
}

int main(int argc, char *argv[]) {
	infile = argc > 2;
	outfile = argc > 3;
	if (argc > 1) layers = stoi(argv[1]);
	if (argc > 2) in.open(argv[2]);
	if (argc > 3) out.open(argv[3]);
	interpret();
	run();
}
