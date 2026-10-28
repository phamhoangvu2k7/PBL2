#ifndef RECORD_FILE_H
#define RECORD_FILE_H

#include <string>
#include <vector>
using namespace std;

// doc / ghi file text, moi dong 1 ban ghi, cac truong ngan cach bang '|'
class RecordFile {
public:
    RecordFile() = delete;

    // escape de truong chua '|', '\', xuong dong van an toan
    static string escape(const string& field);
    static string unescape(const string& field);

    static string join(const vector<string>& fields);
    static vector<string> split(const string& line);

    // doc file thanh cac ban ghi, bo qua dong trong, false neu khong mo duoc
    static bool read(const string& path, vector<vector<string>>& records);

    // ghi file tam roi doi ten, tranh hong file cu khi ghi do
    static bool writeAtomic(const string& path, const vector<string>& lines);
};

#endif
