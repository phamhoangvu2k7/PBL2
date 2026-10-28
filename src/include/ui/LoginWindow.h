#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QStackedWidget>
#include <QTimer>
#include "../services/AuthService.h"

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoginWindow(AuthService& auth, QWidget* parent = nullptr);
    ~LoginWindow();

    bool loginSuccessful() const { return m_loginSuccess; }
    bool isGuestMode()     const { return m_guestMode; }

private:
    AuthService& m_auth;
    bool m_loginSuccess;
    bool m_guestMode;

    QStackedWidget* stackedWidget;

    // trang dang nhap
    QLineEdit*   loginUsernameInput;
    QLineEdit*   loginPasswordInput;
    QLabel*      loginErrorLabel;
    QPushButton* btnLogin;
    QPushButton* btnGoRegister;
    QPushButton* btnGuest;

    // trang dang ky
    QLineEdit*   regUsernameInput;
    QLineEdit*   regFullNameInput;  // ten that
    QLineEdit*   regPhoneInput;
    QLineEdit*   regEmailInput;
    QLineEdit*   regPasswordInput;
    QLineEdit*   regConfirmInput;
    // ma dinh danh tu sinh, khong co o nhap
    QComboBox*   regRoleCombo;
    QLabel*      regErrorLabel;
    QPushButton* btnRegister;
    QPushButton* btnGoLogin;

    void setupUi();
    QWidget* createLoginPage();
    QWidget* createRegisterPage();
    void applyStyles();
    void resizeForPage(int pageIndex);
    QWidget* makeField(const QString& label, QWidget* input);

private slots:
    void onLogin();
    void onRegister();
    void onGuestMode();
    void onShowRegisterPage();
    void onShowLoginPage();
};

#endif
