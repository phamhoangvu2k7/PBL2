#ifndef RECORD_FILE_H
#define RECORD_FILE_H

#include <string>
#include <vector>
using namespace std;

class RecordFile {
public:

    static string ma_hoa(const string& field);
    static string giai_ma(const string& field);

    static string noi_cot(const vector<string>& fields);
    static vector<string> tach_cot(const string& line);

    static bool doc_file(const string& path, vector<vector<string>>& records);

    static bool ghi_file(const string& path, const vector<string>& lines);
};

#endif
