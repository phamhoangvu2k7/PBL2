#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <string>
#include "../dsa/DoublyLinkedList.h"
#include "../dsa/HashTable.h"
#include "../entities/Account.h"
using namespace std;

class AuthService {
private:
    DoublyLinkedList<Account*> accountList;  // danh sach chinh
    HashTable<string, Account*> accountTable;  // tra theo username, O(1)
    Account* currentUser;  // tai khoan dang dang nhap
    string accountsFile;  // file luu tai khoan

public:
    // duong dan mac dinh, tinh tu thu muc chay app
    static const string DEFAULT_ACCOUNTS_FILE;

    // co the doi file luu
    explicit AuthService(const string& accountsFile = DEFAULT_ACCOUNTS_FILE);
    ~AuthService();

    // dang ky tai khoan moi
    // Student/Tutor bat buoc co ten, sdt (10 so, dau 0), email
    // personId cap sau bang assignMissingPersonIds()
    // de khong trung ma du lieu mau
    // outError nhan ly do loi
    bool registerAccount(const string& username, const string& password,
                         const string& role, const string& fullName = "",
                         const string& phone = "", const string& email = "",
                         string* outError = nullptr);

    // cap ma SV/GS cho tai khoan chua co
    // tra ve so tai khoan vua cap, co thay doi thi luu file
    int assignMissingPersonIds();

    // dang nhap
    bool login(const string& username, const string& password);

    // dang xuat
    void logout();

    // trang thai dang nhap
    bool isLoggedIn() const;
    Account* getCurrentUser() const;
    string getCurrentRole() const;
    string getCurrentPersonId() const;
    string getCurrentDisplayName() const;

    bool isStudent() const;
    bool isTutor() const;
    bool isAdmin() const;

    bool usernameExists(const string& username) const;

    // luu tai khoan ra file, false neu khong ghi duoc
    bool saveToFile() const;

    // doc tai khoan tu file
    void loadFromFile();

    const DoublyLinkedList<Account*>& getAccountList() const { return accountList; }
};

#endif
