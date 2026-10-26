#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "../services/CenterService.h"
#include "../services/AuthService.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // Guest mode: auth = nullptr
    explicit MainWindow(AuthService* auth = nullptr, QWidget* parent = nullptr);
    ~MainWindow();

private:
    CenterService service;
    AuthService* m_auth;        // nullptr = chế độ Khách
    QString m_currentRole;      // "Student", "Tutor", "Admin", "Guest"
    QString m_currentPersonId;  // "SV001", "GS002", ...

    // Widgets chính
    QTabWidget* tabWidget;

    // Tab 1: Quản lý Người dùng
    QTableWidget* studentTableWidget;
    QTableWidget* tutorTableWidget;
    // ID được tự động sinh - không cần ô nhập
    QLabel* stGeneratedIdLabel;   // Hiện mã vừa sinh: "SV001"
    QLineEdit* stNameInput;
    QLineEdit* stGradeInput;
    QLineEdit* stPhoneInput;
    QLineEdit* stSubjectsInput;
    QLineEdit* stBudgetInput;

    QLabel* tuGeneratedIdLabel;   // Hiện mã vừa sinh: "GS001"
    QLineEdit* tuNameInput;
    QLineEdit* tuPhoneInput;
    QLineEdit* tuSubjectsInput;
    QLineEdit* tuExpInput;
    QLineEdit* tuRateInput;

    // Tab 2: Tìm kiếm & Đăng ký
    QLineEdit* searchSubjectInput;
    QTableWidget* availableClassesTable;
    QLineEdit* bookingStudentIdInput;
    QLineEdit* selectedClassIdInput;

    // Tab 3: Gia sư mở ca & Phê duyệt
    // openClassIdInput đã xóa - ID tự động sinh
    QLineEdit* openTutorIdInput;
    QLineEdit* openSubjectInput;
    QLineEdit* openScheduleInput;
    QLineEdit* openFeeInput;

    QLineEdit* tutorIdFilterInput;
    QTableWidget* pendingRequestsTable;

    // Tab 4: Lớp của tôi & Hợp đồng
    QLineEdit* myUserIdInput;
    QComboBox* myUserRoleCombo;
    QTableWidget* myClassesTable;
    QTableWidget* contractsTableWidget;

    void setupUi();
    QWidget* createUsersTab();
    QWidget* createBookingTab();
    QWidget* createTutorTab();
    QWidget* createOverviewTab();
    void applyStyles();
    void seedSampleData();
    // Áp dụng phân quyền Tab theo role
    void applyRolePermissions();

public:
    // Slot đăng xuất (quay về LoginWindow)
    void onLogout();

private slots:
    void onAddStudent();
    void onAddTutor();
    void onSearchAvailableClasses();
    void onResetSearchAvailableClasses();
    void onAvailableClassSelected(int row, int col);
    void onBookClass();
    void onOpenClass();
    void onFilterPendingRequests();
    void onApproveRequest();
    void onRejectRequest();
    void onFilterMyClasses();
    void refreshAllTables();
};

#endif
