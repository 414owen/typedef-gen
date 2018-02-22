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
vector<string> dump;
// [name, alias]
vector<pair<string, string>> prims;
// [name, arity, alias]
vector<tuple<string, int, string>> types;

vector<string> spl(string s, string delims) {
	vector<string> res;
	boost::split(res, s, boost::is_any_of(delims));
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
			if (i != strs.size() - 2) {
				dump.push_back(line);
				continue;
			}
			prims.push_back({type, strs.back()});
		} else if (strs.size() == 2) {
			// template
			vector<string> strl = spl(strs[0], "<>");
			if (strl.size() != 2) {
				dump.push_back(line);
				continue;
			}
			vector<string> strt = spl(strl[1], "-");
			if (strl.size() > 2) {
				dump.push_back(line);
			}
			int start = stoi(strt[0]), end;
			if (strt.size() == 1) end = start;
			else end = stoi(strt[1]);
			for (int i = start; i <= end; i++) {
				types.push_back({strl[0], i, strs[1]});
			}
		} else if (strs.size() != 0) {
			dump.push_back(line);
		}
		line_n++;
	}
}

pair<string, string> new_type(vector<int> inds, tuple<string, int, string> t,
		vector<pair<string, string>> prev, bool full) {
	string name = get<0>(t);
	int arity = get<1>(t);
	string alias = get<2>(t);
	string newname = name + '<';
	string newalias = alias;
	for (int j = 0; j < arity - 1; j++) {
		newname += (full ? get<0>(prev[inds[j]]) : get<1>(prev[inds[j]])) + ',';
		newalias += get<1>(prev[inds[j]]);
	}
	newname += (full ? get<0>(prev[inds.back()]) : get<1>(prev[inds.back()])) + '>';
	newalias += get<1>(prev[inds.back()]);
	return {newname, newalias};
}

vector<pair<string, string>> next_layer(vector<pair<string, string>> prev,
		bool full) {
	vector<pair<string, string>> next;

	for (auto el : types) {
		vector<int> inds(get<1>(el));
		while (true) {
			next.push_back(new_type(inds, el, prev, full));
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
	for (string s : dump) (outfile ? out : cout) << s << '\n';
	vector<pair<string, string>> prev = prims;
	for (int i = 0; i < layers; i++) {
		prev = next_layer(prev, i == 0);
		for (auto a : prev)
			(outfile ? out : cout) << "typedef " <<
				get<0>(a) << ' ' << get<1>(a) << ';' << '\n';
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
