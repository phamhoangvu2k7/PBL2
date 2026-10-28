#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "../services/CenterService.h"
#include "../services/AuthService.h"

class SchedulePicker;
class QPlainTextEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // service dung chung cho moi cua so
    // auth null hoac chua dang nhap: che do Khach
    explicit MainWindow(AuthService* auth, CenterService& service, QWidget* parent = nullptr);
    ~MainWindow();

private:
    CenterService& service;
    AuthService* m_auth;  // null = Khach
    QString m_currentRole;
    QString m_currentPersonId;
    QString m_currentName;  // ten that de hien thi
    bool m_profileMissing;  // tai khoan chua co ho so

    // widgets chinh
    QTabWidget* tabWidget;

    // tab 1: quan ly nguoi dung (Admin)
    QTableWidget* studentTableWidget;
    QTableWidget* tutorTableWidget;
    // ID tu sinh, khong can o nhap
    QLabel* stGeneratedIdLabel;  // ma vua sinh
    QLineEdit* stNameInput;
    QLineEdit* stGradeInput;
    QLineEdit* stPhoneInput;
    QLineEdit* stEmailInput;
    QLineEdit* stSubjectsInput;
    QLineEdit* stBudgetInput;

    QLabel* tuGeneratedIdLabel;  // ma vua sinh
    QLineEdit* tuNameInput;
    QLineEdit* tuPhoneInput;
    QLineEdit* tuEmailInput;
    QLineEdit* tuSubjectsInput;
    QLineEdit* tuExpInput;

    // tab 2: tim kiem & dang ky
    QLineEdit* searchSubjectInput;
    QTableWidget* availableClassesTable;
    QLineEdit* selectedClassIdInput;
    QLabel* bookingActorLabel;  // hoc vien dang dang nhap
    QComboBox* bookingStudentCombo;  // chi Admin: chon hoc vien dang ky ho
    QPushButton* btnBook;

    // tab 3: gia su mo ca & phe duyet
    QLabel* tutorActorLabel;  // gia su dang dang nhap
    QComboBox* tutorActorCombo;  // chi Admin: chon gia su thao tac ho
    QLineEdit* openSubjectInput;
    SchedulePicker* schedulePicker;  // chon thu + gio, nhieu khung
    QLineEdit* openFeeInput;
    QTableWidget* pendingRequestsTable;

    // tab 5: ho so ca nhan, sua duoc moi truong tru ma
    QLabel* pfIdLabel;
    QLineEdit* pfName;
    QLineEdit* pfPhone;
    QLineEdit* pfEmail;
    QComboBox* pfGender;
    QLineEdit* pfDob;
    QLineEdit* pfAddress;
    QLineEdit* pfSubjects;  // mon quan tam (hoc vien) / mon day (gia su)
    QLineEdit* pfSchedule;  // lich ranh, ghi tu do
    QLineEdit* pfGrade;  // chi hoc vien
    QLineEdit* pfBudget;  // chi hoc vien
    QLineEdit* pfQualification;  // chi gia su
    QLineEdit* pfExp;  // chi gia su
    QPlainTextEdit* pfAchievements;  // chi gia su
    QPushButton* btnSaveProfile;
    QPushButton* btnResetProfile;

    QLabel* userInfoLabel;  // ten tren toolbar

    // tab 4: lop cua toi & hop dong
    QGroupBox* myClassesBox;
    QGroupBox* contractsBox;
    QTableWidget* myClassesTable;
    QTableWidget* contractsTableWidget;

    void setupUi();
    QWidget* createUsersTab();
    QWidget* createBookingTab();
    QWidget* createTutorTab();
    QWidget* createOverviewTab();
    QWidget* createProfileTab();
    void applyStyles();
    // phan quyen tab theo role
    void applyRolePermissions();

    // ma SV / GS dang thao tac: chu tai khoan, hoac do Admin chon
    QString actingStudentId() const;
    QString actingTutorId() const;

    void populateActorCombos();
    QString identityText() const;
    void showTutorProfile(const QString& tutorId);  // hop thoai xem ho so gia su
    void refreshIdentity();  // cap nhat ten hien thi
    void refreshPendingRequests();
    void refreshMyClasses();
    void refreshContracts();

public:
    // dang xuat, ve LoginWindow
    void onLogout();

private slots:
    void onAddStudent();
    void onAddTutor();
    void onSearchAvailableClasses();
    void onResetSearchAvailableClasses();
    void onAvailableClassSelected(int row, int col);
    void onBookClass();
    void onOpenClass();
    void onApproveRequest();
    void onRejectRequest();
    void loadProfileForm();  // nap ho so vao form, dung cho nut Hoan Tac
    void onSaveProfile();
    void refreshAllTables();
};

#endif
