#include "../../include/helper/RecordFile.h"
#include <cstdio>
#include <fstream>

string RecordFile::ma_hoa(const string& field) {
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

string RecordFile::giai_ma(const string& field) {
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

string RecordFile::noi_cot(const vector<string>& fields) {
    string out;
    for (size_t i = 0; i < fields.size(); i++) {
        if (i > 0) out += "|";
        out += ma_hoa(fields[i]);
    }
    return out;
}

vector<string> RecordFile::tach_cot(const string& line) {
    vector<string> fields;
    string cur;
    for (char c : line) {
        if (c == '|') {
            fields.push_back(giai_ma(cur));
            cur.clear();
        } else {
            cur += c;
        }
    }
    fields.push_back(giai_ma(cur));
    return fields;
}

bool RecordFile::doc_file(const string& path, vector<vector<string>>& records) {
    ifstream f(path);
    if (!f.is_open()) return false;
    string line;
    while (getline(f, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        
        if (line.empty()) continue;
        records.push_back(tach_cot(line));
    }
    return true;
}

bool RecordFile::ghi_file(const string& path, const vector<string>& lines) {
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
