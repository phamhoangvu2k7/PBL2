#include "../../include/services/AuthService.h"
#include "../../include/helper/IdGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Đường dẫn file lưu tài khoản (tương đối với CWD khi chạy app)
const string AuthService::ACCOUNTS_FILE = "src/data/accounts.txt";

// ======================== HÀM BĂM MẬT KHẨU (DJB2) ========================
string AuthService::hashPassword(const string& plaintext) {
    unsigned long hash = 5381;
    for (unsigned char c : plaintext) {
        hash = ((hash << 5) + hash) + c;
    }
    return to_string(hash);
}

// ======================== CONSTRUCTOR / DESTRUCTOR ========================

AuthService::AuthService() : currentUser(nullptr) {
    // Thử đọc từ file trước
    loadFromFile();

    // Nếu chưa có admin thì tạo mặc định
    if (!accountTable.containsKey("admin")) {
        string adminId  = IdGenerator::generateAccountId(); // "AC001"
        string passHash = hashPassword("admin123");
        Account* adminAcc = new Account(adminId, "admin", passHash, "ADMIN", "Admin");
        accountList.push_back(adminAcc);
        accountTable.insert("admin", adminAcc);
        saveToFile(); // Lưu ngay
    }
}

AuthService::~AuthService() {
    Node<Account*>* cur = accountList.getHead();
    while (cur != nullptr) {
        delete cur->data;
        cur = cur->next;
    }
    accountList.clear();
    accountTable.clear();
}

// ======================== LƯU / ĐỌC FILE ========================

// Định dạng mỗi dòng: accountId|username|passwordHash|personId|role
void AuthService::saveToFile() const {
    ofstream f(ACCOUNTS_FILE);
    if (!f.is_open()) {
        cerr << "[Loi] Khong the mo file de luu tai khoan: " << ACCOUNTS_FILE << "\n";
        return;
    }
    Node<Account*>* cur = accountList.getHead();
    while (cur != nullptr) {
        Account* a = cur->data;
        f << a->getAccountId()   << "|"
          << a->getUsername()    << "|"
          << a->getPasswordHash()<< "|"
          << a->getPersonId()    << "|"
          << a->getRole()        << "\n";
        cur = cur->next;
    }
    f.close();
}

void AuthService::loadFromFile() {
    ifstream f(ACCOUNTS_FILE);
    if (!f.is_open()) return;  // File chưa tồn tại = lần đầu chạy

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;

        // Tách theo '|'
        istringstream ss(line);
        string accId, username, passHash, personId, role;
        if (!getline(ss, accId,     '|')) continue;
        if (!getline(ss, username,  '|')) continue;
        if (!getline(ss, passHash,  '|')) continue;
        if (!getline(ss, personId,  '|')) continue;
        if (!getline(ss, role,      '|')) continue;

        if (username.empty() || passHash.empty()) continue;

        // Đồng bộ bộ đếm ID
        IdGenerator::syncFromExistingId(accId);

        Account* acc = new Account(accId, username, passHash, personId, role);
        accountList.push_back(acc);
        accountTable.insert(username, acc);
    }
    f.close();
}

// ======================== ĐĂNG KÝ ========================

bool AuthService::registerAccount(const string& username, const string& password,
                                   const string& role) {
    // Kiểm tra đầu vào
    if (username.empty() || password.empty()) {
        cout << "[Loi] Username va mat khau khong duoc de trong!\n";
        return false;
    }
    if (accountTable.containsKey(username)) {
        cout << "[Loi] Username '" << username << "' da ton tai!\n";
        return false;
    }
    if (password.length() < 6) {
        cout << "[Loi] Mat khau phai co it nhat 6 ky tu!\n";
        return false;
    }
    if (role != "Student" && role != "Tutor" && role != "Admin") {
        cout << "[Loi] Vai tro khong hop le!\n";
        return false;
    }

    // Tự động sinh personId theo role
    string personId;
    if (role == "Student")     personId = IdGenerator::generateStudentId();
    else if (role == "Tutor")  personId = IdGenerator::generateTutorId();
    else                       personId = "ADMIN";

    string accId    = IdGenerator::generateAccountId();
    string passHash = hashPassword(password);
    Account* newAcc = new Account(accId, username, passHash, personId, role);

    accountList.push_back(newAcc);
    accountTable.insert(username, newAcc);

    // Lưu ngay vào file
    saveToFile();

    cout << "[Thanh Cong] Tai khoan '" << username << "' (Role: " << role
         << ", PersonId: " << personId << ") da duoc tao!\n";
    return true;
}

// ======================== ĐĂNG NHẬP ========================

bool AuthService::login(const string& username, const string& password) {
    Account* acc = nullptr;
    if (!accountTable.find(username, acc) || acc == nullptr) {
        cout << "[Loi] Tai khoan '" << username << "' khong ton tai!\n";
        return false;
    }

    string inputHash = hashPassword(password);
    if (acc->getPasswordHash() != inputHash) {
        cout << "[Loi] Mat khau khong chinh xac!\n";
        return false;
    }

    currentUser = acc;
    cout << "[OK] Dang nhap: " << username << " (Role: " << acc->getRole() << ")\n";
    return true;
}

// ======================== ĐĂNG XUẤT ========================

void AuthService::logout() {
    currentUser = nullptr;
}

// ======================== TRUY VẤN TRẠNG THÁI ========================

bool    AuthService::isLoggedIn()       const { return currentUser != nullptr; }
Account* AuthService::getCurrentUser()  const { return currentUser; }

string AuthService::getCurrentRole() const {
    return currentUser ? currentUser->getRole() : "";
}

string AuthService::getCurrentPersonId() const {
    return currentUser ? currentUser->getPersonId() : "";
}

bool AuthService::isStudent() const { return currentUser && currentUser->getRole() == "Student"; }
bool AuthService::isTutor()   const { return currentUser && currentUser->getRole() == "Tutor"; }
bool AuthService::isAdmin()   const { return currentUser && currentUser->getRole() == "Admin"; }

bool AuthService::usernameExists(const string& username) const {
    return accountTable.containsKey(username);
}
