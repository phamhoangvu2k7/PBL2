#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

// Framework test toi gian (header-only, khong phu thuoc gtest/Qt).
//   TEST(Ten) { CHECK(...); CHECK_EQ(a, b); CHECK_THROWS(expr); ... }
// Moi CHECK that bai chi ghi nhan loi roi chay tiep; test fail neu co >= 1 loi.

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace tf {

struct TestCase {
    const char* name;
    const char* file;
    void (*fn)();
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> r;
    return r;
}

struct Registrar {
    Registrar(const char* name, const char* file, void (*fn)()) {
        registry().push_back({name, file, fn});
    }
};

struct State {
    int checks = 0;
    std::vector<std::string> failures;  // loi cua test dang chay
};

inline State& state() {
    static State s;
    return s;
}

inline std::string baseName(const char* path) {
    std::string p(path);
    size_t pos = p.find_last_of("/\\");
    return pos == std::string::npos ? p : p.substr(pos + 1);
}

inline void recordFail(const char* file, int line, const std::string& msg) {
    std::ostringstream os;
    os << baseName(file) << ":" << line << ": " << msg;
    state().failures.push_back(os.str());
}

template <typename T>
std::string show(const T& v) {
    std::ostringstream os;
    os << v;
    return os.str();
}
inline std::string show(const std::string& v) { return "\"" + v + "\""; }
inline std::string show(const char* v) { return std::string("\"") + v + "\""; }
inline std::string show(bool v) { return v ? "true" : "false"; }

template <typename A, typename B>
void checkEq(const A& a, const B& b, const char* ea, const char* eb, const char* file, int line) {
    state().checks++;
    if (!(a == b)) {
        recordFail(file, line, std::string("CHECK_EQ(") + ea + ", " + eb + ") sai: " +
                                   show(a) + " != " + show(b));
    }
}

template <typename A, typename B>
void checkNe(const A& a, const B& b, const char* ea, const char* eb, const char* file, int line) {
    state().checks++;
    if (a == b) {
        recordFail(file, line, std::string("CHECK_NE(") + ea + ", " + eb + ") sai: ca hai = " +
                                   show(a));
    }
}

// ---- Tien ich file tam (moi test dung thu muc rieng, tu xoa) ----
class TempDir {
    std::filesystem::path dir;

public:
    TempDir() {
        static int counter = 0;
        auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        dir = std::filesystem::temp_directory_path() /
              ("pbl2_test_" + std::to_string(stamp) + "_" + std::to_string(counter++));
        std::filesystem::create_directories(dir);
    }
    ~TempDir() {
        std::error_code ec;
        std::filesystem::remove_all(dir, ec);
    }
    TempDir(const TempDir&) = delete;
    TempDir& operator=(const TempDir&) = delete;
    std::string file(const std::string& name) const { return (dir / name).string(); }
    std::string path() const { return dir.string(); }
};

inline void writeFile(const std::string& path, const std::string& content) {
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    f << content;
}

inline std::string readFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

inline int countChar(const std::string& s, char c) {
    int n = 0;
    for (char x : s) if (x == c) n++;
    return n;
}

}  // namespace tf

#define TF_CONCAT2(a, b) a##b
#define TF_CONCAT(a, b) TF_CONCAT2(a, b)

#define TEST(name)                                                              \
    static void test_##name();                                                  \
    static tf::Registrar TF_CONCAT(reg_, name)(#name, __FILE__, &test_##name);  \
    static void test_##name()

#define CHECK(cond)                                                             \
    do {                                                                        \
        tf::state().checks++;                                                   \
        if (!(cond)) tf::recordFail(__FILE__, __LINE__, "CHECK(" #cond ") sai"); \
    } while (0)

#define CHECK_TRUE(cond) CHECK(cond)
#define CHECK_FALSE(cond)                                                       \
    do {                                                                        \
        tf::state().checks++;                                                   \
        if ((cond)) tf::recordFail(__FILE__, __LINE__, "CHECK_FALSE(" #cond ") sai"); \
    } while (0)

#define CHECK_EQ(a, b) tf::checkEq((a), (b), #a, #b, __FILE__, __LINE__)
#define CHECK_NE(a, b) tf::checkNe((a), (b), #a, #b, __FILE__, __LINE__)

#define CHECK_NOTHROW(expr)                                                     \
    do {                                                                        \
        tf::state().checks++;                                                   \
        try {                                                                   \
            expr;                                                               \
        } catch (const std::exception& ex_) {                                   \
            tf::recordFail(__FILE__, __LINE__,                                  \
                           std::string("CHECK_NOTHROW(" #expr ") nem exception: ") + ex_.what()); \
        } catch (...) {                                                         \
            tf::recordFail(__FILE__, __LINE__, "CHECK_NOTHROW(" #expr ") nem exception la"); \
        }                                                                       \
    } while (0)

#define CHECK_THROWS(expr)                                                      \
    do {                                                                        \
        tf::state().checks++;                                                   \
        bool threw_ = false;                                                    \
        try {                                                                   \
            expr;                                                               \
        } catch (...) {                                                         \
            threw_ = true;                                                      \
        }                                                                       \
        if (!threw_) tf::recordFail(__FILE__, __LINE__, "CHECK_THROWS(" #expr ") khong nem exception"); \
    } while (0)

#endif
