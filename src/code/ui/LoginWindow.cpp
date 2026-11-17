#include "../../include/ui/LoginWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QApplication>
#include <QScreen>
#include <QFrame>

LoginWindow::LoginWindow(AuthService& auth, QWidget* parent)
    : QDialog(parent), m_auth(auth)
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

// dung giao dien

void LoginWindow::setupUi() {
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(createLoginPage());
    stackedWidget->addWidget(createRegisterPage());
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

    // tieu de
    QLabel* titleLabel = new QLabel("Trung tam gia su");
    titleLabel->setObjectName("titleLabel");

    QLabel* subtitleLabel = new QLabel("Dang nhap de tiep tuc");
    subtitleLabel->setObjectName("subtitleLabel");

    layout->addWidget(titleLabel);
    layout->addSpacing(2);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(28);

    // form
    QLabel* usernameLabel = new QLabel("Ten dang nhap");
    usernameLabel->setObjectName("fieldLabel");
    loginUsernameInput = new QLineEdit();
    loginUsernameInput->setPlaceholderText("");
    loginUsernameInput->setObjectName("inputField");
    loginUsernameInput->setMinimumHeight(40);

    QLabel* passwordLabel = new QLabel("Mat khau");
    passwordLabel->setObjectName("fieldLabel");
    loginPasswordInput = new QLineEdit();
    loginPasswordInput->setPlaceholderText("");
    loginPasswordInput->setEchoMode(QLineEdit::Password);
    loginPasswordInput->setObjectName("inputField");
    loginPasswordInput->setMinimumHeight(40);

    connect(loginPasswordInput, &QLineEdit::returnPressed, this, &LoginWindow::onLogin);
    connect(loginUsernameInput, &QLineEdit::returnPressed, this, &LoginWindow::onLogin);

    loginErrorLabel = new QLabel("");
    loginErrorLabel->setObjectName("errorLabel");
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

    // nut
    btnLogin = new QPushButton("Dang nhap");
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
    layout->addWidget(hintLabel);

    connect(btnLogin,       &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(btnGoRegister,  &QPushButton::clicked, this, &LoginWindow::onShowRegisterPage);
    connect(btnGuest,       &QPushButton::clicked, this, &LoginWindow::onGuestMode);

    return page;
}

// 1 o nhap: nhan tren, input duoi
QWidget* LoginWindow::makeField(const QString& label, QWidget* input) {
    QWidget* cell = new QWidget();
    QVBoxLayout* v = new QVBoxLayout(cell);
    v->setContentsMargins(0, 0, 0, 0);
    v->setSpacing(4);
    QLabel* l = new QLabel(label);
    l->setObjectName("fieldLabel");
    v->addWidget(l);
    v->addWidget(input);
    return cell;
}

QWidget* LoginWindow::createRegisterPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(50, 30, 50, 24);
    layout->setSpacing(0);

    // tieu de
    QLabel* titleLabel = new QLabel("Tao tai khoan");
    titleLabel->setObjectName("titleLabel");

    layout->addWidget(titleLabel);
    layout->addSpacing(20);

    // form
    regUsernameInput = new QLineEdit();
    regUsernameInput->setPlaceholderText("Khong chua khoang trang");
    regUsernameInput->setObjectName("inputField");
    regUsernameInput->setMinimumHeight(38);

    regRoleCombo = new QComboBox();
    regRoleCombo->addItems({"Student", "Tutor"});  // khong cho tu tao Admin
    regRoleCombo->setObjectName("comboField");
    regRoleCombo->setMinimumHeight(38);

    regFullNameInput = new QLineEdit();
    regFullNameInput->setPlaceholderText("VD: Nguyen Van A");
    regFullNameInput->setObjectName("inputField");
    regFullNameInput->setMinimumHeight(38);

    regPhoneInput = new QLineEdit();
    regPhoneInput->setPlaceholderText("VD: 0912345678");
    regPhoneInput->setMaxLength(10);
    regPhoneInput->setObjectName("inputField");
    regPhoneInput->setMinimumHeight(38);

    regEmailInput = new QLineEdit();
    regEmailInput->setPlaceholderText("VD: abc@gmail.com");
    regEmailInput->setObjectName("inputField");
    regEmailInput->setMinimumHeight(38);

    regPasswordInput = new QLineEdit();
    regPasswordInput->setPlaceholderText("It nhat 6 ky tu");
    regPasswordInput->setEchoMode(QLineEdit::Password);
    regPasswordInput->setObjectName("inputField");
    regPasswordInput->setMinimumHeight(38);

    regConfirmInput = new QLineEdit();
    regConfirmInput->setPlaceholderText("Nhap lai mat khau");
    regConfirmInput->setEchoMode(QLineEdit::Password);
    regConfirmInput->setObjectName("inputField");
    regConfirmInput->setMinimumHeight(38);

    QGridLayout* grid = new QGridLayout();
    grid->setHorizontalSpacing(16);
    grid->setVerticalSpacing(10);
    grid->addWidget(makeField("Ten dang nhap", regUsernameInput), 0, 0);
    grid->addWidget(makeField("Vai tro", regRoleCombo),           0, 1);
    grid->addWidget(makeField("Ho va ten (hien thi)", regFullNameInput), 1, 0, 1, 2);
    grid->addWidget(makeField("So dien thoai", regPhoneInput),    2, 0);
    grid->addWidget(makeField("Email", regEmailInput),            2, 1);
    grid->addWidget(makeField("Mat khau", regPasswordInput),      3, 0);
    grid->addWidget(makeField("Xac nhan mat khau", regConfirmInput), 3, 1);
    layout->addLayout(grid);
    layout->addSpacing(10);

    // ma dinh danh tu dong cap
    QLabel* personIdNote = new QLabel("Ma dinh danh (Ma SV/GS) se duoc he thong tu dong cap.");
    personIdNote->setObjectName("noteLabel");
    personIdNote->setWordWrap(true);
    layout->addWidget(personIdNote);
    layout->addSpacing(6);

    regErrorLabel = new QLabel("");
    regErrorLabel->setObjectName("errorLabel");
    regErrorLabel->setWordWrap(true);
    regErrorLabel->setMinimumHeight(24);
    layout->addWidget(regErrorLabel);
    layout->addSpacing(10);

    btnRegister = new QPushButton("Tao tai khoan");
    btnRegister->setObjectName("primaryBtn");
    btnRegister->setMinimumHeight(44);
    btnRegister->setCursor(Qt::PointingHandCursor);

    btnGoLogin = new QPushButton("Quay lai dang nhap");
    btnGoLogin->setObjectName("linkBtn");
    btnGoLogin->setCursor(Qt::PointingHandCursor);

    layout->addWidget(btnRegister);
    layout->addSpacing(8);
    layout->addWidget(btnGoLogin, 0, Qt::AlignLeft);
    layout->addStretch();

    connect(btnRegister,    &QPushButton::clicked, this, &LoginWindow::onRegister);
    connect(btnGoLogin,     &QPushButton::clicked, this, &LoginWindow::onShowLoginPage);
    connect(regConfirmInput, &QLineEdit::returnPressed, this, &LoginWindow::onRegister);

    return page;
}

// style (theme sang)

void LoginWindow::applyStyles() {
    // toi gian: nen trang, mau navy, vien manh, khong do bong / gradient
    setStyleSheet(R"(
        QDialog, QWidget {
            background-color: #ffffff;
            color: #1f2937;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 14px;
        }
        QLabel#titleLabel {
            font-size: 22px;
            font-weight: 600;
            color: #1e2b4a;
        }
        QLabel#subtitleLabel {
            font-size: 13px;
            color: #6b7280;
        }
        QLabel#fieldLabel {
            font-size: 12px;
            font-weight: 600;
            color: #4b5563;
        }
        QLabel#noteLabel {
            font-size: 12px;
            color: #6b7280;
        }
        QLabel#hintLabel {
            font-size: 12px;
            color: #9ca3af;
        }
        QLabel#errorLabel {
            font-size: 13px;
            color: #b91c1c;
        }
        QLineEdit#inputField, QComboBox#comboField {
            background-color: #ffffff;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            color: #111827;
            selection-background-color: #dfe5f0;
            selection-color: #1e2b4a;
        }
        QLineEdit#inputField:hover, QComboBox#comboField:hover {
            border-color: #9ca3af;
        }
        QLineEdit#inputField:focus, QComboBox#comboField:focus {
            border-color: #1e2b4a;
        }        QPushButton#primaryBtn {
            background-color: #1e2b4a;
            color: #ffffff;
            border: none;
            border-radius: 6px;
            padding: 10px;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton#primaryBtn:hover {
            background-color: #2b3b61;
        }
        QPushButton#primaryBtn:pressed {
            background-color: #16203a;
        }
        QPushButton#linkBtn {
            background: transparent;
            color: #1e2b4a;
            border: none;
            font-size: 13px;
            padding: 4px 0;
        }
        QPushButton#linkBtn:hover {
            text-decoration: underline;
        }
    )");
}

// slots

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
    QString fullName = regFullNameInput->text().trimmed();
    QString phone    = regPhoneInput->text().trimmed();
    QString email    = regEmailInput->text().trimmed();
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
    if (m_auth.username_ton_tai(username.toStdString())) {
        regErrorLabel->setText("Ten dang nhap '" + username + "' da ton tai! Vui long chon ten khac.");
        regUsernameInput->setFocus();
        regUsernameInput->selectAll();
        return;
    }
    if (password != confirm) {
        regErrorLabel->setText("Xac nhan mat khau khong khop!");
        regConfirmInput->clear();
        regConfirmInput->setFocus();
        return;
    }

    // AuthService kiem tra du lieu, tra ly do neu sai, tra ma SV/GS vua cap
    std::string error, personId;
    bool ok = m_auth.registerAccount(
        username.toStdString(), password.toStdString(), role.toStdString(),
        fullName.toStdString(), phone.toStdString(), email.toStdString(), &error, &personId
    );

    if (ok) {
        // bao thanh cong, ve trang dang nhap
        regErrorLabel->setStyleSheet("color: #16a34a; background-color: #f0fdf4; border: 1px solid #bbf7d0;");
        regErrorLabel->setText("Tao tai khoan thanh cong! Ma cua ban: " + QString::fromStdString(personId));

        regUsernameInput->clear();
        regFullNameInput->clear();
        regPhoneInput->clear();
        regEmailInput->clear();
        regPasswordInput->clear();
        regConfirmInput->clear();

        loginUsernameInput->setText(username);
        loginPasswordInput->clear();
        loginPasswordInput->setFocus();

        // sau 2 giay ve trang dang nhap (du thoi gian doc ma)
        QTimer::singleShot(2000, this, &LoginWindow::onShowLoginPage);
    } else {
        regErrorLabel->setText(QString::fromStdString(error));
    }
}

void LoginWindow::onGuestMode() {
    accept();
}

void LoginWindow::onShowRegisterPage() {
    loginErrorLabel->clear();
    regErrorLabel->clear();
    stackedWidget->setCurrentIndex(1);
    resizeForPage(1);
}

void LoginWindow::onShowLoginPage() {
    regErrorLabel->clear();
    loginErrorLabel->clear();
    stackedWidget->setCurrentIndex(0);
    resizeForPage(0);
}

// trang dang ky nhieu o nen lon hon
void LoginWindow::resizeForPage(int pageIndex) {
    if (pageIndex == 1) setFixedSize(640, 640);
    else                setFixedSize(480, 500);

    QScreen* screen = QApplication::primaryScreen();
    QRect sg = screen->availableGeometry();
    move(sg.center() - rect().center());
}
