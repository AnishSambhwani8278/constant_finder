#include <bits/stdc++.h>
using namespace std;

long long decodeValue(int base, const string &valueStr) {
    long long result = 0;
    for (char c : valueStr) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = toupper(c) - 'A' + 10;
        else continue;
        result = result * base + digit;
    }
    return result;
}

long long computeConstantTerm(int k, const vector<pair<long long,long long>> &points) {
    long double secret = 0.0;
    for (int i = 0; i < k; i++) {
        long double term = points[i].second;
        for (int j = 0; j < k; j++) {
            if (i == j) continue;
            term *= (long double)(-points[j].first) / (points[i].first - points[j].first);
        }
        secret += term;
    }
    return llround(secret);
}

vector<pair<long long,long long>> parseCase(const string &caseContent, int &k) {
    vector<pair<long long,long long>> points;

    regex kRegex("\"k\"\\s*:\\s*(\\d+)");
    smatch match;
    if (regex_search(caseContent, match, kRegex)) {
        k = stoi(match[1]);
    } else {
        throw runtime_error("Could not find k");
    }

    regex pointRegex("\"(\\d+)\"\\s*:\\s*\\{[^}]*?\"base\"\\s*:\\s*\"(\\d+)\"[^}]*?\"value\"\\s*:\\s*\"([^\"]+)\"");
    auto begin = sregex_iterator(caseContent.begin(), caseContent.end(), pointRegex);
    auto end = sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        long long x = stoll((*it)[1]);
        int base = stoi((*it)[2]);
        string valueStr = (*it)[3];
        long long y = decodeValue(base, valueStr);
        points.push_back({x, y});
    }

    return points;
}

vector<string> splitCases(const string &content) {
    vector<string> cases;
    int depth = 0;
    string current;

    for (char c : content) {
        if (c == '{') {
            depth++;
            current.push_back(c);
        } else if (c == '}') {
            depth--;
            current.push_back(c);
            if (depth == 0) {
                cases.push_back(current);
                current.clear();
            }
        } else if (depth > 0) {
            current.push_back(c);
        }
    }

    if (!current.empty()) cases.push_back(current);
    return cases;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./constant_finder <input.json>\n";
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Could not open file\n";
        return 1;
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    try {
        auto cases = splitCases(content);
        if (cases.empty()) {
            cerr << "No valid cases found\n";
            return 1;
        }

        for (size_t idx = 0; idx < cases.size(); idx++) {
            int k = 0;
            auto points = parseCase(cases[idx], k);
            if ((int)points.size() < k) {
                cerr << "Case " << idx + 1 << " has insufficient points\n";
                continue;
            }

            vector<pair<long long,long long>> selected(points.begin(), points.begin() + k);
            long long c = computeConstantTerm(k, selected);
            cout << "Case " << idx + 1 << ": " << c << "\n";
        }
    } catch (exception &e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
