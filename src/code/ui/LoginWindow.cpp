#include "../../include/ui/LoginWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QApplication>
#include <QScreen>
#include <QFrame>

LoginWindow::LoginWindow(AuthService& auth, QWidget* parent)
    : QDialog(parent), m_auth(auth), m_loginSuccess(false), m_guestMode(false)
{
    setWindowTitle("Dang Nhap - He Thong Quan Ly Gia Su PBL2");
    setFixedSize(480, 500);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QScreen* screen = QApplication::primaryScreen();
    QRect sg = screen->availableGeometry();
    move(sg.center() - rect().center());

    setupUi();
    applyStyles();
}

LoginWindow::~LoginWindow() {}

// ======================== SETUP UI ========================

void LoginWindow::setupUi() {
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(createLoginPage());    // Index 0
    stackedWidget->addWidget(createRegisterPage()); // Index 1
    stackedWidget->setCurrentIndex(0);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
}

QWidget* LoginWindow::createLoginPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(50, 40, 50, 30);
    layout->setSpacing(0);

    // --- Header ---
    QLabel* titleLabel = new QLabel("HE THONG QUAN LY GIA SU");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    QLabel* subtitleLabel = new QLabel("Trung Tam Gia Su PBL2");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setObjectName("subtitleLabel");

    QFrame* divider = new QFrame();
    divider->setFrameShape(QFrame::HLine);
    divider->setObjectName("divider");

    layout->addWidget(titleLabel);
    layout->addSpacing(4);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(16);
    layout->addWidget(divider);
    layout->addSpacing(20);

    // --- Form ---
    QLabel* usernameLabel = new QLabel("Ten dang nhap:");
    usernameLabel->setObjectName("fieldLabel");
    loginUsernameInput = new QLineEdit();
    loginUsernameInput->setPlaceholderText("Nhap ten dang nhap...");
    loginUsernameInput->setObjectName("inputField");
    loginUsernameInput->setMinimumHeight(40);

    QLabel* passwordLabel = new QLabel("Mat khau:");
    passwordLabel->setObjectName("fieldLabel");
    loginPasswordInput = new QLineEdit();
    loginPasswordInput->setPlaceholderText("Nhap mat khau...");
    loginPasswordInput->setEchoMode(QLineEdit::Password);
    loginPasswordInput->setObjectName("inputField");
    loginPasswordInput->setMinimumHeight(40);

    connect(loginPasswordInput, &QLineEdit::returnPressed, this, &LoginWindow::onLogin);
    connect(loginUsernameInput, &QLineEdit::returnPressed, this, &LoginWindow::onLogin);

    loginErrorLabel = new QLabel("");
    loginErrorLabel->setObjectName("errorLabel");
    loginErrorLabel->setAlignment(Qt::AlignCenter);
    loginErrorLabel->setWordWrap(true);
    loginErrorLabel->setMinimumHeight(24);

    layout->addWidget(usernameLabel);
    layout->addSpacing(6);
    layout->addWidget(loginUsernameInput);
    layout->addSpacing(14);
    layout->addWidget(passwordLabel);
    layout->addSpacing(6);
    layout->addWidget(loginPasswordInput);
    layout->addSpacing(8);
    layout->addWidget(loginErrorLabel);
    layout->addSpacing(14);

    // --- Buttons ---
    btnLogin = new QPushButton("DANG NHAP");
    btnLogin->setObjectName("primaryBtn");
    btnLogin->setMinimumHeight(44);
    btnLogin->setCursor(Qt::PointingHandCursor);

    layout->addWidget(btnLogin);
    layout->addSpacing(12);

    QHBoxLayout* linksLayout = new QHBoxLayout();
    btnGoRegister = new QPushButton("Dang ky tai khoan moi");
    btnGoRegister->setObjectName("linkBtn");
    btnGoRegister->setCursor(Qt::PointingHandCursor);

    btnGuest = new QPushButton("Vao voi tu cach Khach");
    btnGuest->setObjectName("linkBtn");
    btnGuest->setCursor(Qt::PointingHandCursor);

    linksLayout->addWidget(btnGoRegister);
    linksLayout->addStretch();
    linksLayout->addWidget(btnGuest);
    layout->addLayout(linksLayout);

    layout->addStretch();

    QLabel* hintLabel = new QLabel("Admin mac dinh: admin / admin123");
    hintLabel->setObjectName("hintLabel");
    hintLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(hintLabel);

    connect(btnLogin,       &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(btnGoRegister,  &QPushButton::clicked, this, &LoginWindow::onShowRegisterPage);
    connect(btnGuest,       &QPushButton::clicked, this, &LoginWindow::onGuestMode);

    return page;
}

QWidget* LoginWindow::createRegisterPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(50, 40, 50, 30);
    layout->setSpacing(0);

    // --- Header ---
    QLabel* titleLabel = new QLabel("TAO TAI KHOAN MOI");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    QFrame* divider = new QFrame();
    divider->setFrameShape(QFrame::HLine);
    divider->setObjectName("divider");

    layout->addWidget(titleLabel);
    layout->addSpacing(12);
    layout->addWidget(divider);
    layout->addSpacing(20);

    // --- Form ---
    QLabel* usernameLabel = new QLabel("Ten dang nhap:");
    usernameLabel->setObjectName("fieldLabel");
    regUsernameInput = new QLineEdit();
    regUsernameInput->setPlaceholderText("Nhap ten dang nhap (khong chua khoang trang)");
    regUsernameInput->setObjectName("inputField");
    regUsernameInput->setMinimumHeight(40);

    QLabel* passwordLabel = new QLabel("Mat khau (it nhat 6 ky tu):");
    passwordLabel->setObjectName("fieldLabel");
    regPasswordInput = new QLineEdit();
    regPasswordInput->setPlaceholderText("Nhap mat khau...");
    regPasswordInput->setEchoMode(QLineEdit::Password);
    regPasswordInput->setObjectName("inputField");
    regPasswordInput->setMinimumHeight(40);

    QLabel* confirmLabel = new QLabel("Xac nhan mat khau:");
    confirmLabel->setObjectName("fieldLabel");
    regConfirmInput = new QLineEdit();
    regConfirmInput->setPlaceholderText("Nhap lai mat khau...");
    regConfirmInput->setEchoMode(QLineEdit::Password);
    regConfirmInput->setObjectName("inputField");
    regConfirmInput->setMinimumHeight(40);

    QLabel* roleLabel = new QLabel("Vai tro:");
    roleLabel->setObjectName("fieldLabel");
    regRoleCombo = new QComboBox();
    regRoleCombo->addItems({"Student", "Tutor"});  // Khong cho tu tao Admin
    regRoleCombo->setObjectName("comboField");
    regRoleCombo->setMinimumHeight(40);

    // Ma dinh danh duoc tu dong sinh - hien thi thong bao
    QLabel* personIdNote = new QLabel("Ma dinh danh (Ma SV/GS) se duoc he thong tu dong cap.");
    personIdNote->setObjectName("noteLabel");
    personIdNote->setWordWrap(true);

    // Khong co regPersonIdInput - dau duoc xoa khoi form

    regErrorLabel = new QLabel("");
    regErrorLabel->setObjectName("errorLabel");
    regErrorLabel->setAlignment(Qt::AlignCenter);
    regErrorLabel->setWordWrap(true);
    regErrorLabel->setMinimumHeight(24);

    layout->addWidget(usernameLabel);
    layout->addSpacing(6);
    layout->addWidget(regUsernameInput);
    layout->addSpacing(12);
    layout->addWidget(passwordLabel);
    layout->addSpacing(6);
    layout->addWidget(regPasswordInput);
    layout->addSpacing(12);
    layout->addWidget(confirmLabel);
    layout->addSpacing(6);
    layout->addWidget(regConfirmInput);
    layout->addSpacing(12);
    layout->addWidget(roleLabel);
    layout->addSpacing(6);
    layout->addWidget(regRoleCombo);
    layout->addSpacing(8);
    layout->addWidget(personIdNote);
    layout->addSpacing(6);
    layout->addWidget(regErrorLabel);
    layout->addSpacing(12);

    btnRegister = new QPushButton("TAO TAI KHOAN");
    btnRegister->setObjectName("primaryBtn");
    btnRegister->setMinimumHeight(44);
    btnRegister->setCursor(Qt::PointingHandCursor);

    btnGoLogin = new QPushButton("Quay lai dang nhap");
    btnGoLogin->setObjectName("linkBtn");
    btnGoLogin->setCursor(Qt::PointingHandCursor);

    layout->addWidget(btnRegister);
    layout->addSpacing(10);
    layout->addWidget(btnGoLogin, 0, Qt::AlignCenter);

    connect(btnRegister,    &QPushButton::clicked, this, &LoginWindow::onRegister);
    connect(btnGoLogin,     &QPushButton::clicked, this, &LoginWindow::onShowLoginPage);
    connect(regConfirmInput, &QLineEdit::returnPressed, this, &LoginWindow::onRegister);

    return page;
}

// ======================== STYLES - THEME TRANG SANG ========================

void LoginWindow::applyStyles() {
    setStyleSheet(R"(
        QDialog {
            background-color: #ffffff;
        }
        QWidget {
            background-color: #ffffff;
            color: #1e293b;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 14px;
        }
        QLabel#titleLabel {
            font-size: 20px;
            font-weight: bold;
            color: #1e40af;
            padding: 4px 0;
        }
        QLabel#subtitleLabel {
            font-size: 13px;
            color: #64748b;
        }
        QFrame#divider {
            background-color: #e2e8f0;
            max-height: 1px;
        }
        QLabel#fieldLabel {
            font-size: 13px;
            font-weight: 600;
            color: #374151;
        }
        QLabel#noteLabel {
            font-size: 12px;
            color: #2563eb;
            background-color: #eff6ff;
            border: 1px solid #bfdbfe;
            border-radius: 6px;
            padding: 8px 10px;
        }
        QLabel#hintLabel {
            font-size: 12px;
            color: #94a3b8;
            padding: 8px;
            border-top: 1px solid #e2e8f0;
        }
        QLabel#errorLabel {
            font-size: 13px;
            color: #dc2626;
            background-color: #fef2f2;
            border-radius: 6px;
            padding: 4px 8px;
        }
        QLineEdit#inputField {
            background-color: #f8fafc;
            border: 1.5px solid #cbd5e1;
            border-radius: 8px;
            padding: 8px 14px;
            font-size: 14px;
            color: #0f172a;
            selection-background-color: #bfdbfe;
        }
        QLineEdit#inputField:focus {
            border: 1.5px solid #2563eb;
            background-color: #ffffff;
        }
        QLineEdit#inputField:hover {
            border-color: #93c5fd;
        }
        QComboBox#comboField {
            background-color: #f8fafc;
            border: 1.5px solid #cbd5e1;
            border-radius: 8px;
            padding: 8px 14px;
            font-size: 14px;
            color: #0f172a;
        }
        QComboBox#comboField:focus {
            border-color: #2563eb;
        }
        QPushButton#primaryBtn {
            background-color: #1d4ed8;
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 10px;
            font-size: 14px;
            font-weight: bold;
            letter-spacing: 1px;
        }
        QPushButton#primaryBtn:hover {
            background-color: #1e40af;
        }
        QPushButton#primaryBtn:pressed {
            background-color: #1e3a8a;
        }
        QPushButton#linkBtn {
            background: transparent;
            color: #2563eb;
            border: none;
            font-size: 13px;
            padding: 4px 0;
            text-decoration: underline;
        }
        QPushButton#linkBtn:hover {
            color: #1d4ed8;
        }
    )");
}

// ======================== SLOTS ========================

void LoginWindow::onLogin() {
    loginErrorLabel->clear();
    loginErrorLabel->setStyleSheet("");

    QString username = loginUsernameInput->text().trimmed();
    QString password = loginPasswordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        loginErrorLabel->setText("Vui long nhap day du ten dang nhap va mat khau!");
        return;
    }

    if (m_auth.login(username.toStdString(), password.toStdString())) {
        m_loginSuccess = true;
        m_guestMode = false;
        accept();
    } else {
        loginErrorLabel->setText("Ten dang nhap hoac mat khau khong chinh xac!");
        loginPasswordInput->clear();
        loginPasswordInput->setFocus();
    }
}

void LoginWindow::onRegister() {
    regErrorLabel->clear();
    regErrorLabel->setStyleSheet("");

    QString username = regUsernameInput->text().trimmed();
    QString password = regPasswordInput->text();
    QString confirm  = regConfirmInput->text();
    QString role     = regRoleCombo->currentText();

    if (username.isEmpty() || password.isEmpty()) {
        regErrorLabel->setText("Ten dang nhap va mat khau khong duoc de trong!");
        return;
    }
    if (username.contains(' ')) {
        regErrorLabel->setText("Ten dang nhap khong duoc chua khoang trang!");
        return;
    }
    if (m_auth.usernameExists(username.toStdString())) {
        regErrorLabel->setText("Ten dang nhap '" + username + "' da ton tai! Vui long chon ten khac.");
        regUsernameInput->setFocus();
        regUsernameInput->selectAll();
        return;
    }
    if (password.length() < 6) {
        regErrorLabel->setText("Mat khau phai co it nhat 6 ky tu!");
        return;
    }
    if (password != confirm) {
        regErrorLabel->setText("Xac nhan mat khau khong khop!");
        regConfirmInput->clear();
        regConfirmInput->setFocus();
        return;
    }

    // Goi registerAccount khong truyen personId - tu dong sinh ben trong
    bool ok = m_auth.registerAccount(
        username.toStdString(), password.toStdString(), role.toStdString()
    );

    if (ok) {
        // Hien thi thanh cong va chuyen ve trang dang nhap
        regErrorLabel->setStyleSheet("color: #16a34a; background-color: #f0fdf4; border: 1px solid #bbf7d0;");
        regErrorLabel->setText("Tao tai khoan thanh cong! Ma dinh danh tu dong tao.");

        regUsernameInput->clear();
        regPasswordInput->clear();
        regConfirmInput->clear();

        loginUsernameInput->setText(username);
        loginPasswordInput->clear();
        loginPasswordInput->setFocus();

        // Chuyen ve trang dang nhap sau 1 giay
        QTimer::singleShot(1000, this, &LoginWindow::onShowLoginPage);
    } else {
        // Truong hop con lai (vi du role sai - khong nen xay ra)
        regErrorLabel->setText("Tao tai khoan that bai. Vui long thu lai.");
    }
}

void LoginWindow::onGuestMode() {
    m_loginSuccess = false;
    m_guestMode = true;
    accept();
}

void LoginWindow::onShowRegisterPage() {
    loginErrorLabel->clear();
    regErrorLabel->clear();
    stackedWidget->setCurrentIndex(1);
}

void LoginWindow::onShowLoginPage() {
    regErrorLabel->clear();
    loginErrorLabel->clear();
    stackedWidget->setCurrentIndex(0);
}
