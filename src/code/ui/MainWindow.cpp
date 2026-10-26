#include "../../include/ui/MainWindow.h"
#include "../../include/ui/LoginWindow.h"
#include <QApplication>
#include <QGroupBox>
#include <QFormLayout>
#include <QSplitter>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(AuthService* auth, QWidget* parent)
    : QMainWindow(parent), m_auth(auth), m_currentRole("Guest"), m_currentPersonId("") {
    if (m_auth && m_auth->isLoggedIn()) {
        m_currentRole     = QString::fromStdString(m_auth->getCurrentRole());
        m_currentPersonId = QString::fromStdString(m_auth->getCurrentPersonId());
    }
    setupUi();
    applyStyles();
    seedSampleData();
    refreshAllTables();
    applyRolePermissions();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
    setWindowTitle("He Thong Quan Ly Trung Tam Gia Su - PBL2");
    resize(1200, 780);

    // Tao tat ca 4 tab truoc, applyRolePermissions se xoa tab khong can
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(createUsersTab(),    "Quan Ly Nguoi Dung");
    tabWidget->addTab(createBookingTab(),  "Tim Kiem & Dang Ky");
    tabWidget->addTab(createTutorTab(),    "Gia Su & Phe Duyet");
    tabWidget->addTab(createOverviewTab(), "Lop Cua Toi & Hop Dong");

    setCentralWidget(tabWidget);

    // Toolbar: hien thi thong tin user + nut Dang Xuat
    QToolBar* toolBar = addToolBar("Main Toolbar");
    toolBar->setMovable(false);
    toolBar->setFloatable(false);

    QString userInfo = (m_currentRole == "Guest")
        ? "Dang nhap: Khach (chi xem)"
        : "Dang nhap: " + m_currentRole +
          (m_currentPersonId.isEmpty() ? "" : "  |  Ma dinh danh: " + m_currentPersonId);

    QLabel* userInfoLabel = new QLabel("  " + userInfo + "  ");
    userInfoLabel->setObjectName("toolbarLabel");
    toolBar->addWidget(userInfoLabel);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(spacer);

    QPushButton* btnLogout = new QPushButton("Dang Xuat");
    btnLogout->setObjectName("btnLogout");
    btnLogout->setCursor(Qt::PointingHandCursor);
    connect(btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    toolBar->addWidget(btnLogout);

    QString statusMsg = (m_currentRole == "Guest")
        ? "Che do Khach - Chi xem, khong the dang ky hay thao tac."
        : "Da dang nhap voi vai tro: " + m_currentRole + "   |   Ma dinh danh: " + m_currentPersonId;
    statusBar()->showMessage(statusMsg);
}

void MainWindow::applyStyles() {
    QString qss = R"(
        QMainWindow {
            background-color: #f8fafc;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 13px;
        }
        QToolBar {
            background-color: #1e293b;
            border-bottom: 1px solid #334155;
            padding: 4px 8px;
            spacing: 6px;
        }
        QLabel#toolbarLabel {
            color: #e2e8f0;
            font-size: 13px;
        }
        QPushButton#btnLogout {
            background-color: #dc2626;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 6px 20px;
            font-weight: bold;
            font-size: 13px;
        }
        QPushButton#btnLogout:hover { background-color: #b91c1c; }
        QTabWidget::pane {
            border: 1px solid #e2e8f0;
            background: #ffffff;
            border-radius: 0 6px 6px 6px;
            top: -1px;
        }
        QTabBar::tab {
            background: #e2e8f0;
            color: #475569;
            padding: 10px 24px;
            margin-right: 3px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            font-weight: 600;
            font-size: 13px;
        }
        QTabBar::tab:selected {
            background: #1e40af;
            color: #ffffff;
        }
        QTabBar::tab:hover:!selected {
            background: #cbd5e1;
        }
        QGroupBox {
            font-weight: bold;
            font-size: 13px;
            color: #1e293b;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            margin-top: 14px;
            padding-top: 18px;
            background-color: #ffffff;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
            color: #1e40af;
        }
        QLineEdit, QComboBox {
            border: 1.5px solid #cbd5e1;
            border-radius: 6px;
            padding: 7px 12px;
            background-color: #f8fafc;
            color: #0f172a;
            min-height: 34px;
            font-size: 13px;
        }
        QLineEdit:focus, QComboBox:focus {
            border-color: #2563eb;
            background-color: #ffffff;
        }
        QLineEdit[readOnly="true"] {
            background-color: #f1f5f9;
            color: #64748b;
        }
        QPushButton {
            background-color: #1d4ed8;
            color: #ffffff;
            border-radius: 6px;
            padding: 8px 18px;
            font-weight: bold;
            font-size: 13px;
            border: none;
        }
        QPushButton:hover { background-color: #1e40af; }
        QPushButton:pressed { background-color: #1e3a8a; }
        QPushButton#btnSuccess { background-color: #059669; }
        QPushButton#btnSuccess:hover { background-color: #047857; }
        QPushButton#btnDanger { background-color: #dc2626; }
        QPushButton#btnDanger:hover { background-color: #b91c1c; }
        QPushButton#btnSecondary { background-color: #64748b; }
        QPushButton#btnSecondary:hover { background-color: #475569; }
        QTableWidget {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            gridline-color: #f1f5f9;
            border-radius: 6px;
            selection-background-color: #dbeafe;
            selection-color: #1e3a8a;
            font-size: 13px;
        }
        QHeaderView::section {
            background-color: #1e293b;
            color: #ffffff;
            padding: 9px 8px;
            border: none;
            font-weight: bold;
            font-size: 13px;
        }
        QLabel {
            font-size: 13px;
            color: #374151;
        }
        QStatusBar {
            background-color: #f1f5f9;
            color: #475569;
            font-size: 12px;
            border-top: 1px solid #e2e8f0;
        }
    )";
    qApp->setStyleSheet(qss);
}

// ------------------- TAB 1: QUAN LY NGUOI DUNG -------------------
QWidget* MainWindow::createUsersTab() {
    QWidget* tab = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(tab);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(12);

    // Form Them Hoc Vien
    QGroupBox* stBox = new QGroupBox("Them Hoc Vien Moi");
    QFormLayout* stForm = new QFormLayout(stBox);
    stForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    stForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    stForm->setSpacing(10);

    stGeneratedIdLabel = new QLabel("(Tu dong sinh sau khi luu)");
    stGeneratedIdLabel->setStyleSheet("color:#1d4ed8;font-weight:600;font-style:italic;");
    stNameInput     = new QLineEdit(); stNameInput->setPlaceholderText("VD: Le Van C");
    stGradeInput    = new QLineEdit(); stGradeInput->setPlaceholderText("VD: 12");
    stPhoneInput    = new QLineEdit(); stPhoneInput->setPlaceholderText("VD: 0912345678");
    stSubjectsInput = new QLineEdit(); stSubjectsInput->setPlaceholderText("VD: Toan, Ly");
    stBudgetInput   = new QLineEdit(); stBudgetInput->setPlaceholderText("VD: 150000");

    QPushButton* btnAddSt = new QPushButton("Luu Hoc Vien");
    btnAddSt->setObjectName("btnSuccess");

    stForm->addRow("Ma Hoc Vien:", stGeneratedIdLabel);
    stForm->addRow("Ho va Ten:",   stNameInput);
    stForm->addRow("Lop Hoc:",     stGradeInput);
    stForm->addRow("So DT:",       stPhoneInput);
    stForm->addRow("Mon hoc:",     stSubjectsInput);
    stForm->addRow("Hoc phi:",     stBudgetInput);
    stForm->addRow("",             btnAddSt);
    leftLayout->addWidget(stBox);

    // Form Them Gia Su
    QGroupBox* tuBox = new QGroupBox("Them Gia Su Moi");
    QFormLayout* tuForm = new QFormLayout(tuBox);
    tuForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tuForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    tuForm->setSpacing(10);

    tuGeneratedIdLabel = new QLabel("(Tu dong sinh sau khi luu)");
    tuGeneratedIdLabel->setStyleSheet("color:#1d4ed8;font-weight:600;font-style:italic;");
    tuNameInput     = new QLineEdit(); tuNameInput->setPlaceholderText("VD: Nguyen Thi D");
    tuPhoneInput    = new QLineEdit(); tuPhoneInput->setPlaceholderText("VD: 0987654321");
    tuSubjectsInput = new QLineEdit(); tuSubjectsInput->setPlaceholderText("VD: Toan, Hoa");
    tuExpInput      = new QLineEdit(); tuExpInput->setPlaceholderText("VD: 3");
    tuRateInput     = new QLineEdit(); tuRateInput->setPlaceholderText("VD: 150000");

    QPushButton* btnAddTu = new QPushButton("Luu Gia Su");
    btnAddTu->setObjectName("btnSuccess");

    tuForm->addRow("Ma Gia Su:", tuGeneratedIdLabel);
    tuForm->addRow("Ho va Ten:", tuNameInput);
    tuForm->addRow("So DT:",     tuPhoneInput);
    tuForm->addRow("Mon day:",   tuSubjectsInput);
    tuForm->addRow("KN (nam):",  tuExpInput);
    tuForm->addRow("Hoc phi:",   tuRateInput);
    tuForm->addRow("",           btnAddTu);
    leftLayout->addWidget(tuBox);

    // Panel Phai: Bang hien thi
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(12);

    QGroupBox* tableStBox = new QGroupBox("Danh Sach Hoc Vien");
    QVBoxLayout* v1 = new QVBoxLayout(tableStBox);
    studentTableWidget = new QTableWidget();
    studentTableWidget->setColumnCount(6);
    studentTableWidget->setHorizontalHeaderLabels({"Ma SV", "Ho Ten", "Lop", "SDT", "Mon Hoc", "Trang Thai"});
    studentTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    studentTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v1->addWidget(studentTableWidget);
    rightLayout->addWidget(tableStBox);

    QGroupBox* tableTuBox = new QGroupBox("Danh Sach Gia Su");
    QVBoxLayout* v2 = new QVBoxLayout(tableTuBox);
    tutorTableWidget = new QTableWidget();
    tutorTableWidget->setColumnCount(6);
    tutorTableWidget->setHorizontalHeaderLabels({"Ma GS", "Ho Ten", "SDT", "Mon Day", "Kinh Nghiem", "Hoc Phi"});
    tutorTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tutorTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tutorTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v2->addWidget(tutorTableWidget);
    rightLayout->addWidget(tableTuBox);

    mainLayout->addLayout(leftLayout, 38);
    mainLayout->addLayout(rightLayout, 62);

    connect(btnAddSt, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(btnAddTu, &QPushButton::clicked, this, &MainWindow::onAddTutor);

    return tab;
}

// ------------------- TAB 2: TIM KIEM & DANG KY -------------------
QWidget* MainWindow::createBookingTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(12);
    layout->setContentsMargins(12, 12, 12, 12);

    // Thanh tim kiem
    QGroupBox* searchBox = new QGroupBox("Tra Cuu Khung Gio Theo Mon (Chi hien thi ca hoc AVAILABLE)");
    QHBoxLayout* hSearch = new QHBoxLayout(searchBox);
    hSearch->setSpacing(10);
    searchSubjectInput = new QLineEdit();
    searchSubjectInput->setPlaceholderText("Nhap mon can tim (VD: Toan, Vat Ly, Hoa)...");
    QPushButton* btnSearch = new QPushButton("Tim Kiem");
    QPushButton* btnReset  = new QPushButton("Tat Ca Ca Trong");
    btnReset->setObjectName("btnSecondary");
    hSearch->addWidget(new QLabel("Mon hoc:"));
    hSearch->addWidget(searchSubjectInput, 1);
    hSearch->addWidget(btnSearch);
    hSearch->addWidget(btnReset);
    layout->addWidget(searchBox);

    // Bang ca hoc
    QGroupBox* classBox = new QGroupBox("Cac Ca Hoc Con Trong - Click vao hang de chon");
    QVBoxLayout* vClass = new QVBoxLayout(classBox);
    availableClassesTable = new QTableWidget();
    availableClassesTable->setColumnCount(7);
    availableClassesTable->setHorizontalHeaderLabels({"Ma Lop", "Mon Hoc", "Lich Hoc", "Hoc Phi (VND)", "Ma GS", "Ten Gia Su", "Trang Thai"});
    availableClassesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    availableClassesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    availableClassesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vClass->addWidget(availableClassesTable);
    layout->addWidget(classBox);

    // Form dang ky
    QGroupBox* bookBox = new QGroupBox("Dang Ky Ca Hoc");
    QFormLayout* bookForm = new QFormLayout(bookBox);
    bookForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    bookForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    bookForm->setSpacing(10);

    selectedClassIdInput = new QLineEdit();
    selectedClassIdInput->setReadOnly(true);
    selectedClassIdInput->setPlaceholderText("(Click vao hang o bang tren de chon lop)");

    bookingStudentIdInput = new QLineEdit();
    bookingStudentIdInput->setPlaceholderText("Ma Hoc Vien (VD: SV001)");

    QPushButton* btnBook = new QPushButton("Dang Ky Ca Hoc Nay");
    btnBook->setObjectName("btnSuccess");

    bookForm->addRow("Lop Da Chon:", selectedClassIdInput);
    bookForm->addRow("Ma Hoc Vien:", bookingStudentIdInput);
    bookForm->addRow("",             btnBook);
    layout->addWidget(bookBox);

    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::onSearchAvailableClasses);
    connect(btnReset,  &QPushButton::clicked, this, &MainWindow::onResetSearchAvailableClasses);
    connect(availableClassesTable, &QTableWidget::cellClicked, this, &MainWindow::onAvailableClassSelected);
    connect(btnBook,   &QPushButton::clicked, this, &MainWindow::onBookClass);

    return tab;
}

// ------------------- TAB 3: GIA SU & PHE DUYET -------------------
QWidget* MainWindow::createTutorTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(12);
    layout->setContentsMargins(12, 12, 12, 12);

    // Form Mo ca day moi
    QGroupBox* openBox = new QGroupBox("Mo Khung Gio Day Moi");
    QFormLayout* openForm = new QFormLayout(openBox);
    openForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    openForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    openForm->setSpacing(10);

    QLabel* autoIdNote = new QLabel("(He thong tu dong sinh)");
    autoIdNote->setStyleSheet("color:#1d4ed8;font-style:italic;font-weight:600;");

    openTutorIdInput   = new QLineEdit(); openTutorIdInput->setPlaceholderText("Ma Gia Su (VD: GS001)");
    openSubjectInput   = new QLineEdit(); openSubjectInput->setPlaceholderText("Mon hoc (VD: Toan 12)");
    openScheduleInput  = new QLineEdit(); openScheduleInput->setPlaceholderText("Lich hoc (VD: T2-T4 (18:00-20:00)) - Phut phai la 00,15,30,45");
    openFeeInput       = new QLineEdit(); openFeeInput->setPlaceholderText("Hoc phi moi buoi (VD: 140000)");

    QPushButton* btnOpen = new QPushButton("Mo Ca Day");
    btnOpen->setObjectName("btnSuccess");

    openForm->addRow("Ma Lop (tu sinh):", autoIdNote);
    openForm->addRow("Ma Gia Su:",        openTutorIdInput);
    openForm->addRow("Mon Hoc:",          openSubjectInput);
    openForm->addRow("Lich Hoc:",         openScheduleInput);
    openForm->addRow("Hoc Phi/Buoi:",     openFeeInput);
    openForm->addRow("",                  btnOpen);
    layout->addWidget(openBox);

    // Bang yeu cau cho duyet
    QGroupBox* pendingBox = new QGroupBox("Yeu Cau Cho Duyet (PENDING)");
    QVBoxLayout* vPending = new QVBoxLayout(pendingBox);

    QHBoxLayout* hFilter = new QHBoxLayout();
    hFilter->setSpacing(10);
    tutorIdFilterInput = new QLineEdit();
    tutorIdFilterInput->setPlaceholderText("Ma Gia Su (VD: GS001)...");
    QPushButton* btnFilter = new QPushButton("Xem Yeu Cau");
    hFilter->addWidget(new QLabel("Ma Gia Su:"));
    hFilter->addWidget(tutorIdFilterInput, 1);
    hFilter->addWidget(btnFilter);
    vPending->addLayout(hFilter);

    pendingRequestsTable = new QTableWidget();
    pendingRequestsTable->setColumnCount(7);
    pendingRequestsTable->setHorizontalHeaderLabels({"Ma Lop", "Mon Hoc", "Lich Hoc", "Ma Hoc Vien", "Ten Hoc Vien", "SDT", "Trang Thai"});
    pendingRequestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pendingRequestsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pendingRequestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vPending->addWidget(pendingRequestsTable);

    QHBoxLayout* hActions = new QHBoxLayout();
    hActions->setSpacing(10);
    QPushButton* btnApprove = new QPushButton("Phe Duyet (Kich Hoat Lop & Tao Hop Dong)");
    btnApprove->setObjectName("btnSuccess");
    QPushButton* btnReject  = new QPushButton("Tu Choi (Mo Lai Khung Gio)");
    btnReject->setObjectName("btnDanger");
    hActions->addWidget(btnApprove);
    hActions->addWidget(btnReject);
    vPending->addLayout(hActions);

    layout->addWidget(pendingBox);

    connect(btnOpen,    &QPushButton::clicked, this, &MainWindow::onOpenClass);
    connect(btnFilter,  &QPushButton::clicked, this, &MainWindow::onFilterPendingRequests);
    connect(btnApprove, &QPushButton::clicked, this, &MainWindow::onApproveRequest);
    connect(btnReject,  &QPushButton::clicked, this, &MainWindow::onRejectRequest);

    return tab;
}

// ------------------- TAB 4: LOP CUA TOI & HOP DONG -------------------
QWidget* MainWindow::createOverviewTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(12);
    layout->setContentsMargins(12, 12, 12, 12);

    QGroupBox* myBox = new QGroupBox("Tra Cuu Lop Hoc Cua Ca Nhan");
    QVBoxLayout* vMy = new QVBoxLayout(myBox);

    QHBoxLayout* hMy = new QHBoxLayout();
    hMy->setSpacing(10);
    myUserIdInput = new QLineEdit();
    myUserIdInput->setPlaceholderText("Ma SV hoac Ma GS (VD: SV001, GS001)...");
    myUserRoleCombo = new QComboBox();
    myUserRoleCombo->addItems({"Hoc Vien (Student)", "Gia Su (Tutor)"});
    QPushButton* btnMy = new QPushButton("Tra Cuu Lop Cua Toi");

    hMy->addWidget(new QLabel("Vai tro:"));
    hMy->addWidget(myUserRoleCombo);
    hMy->addWidget(new QLabel("Ma:"));
    hMy->addWidget(myUserIdInput, 1);
    hMy->addWidget(btnMy);
    vMy->addLayout(hMy);

    myClassesTable = new QTableWidget();
    myClassesTable->setColumnCount(7);
    myClassesTable->setHorizontalHeaderLabels({"Ma Lop", "Mon Hoc", "Lich Hoc", "Hoc Phi", "Doi Tac", "Trang Thai", "Ma Hop Dong"});
    myClassesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    myClassesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vMy->addWidget(myClassesTable);
    layout->addWidget(myBox);

    QGroupBox* ctBox = new QGroupBox("Toan Bo Hop Dong Trong He Thong");
    QVBoxLayout* vCt = new QVBoxLayout(ctBox);
    contractsTableWidget = new QTableWidget();
    contractsTableWidget->setColumnCount(6);
    contractsTableWidget->setHorizontalHeaderLabels({"Ma Hop Dong", "Ma Hoc Vien", "Ma Gia Su", "Mon Hoc", "Lich Trinh", "Trang Thai"});
    contractsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    contractsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vCt->addWidget(contractsTableWidget);
    layout->addWidget(ctBox);

    connect(btnMy, &QPushButton::clicked, this, &MainWindow::onFilterMyClasses);

    return tab;
}

// ------------------- DU LIEU KHOI TAO BAN DAU -------------------
void MainWindow::seedSampleData() {
    Student* s1 = new Student("SV001", "Nguyen Van An", "12", "0901234567",
                              "an@gmail.com", "123 Le Loi, Da Nang", "Nam",
                              "15/03/2007", "Toan,Ly", "T2,T4,T6", 150000);
    Student* s2 = new Student("SV002", "Tran Thi Binh", "11", "0909876543",
                              "binh@gmail.com", "456 Tran Phu, Da Nang", "Nu",
                              "20/08/2008", "Toan,Hoa", "T3,T5,T7", 120000);
    service.addStudent(s1);
    service.addStudent(s2);

    Tutor* t1 = new Tutor("GS001", "Pham Hoang Vu", "0934567890", "vu@gmail.com",
                          "789 Nguyen Luong Bang", "Nam", "25/12/1998",
                          "Toan,Ly", "Dai hoc Bach Khoa", 3, 150000, "T2,T4,T6");
    Tutor* t2 = new Tutor("GS002", "Le Thi Mai", "0912345678", "mai@gmail.com",
                          "102 Dien Bien Phu", "Nu", "10/05/1999",
                          "Toan,Hoa", "Dai hoc Su Pham", 2, 130000, "T3,T5,T7");
    service.addTutor(t1);
    service.addTutor(t2);

    service.tutorOpenClass("GS001", "Toan 12", "T2-T4 (18:00-20:00)", 150000, "01/11/2026", "31/01/2027");
    service.tutorOpenClass("GS001", "Toan 12", "T3-T5 (19:00-21:00)", 150000, "01/11/2026", "31/01/2027");
    service.tutorOpenClass("GS002", "Toan 11", "T7-CN (08:00-10:00)", 130000, "05/11/2026", "05/02/2027");
    service.tutorOpenClass("GS001", "Vat Ly 12", "T6-CN (18:00-20:00)", 140000, "01/11/2026", "31/01/2027");
}

// ------------------- SLOTS & XU LY SU KIEN -------------------

void MainWindow::onAddStudent() {
    string name     = stNameInput->text().trimmed().toStdString();
    string grade    = stGradeInput->text().trimmed().toStdString();
    string phone    = stPhoneInput->text().trimmed().toStdString();
    string subjects = stSubjectsInput->text().trimmed().toStdString();
    double budget   = stBudgetInput->text().trimmed().toDouble();

    if (name.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap Ho va Ten!");
        return;
    }

    Student* st = new Student("", name, grade, phone, "", "", "Nam", "", subjects, "", budget);
    if (service.addStudent(st)) {
        stGeneratedIdLabel->setText(QString::fromStdString(st->getId()));
        QMessageBox::information(this, "Thanh cong",
            QString("Them hoc vien thanh cong!\nMa tu dong sinh: %1")
            .arg(QString::fromStdString(st->getId())));
        stNameInput->clear(); stGradeInput->clear();
        stPhoneInput->clear(); stSubjectsInput->clear(); stBudgetInput->clear();
        refreshAllTables();
    } else {
        delete st;
        QMessageBox::critical(this, "Loi", "Them hoc vien that bai!");
    }
}

void MainWindow::onAddTutor() {
    string name     = tuNameInput->text().trimmed().toStdString();
    string phone    = tuPhoneInput->text().trimmed().toStdString();
    string subjects = tuSubjectsInput->text().trimmed().toStdString();
    int    exp      = tuExpInput->text().trimmed().toInt();
    double rate     = tuRateInput->text().trimmed().toDouble();

    if (name.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap Ho va Ten!");
        return;
    }

    Tutor* tu = new Tutor("", name, phone, "", "", "Nam", "", subjects, "", exp, rate, "");
    if (service.addTutor(tu)) {
        tuGeneratedIdLabel->setText(QString::fromStdString(tu->getId()));
        QMessageBox::information(this, "Thanh cong",
            QString("Them gia su thanh cong!\nMa tu dong sinh: %1")
            .arg(QString::fromStdString(tu->getId())));
        tuNameInput->clear(); tuPhoneInput->clear();
        tuSubjectsInput->clear(); tuExpInput->clear(); tuRateInput->clear();
        refreshAllTables();
    } else {
        delete tu;
        QMessageBox::critical(this, "Loi", "Them gia su that bai!");
    }
}

void MainWindow::onSearchAvailableClasses() {
    QString subKey = searchSubjectInput->text().trimmed().toLower();
    availableClassesTable->setRowCount(0);

    const DoublyLinkedList<Class*>& list = service.getClassList();
    Node<Class*>* cur = list.getHead();
    int row = 0;

    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getStatus() == "AVAILABLE") {
            QString cSub = QString::fromStdString(c->getSubject()).toLower();
            if (subKey.isEmpty() || cSub.contains(subKey)) {
                availableClassesTable->insertRow(row);
                availableClassesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(c->getClassId())));
                availableClassesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c->getSubject())));
                availableClassesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(c->getSchedule())));
                availableClassesTable->setItem(row, 3, new QTableWidgetItem(QString::number(c->getTuitionFee())));
                availableClassesTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(c->getTutorId())));
                Tutor* t = service.findTutorById(c->getTutorId());
                QString tName = t ? QString::fromStdString(t->getFullName()) : "N/A";
                availableClassesTable->setItem(row, 5, new QTableWidgetItem(tName));
                availableClassesTable->setItem(row, 6, new QTableWidgetItem("AVAILABLE"));
                row++;
            }
        }
        cur = cur->next;
    }
}

void MainWindow::onResetSearchAvailableClasses() {
    searchSubjectInput->clear();
    onSearchAvailableClasses();
}

void MainWindow::onAvailableClassSelected(int row, int col) {
    Q_UNUSED(col);
    if (row >= 0 && row < availableClassesTable->rowCount()) {
        selectedClassIdInput->setText(availableClassesTable->item(row, 0)->text());
    }
}

void MainWindow::onBookClass() {
    string classId   = selectedClassIdInput->text().trimmed().toStdString();
    string studentId = bookingStudentIdInput->text().trimmed().toStdString();

    if (classId.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon ca hoc tu danh sach!");
        return;
    }
    if (studentId.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap Ma Hoc Vien!");
        return;
    }

    if (service.studentBookClass(studentId, classId)) {
        QMessageBox::information(this, "Thanh cong",
            QString("Dang ky ca hoc %1 thanh cong!\nCa hoc da chuyen sang PENDING va an khoi hoc vien khac.")
            .arg(QString::fromStdString(classId)));
        selectedClassIdInput->clear();
        refreshAllTables();
    } else {
        QMessageBox::critical(this, "Loi", "Dang ky that bai! Kiem tra lai Ma Hoc Vien hoac ca hoc da bi chon.");
    }
}

void MainWindow::onOpenClass() {
    string tutorId  = openTutorIdInput->text().trimmed().toStdString();
    string subject  = openSubjectInput->text().trimmed().toStdString();
    string schedule = openScheduleInput->text().trimmed().toStdString();
    double fee      = openFeeInput->text().trimmed().toDouble();

    if (tutorId.empty() || subject.empty() || schedule.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap day du Ma GS, Mon hoc va Lich hoc!");
        return;
    }

    string newClassId;
    if (service.tutorOpenClass(tutorId, subject, schedule, fee, "", "", &newClassId)) {
        QMessageBox::information(this, "Thanh cong",
            QString("Mo ca day thanh cong!\nMa lop tu dong sinh: %1")
            .arg(QString::fromStdString(newClassId)));
        openSubjectInput->clear();
        openScheduleInput->clear();
        openFeeInput->clear();
        refreshAllTables();
    } else {
        QMessageBox::critical(this, "Loi",
            "Mo ca day that bai!\nKiem tra: Ma GS hop le, Lich hoc dung dinh dang HH:MM (VD: 18:00-20:00), Hoc phi > 0");
    }
}

void MainWindow::onFilterPendingRequests() {
    string tutorId = tutorIdFilterInput->text().trimmed().toStdString();
    pendingRequestsTable->setRowCount(0);

    if (tutorId.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap Ma Gia Su!");
        return;
    }

    const DoublyLinkedList<Class*>& list = service.getClassList();
    Node<Class*>* cur = list.getHead();
    int row = 0;

    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getTutorId() == tutorId && c->getStatus() == "PENDING") {
            Student* s = service.findStudentById(c->getStudentId());
            pendingRequestsTable->insertRow(row);
            pendingRequestsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(c->getClassId())));
            pendingRequestsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c->getSubject())));
            pendingRequestsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(c->getSchedule())));
            pendingRequestsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(c->getStudentId())));
            pendingRequestsTable->setItem(row, 4, new QTableWidgetItem(s ? QString::fromStdString(s->getFullName()) : "N/A"));
            pendingRequestsTable->setItem(row, 5, new QTableWidgetItem(s ? QString::fromStdString(s->getPhone()) : "N/A"));
            pendingRequestsTable->setItem(row, 6, new QTableWidgetItem("PENDING"));
            row++;
        }
        cur = cur->next;
    }

    if (row == 0) {
        statusBar()->showMessage("Gia su " + QString::fromStdString(tutorId) + " hien khong co yeu cau PENDING nao.");
    }
}

void MainWindow::onApproveRequest() {
    int row = pendingRequestsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon mot yeu cau tu danh sach!");
        return;
    }

    string classId = pendingRequestsTable->item(row, 0)->text().toStdString();
    // Lay tutorId tu hang hien tai hoac tu person dang nhap
    string tutorId  = tutorIdFilterInput->text().trimmed().toStdString();
    if (tutorId.empty()) tutorId = m_currentPersonId.toStdString();
    if (service.tutorApproveClass(tutorId, classId)) {
        QMessageBox::information(this, "Thanh cong",
            QString("Da phe duyet lop %1 va tao hop dong!").arg(QString::fromStdString(classId)));
        tutorIdFilterInput->clear();
        pendingRequestsTable->setRowCount(0);
        refreshAllTables();
    } else {
        QMessageBox::critical(this, "Loi", "Phe duyet that bai!");
    }
}

void MainWindow::onRejectRequest() {
    int row = pendingRequestsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon mot yeu cau tu danh sach!");
        return;
    }

    string classId = pendingRequestsTable->item(row, 0)->text().toStdString();
    string tutorId  = tutorIdFilterInput->text().trimmed().toStdString();
    if (tutorId.empty()) tutorId = m_currentPersonId.toStdString();
    if (service.tutorRejectClass(tutorId, classId)) {
        QMessageBox::information(this, "Thong bao",
            QString("Da tu choi yeu cau lop %1. Khung gio da mo lai.").arg(QString::fromStdString(classId)));
        tutorIdFilterInput->clear();
        pendingRequestsTable->setRowCount(0);
        refreshAllTables();
    } else {
        QMessageBox::critical(this, "Loi", "Tu choi that bai!");
    }
}

void MainWindow::onFilterMyClasses() {
    string userId = myUserIdInput->text().trimmed().toStdString();
    bool isStudent = (myUserRoleCombo->currentIndex() == 0);
    myClassesTable->setRowCount(0);

    if (userId.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap Ma dinh danh!");
        return;
    }

    const DoublyLinkedList<Class*>& list = service.getClassList();
    Node<Class*>* cur = list.getHead();
    int row = 0;

    while (cur != nullptr) {
        Class* c = cur->data;
        bool match = isStudent
            ? (c->getStudentId() == userId)
            : (c->getTutorId()   == userId);

        if (match && c->getStatus() != "AVAILABLE") {
            myClassesTable->insertRow(row);
            myClassesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(c->getClassId())));
            myClassesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c->getSubject())));
            myClassesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(c->getSchedule())));
            myClassesTable->setItem(row, 3, new QTableWidgetItem(QString::number(c->getTuitionFee())));

            QString partner;
            if (isStudent) {
                Tutor* t = service.findTutorById(c->getTutorId());
                partner = t ? QString::fromStdString(t->getFullName()) : QString::fromStdString(c->getTutorId());
            } else {
                Student* s = service.findStudentById(c->getStudentId());
                partner = s ? QString::fromStdString(s->getFullName()) : QString::fromStdString(c->getStudentId());
            }
            myClassesTable->setItem(row, 4, new QTableWidgetItem(partner));
            myClassesTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(c->getStatus())));
            myClassesTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(c->getContractId())));
            row++;
        }
        cur = cur->next;
    }
}

void MainWindow::refreshAllTables() {
    // Bang Hoc Vien
    studentTableWidget->setRowCount(0);
    const DoublyLinkedList<Student*>& sList = service.getStudentList();
    Node<Student*>* sCur = sList.getHead();
    int sRow = 0;
    while (sCur != nullptr) {
        Student* s = sCur->data;
        studentTableWidget->insertRow(sRow);
        studentTableWidget->setItem(sRow, 0, new QTableWidgetItem(QString::fromStdString(s->getId())));
        studentTableWidget->setItem(sRow, 1, new QTableWidgetItem(QString::fromStdString(s->getFullName())));
        studentTableWidget->setItem(sRow, 2, new QTableWidgetItem(QString::fromStdString(s->getGrade())));
        studentTableWidget->setItem(sRow, 3, new QTableWidgetItem(QString::fromStdString(s->getPhone())));
        studentTableWidget->setItem(sRow, 4, new QTableWidgetItem(QString::fromStdString(s->getSubjects())));
        studentTableWidget->setItem(sRow, 5, new QTableWidgetItem("Active"));
        sRow++;
        sCur = sCur->next;
    }

    // Bang Gia Su
    tutorTableWidget->setRowCount(0);
    const DoublyLinkedList<Tutor*>& tList = service.getTutorList();
    Node<Tutor*>* tCur = tList.getHead();
    int tRow = 0;
    while (tCur != nullptr) {
        Tutor* t = tCur->data;
        tutorTableWidget->insertRow(tRow);
        tutorTableWidget->setItem(tRow, 0, new QTableWidgetItem(QString::fromStdString(t->getId())));
        tutorTableWidget->setItem(tRow, 1, new QTableWidgetItem(QString::fromStdString(t->getFullName())));
        tutorTableWidget->setItem(tRow, 2, new QTableWidgetItem(QString::fromStdString(t->getPhone())));
        tutorTableWidget->setItem(tRow, 3, new QTableWidgetItem(QString::fromStdString(t->getSubjects())));
        tutorTableWidget->setItem(tRow, 4, new QTableWidgetItem(QString::number(t->getExpYears()) + " nam"));
        tutorTableWidget->setItem(tRow, 5, new QTableWidgetItem(QString::number(t->getHourlyRate()) + " VND"));
        tRow++;
        tCur = tCur->next;
    }

    // Ca hoc
    onSearchAvailableClasses();

    // Bang Hop Dong
    contractsTableWidget->setRowCount(0);
    const DoublyLinkedList<Contract*>& cList = service.getContractList();
    Node<Contract*>* cCur = cList.getHead();
    int cRow = 0;
    while (cCur != nullptr) {
        Contract* ct = cCur->data;
        contractsTableWidget->insertRow(cRow);
        contractsTableWidget->setItem(cRow, 0, new QTableWidgetItem(QString::fromStdString(ct->getContractId())));
        contractsTableWidget->setItem(cRow, 1, new QTableWidgetItem(QString::fromStdString(ct->getStudentId())));
        contractsTableWidget->setItem(cRow, 2, new QTableWidgetItem(QString::fromStdString(ct->getTutorId())));
        contractsTableWidget->setItem(cRow, 3, new QTableWidgetItem(QString::fromStdString(ct->getSubjects())));
        contractsTableWidget->setItem(cRow, 4, new QTableWidgetItem(
            QString::fromStdString(ct->getStartDate()) + " -> " + QString::fromStdString(ct->getEndDate())));
        contractsTableWidget->setItem(cRow, 5, new QTableWidgetItem(QString::fromStdString(ct->getStatus())));
        cRow++;
        cCur = cCur->next;
    }
}

// ------------------- PHAN QUYEN THEO ROLE -------------------
void MainWindow::applyRolePermissions() {
    // Luu lai cac widget tab (da tao san truoc applyRolePermissions)
    // Chien luoc: removeTab hoan toan de an tab
    if (m_currentRole == "Student") {
        // Student: Tab 1 (Tim Kiem) + Tab 3 (Lop Cua Toi)
        // Xoa theo thu tu giam dan de khong bi lech index
        tabWidget->removeTab(2);   // Gia Su & Phe Duyet (index 2)
        tabWidget->removeTab(0);   // Quan Ly Nguoi Dung (index 0)
        // Con lai: idx 0 = Tim Kiem, idx 1 = Lop Cua Toi
        tabWidget->setCurrentIndex(0);

        // Tu dien ID
        if (!m_currentPersonId.isEmpty()) {
            bookingStudentIdInput->setText(m_currentPersonId);
            myUserIdInput->setText(m_currentPersonId);
            myUserRoleCombo->setCurrentIndex(0);
        }
    } else if (m_currentRole == "Tutor") {
        // Tutor: Tab 2 (Gia Su) + Tab 3 (Lop Cua Toi)
        tabWidget->removeTab(1);   // Tim Kiem (index 1)
        tabWidget->removeTab(0);   // Quan Ly Nguoi Dung (index 0)
        // Con lai: idx 0 = Gia Su, idx 1 = Lop Cua Toi
        tabWidget->setCurrentIndex(0);

        if (!m_currentPersonId.isEmpty()) {
            openTutorIdInput->setText(m_currentPersonId);
            tutorIdFilterInput->setText(m_currentPersonId);
            myUserIdInput->setText(m_currentPersonId);
            myUserRoleCombo->setCurrentIndex(1);
        }
    } else if (m_currentRole == "Admin") {
        // Admin: Toan quyen
        tabWidget->setCurrentIndex(0);
    } else {
        // Guest: Chi Tim Kiem + Lop Cua Toi, khong duoc thao tac
        tabWidget->removeTab(2);
        tabWidget->removeTab(0);
        tabWidget->setCurrentIndex(0);
        bookingStudentIdInput->setReadOnly(true);
        bookingStudentIdInput->setPlaceholderText("(Can dang nhap de dang ky)");
    }
}

// ------------------- DANG XUAT -------------------
void MainWindow::onLogout() {
    if (m_auth) m_auth->logout();

    AuthService* authPtr = m_auth;
    this->hide();

    LoginWindow* loginWin = new LoginWindow(authPtr ? *authPtr : *(new AuthService()));
    if (loginWin->exec() == QDialog::Accepted) {
        MainWindow* newMain = loginWin->isGuestMode()
            ? new MainWindow(nullptr)
            : new MainWindow(authPtr);
        newMain->show();
    }
    delete loginWin;
    this->close();
}
