#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <string>
#include "../dsa/DoublyLinkedList.h"
#include "../dsa/HashTable.h"
#include "../entities/Account.h"
using namespace std;

class AuthService {
private:
    DoublyLinkedList<Account*> accountList;         // Kho lưu trữ chính
    HashTable<string, Account*> accountTable;       // Tra cứu theo username O(1)
    Account* currentUser;                           // Tài khoản đang đăng nhập

    static string hashPassword(const string& plaintext);  // DJB2 one-way hash

    // Đường dẫn file lưu tài khoản
    static const string ACCOUNTS_FILE;

public:
    AuthService();
    ~AuthService();

    // Đăng ký tài khoản mới
    // personId được tự động sinh bởi IdGenerator nếu để rỗng
    bool registerAccount(const string& username, const string& password,
                         const string& role);

    // Đăng nhập
    bool login(const string& username, const string& password);

    // Đăng xuất
    void logout();

    // Truy vấn trạng thái
    bool isLoggedIn() const;
    Account* getCurrentUser() const;
    string getCurrentRole() const;
    string getCurrentPersonId() const;

    bool isStudent() const;
    bool isTutor() const;
    bool isAdmin() const;

    bool usernameExists(const string& username) const;

    // Lưu tất cả tài khoản vào file
    void saveToFile() const;

    // Đọc tài khoản từ file
    void loadFromFile();

    const DoublyLinkedList<Account*>& getAccountList() const { return accountList; }
};

#endif
