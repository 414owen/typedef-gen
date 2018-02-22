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


void put(string s) {
	(outfile ? out : cout) << s << endl;
}

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

void run() {
	int layer = 0;

	// map<string, string> prev;
	// for (auto a : prims) map[get<0>(a)] = get<0>(a);

	vector<pair<string, string>> prev = prims;

	for (int i = 0; i < layers; i++) {
		vector<pair<string, string>> next;
		for (auto el : types) {
			string name = get<0>(el);
			int arity = get<1>(el);
			string alias = get<2>(el);

			vector<int> inds(arity);
			while (true) {
				bool overflow = true;
				for (int j = 0; j < arity; j++) {
					if (inds[j] + 1 < prev.size()) {
						inds[j]++;
						overflow = false;
						break;
					} else {
						inds[j] = 0;
					}
				}
				if (overflow) break;
				else {
					string newname = name + '<';
					string newalias = alias;
					for (int j = 0; j < arity - 1; j++) {
						newname += get<1>(prev[inds[j]]) + ',';
						newalias += get<1>(prev[inds[j]]);
					}
					newname  += get<1>(prev[inds.back()]) + '>';
					newalias += get<1>(prev[inds.back()]);
					next.push_back({newname, newalias});
					put("typedef " + newname + ' ' + newalias + ';');
				}
			}
		}
		prev = next;
	}
}

int main(int argc, char *argv[]) {
	infile = argc > 1;
	outfile = argc > 2;
	if (argc > 1) in.open(argv[1]);
	if (argc > 2) out.open(argv[2]);
	if (argc > 3) layers = stoi(argv[3]);
	interpret();
	run();
}
