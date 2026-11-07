#ifndef RECORD_FILE_H
#define RECORD_FILE_H

#include <string>
#include <vector>
using namespace std;

class RecordFile {
public:

    static string escape(const string& field);
    static string unescape(const string& field);

    static string join(const vector<string>& fields);
    static vector<string> split(const string& line);

    static bool read(const string& path, vector<vector<string>>& records);

    static bool write(const string& path, const vector<string>& lines);
};

#endif
