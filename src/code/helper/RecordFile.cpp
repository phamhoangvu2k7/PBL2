#include "../../include/helper/RecordFile.h"
#include <cstdio>
#include <fstream>

string RecordFile::escape(const string& field) {
    string out;
    for (char c : field) {
        if (c == '\\') out += "\\\\";
        else if (c == '|') out += "\\p";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else out += c;
    }
    return out;
}

string RecordFile::unescape(const string& field) {
    string out;
    for (size_t i = 0; i < field.size(); i++) {
        if (field[i] == '\\' && i + 1 < field.size()) {
            char n = field[++i];
            out += (n == 'p') ? '|' : (n == 'n') ? '\n' : (n == 'r') ? '\r' : n;
        } else {
            out += field[i];
        }
    }
    return out;
}

string RecordFile::join(const vector<string>& fields) {
    string out;
    for (size_t i = 0; i < fields.size(); i++) {
        if (i > 0) out += "|";
        out += escape(fields[i]);
    }
    return out;
}

vector<string> RecordFile::split(const string& line) {
    vector<string> fields;
    string cur;
    for (char c : line) {
        if (c == '|') {
            fields.push_back(unescape(cur));
            cur.clear();
        } else {
            cur += c;
        }
    }
    fields.push_back(unescape(cur));
    return fields;
}

bool RecordFile::read(const string& path, vector<vector<string>>& records) {
    ifstream f(path);
    if (!f.is_open()) return false;
    string line;
    while (getline(f, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        records.push_back(split(line));
    }
    return true;
}

bool RecordFile::write(const string& path, const vector<string>& lines) {
    string tmp = path + ".tmp";
    {
        ofstream f(tmp);
        if (!f.is_open()) return false;
        for (const string& l : lines) f << l << "\n";
        f.flush();
        if (!f.good()) return false;
    }
    remove(path.c_str());
    return rename(tmp.c_str(), path.c_str()) == 0;
}
