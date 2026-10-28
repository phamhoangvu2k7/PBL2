#include "test_framework.h"

#include <map>

int main() {
    std::vector<tf::TestCase>& tests = tf::registry();
    int passed = 0, failed = 0, totalChecks = 0;
    std::map<std::string, std::pair<int, int>> perFile;  // file -> (pass, fail)
    std::vector<std::string> failedNames;

    std::streambuf* origCout = std::cout.rdbuf();

    for (const tf::TestCase& t : tests) {
        tf::state().checks = 0;
        tf::state().failures.clear();

        // Service in nhieu log len cout -> chan lai cho de doc ket qua
        std::ostringstream sink;
        std::cout.rdbuf(sink.rdbuf());
        try {
            t.fn();
        } catch (const std::exception& e) {
            tf::state().failures.push_back(std::string("Exception khong bat: ") + e.what());
        } catch (...) {
            tf::state().failures.push_back("Exception khong bat (kieu la)");
        }
        std::cout.rdbuf(origCout);

        totalChecks += tf::state().checks;
        std::string file = tf::baseName(t.file);
        if (tf::state().failures.empty()) {
            passed++;
            perFile[file].first++;
            std::cout << "[ PASS ] " << t.name << "\n";
        } else {
            failed++;
            perFile[file].second++;
            failedNames.push_back(t.name);
            std::cout << "[ FAIL ] " << t.name << "\n";
            for (const std::string& f : tf::state().failures) {
                std::cout << "           - " << f << "\n";
            }
        }
    }

    std::cout << "\n==================== TONG KET ====================\n";
    for (const auto& kv : perFile) {
        std::cout << "  " << kv.first << ": " << kv.second.first << " pass, "
                  << kv.second.second << " fail\n";
    }
    std::cout << "Tong: " << tests.size() << " test, " << passed << " pass, " << failed
              << " fail, " << totalChecks << " assertion\n";
    if (!failedNames.empty()) {
        std::cout << "Test FAIL:\n";
        for (const std::string& n : failedNames) std::cout << "  * " << n << "\n";
    }
    return failed == 0 ? 0 : 1;
}
