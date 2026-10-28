#include "../../include/ui/MainWindow.h"
#include "../../include/ui/LoginWindow.h"
#include "../../include/ui/SchedulePicker.h"
#include "../../include/services/ProfileSync.h"
#include "../../include/helper/Validator.h"
#include "../../include/helper/UiHelper.h"
#include <QApplication>
#include <QGroupBox>
#include <QFormLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QSignalBlocker>
#include <QPlainTextEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(AuthService* auth, CenterService& svc, QWidget* parent)
    : QMainWindow(parent), service(svc), m_auth(auth), m_currentRole("Guest"),
      m_currentPersonId(""), m_currentName("Khach"), m_profileMissing(false) {
    setAttribute(Qt::WA_DeleteOnClose);

    if (m_auth && m_auth->isLoggedIn()) {
        // cap ma SV/GS va tao ho so cho tai khoan moi
        syncProfilesFromAccounts(*m_auth, service);

        m_currentRole     = UiHelper::toQString(m_auth->getCurrentRole());
        m_currentPersonId = UiHelper::toQString(m_auth->getCurrentPersonId());
        m_currentName     = UiHelper::toQString(m_auth->getCurrentDisplayName());

        // uu tien ten trong ho so
        std::string pid = m_currentPersonId.toStdString();
        if (m_currentRole == "Student") {
            Student* st = service.findStudentById(pid);
            if (st) m_currentName = UiHelper::toQString(st->getFullName());
            else m_profileMissing = true;
        } else if (m_currentRole == "Tutor") {
            Tutor* tu = service.findTutorById(pid);
            if (tu) m_currentName = UiHelper::toQString(tu->getFullName());
            else m_profileMissing = true;
        }
    }

    setupUi();
    applyStyles();
    refreshAllTables();
    applyRolePermissions();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
    setWindowTitle("He Thong Quan Ly Trung Tam Gia Su - PBL2");
    resize(1200, 780);

    // tao du cac tab, applyRolePermissions se bo tab khong can
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(createUsersTab(),    "Quan Ly Nguoi Dung");
    tabWidget->addTab(createBookingTab(),  "Tim Kiem & Dang Ky");
    tabWidget->addTab(createTutorTab(),    "Gia Su & Phe Duyet");
    tabWidget->addTab(createOverviewTab(), "Lop Cua Toi & Hop Dong");
    tabWidget->addTab(createProfileTab(),  "Ho So Ca Nhan");

    setCentralWidget(tabWidget);

    // toolbar: thong tin user + nut Dang Xuat
    QToolBar* toolBar = addToolBar("Main Toolbar");
    toolBar->setMovable(false);
    toolBar->setFloatable(false);

    userInfoLabel = new QLabel();
    userInfoLabel->setObjectName("toolbarLabel");
    userInfoLabel->setText(identityText());
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
    if (m_profileMissing) {
        statusMsg = "Tai khoan chua co ho so " + m_currentPersonId +
                    " (thieu SDT/email hop le). Lien he Admin de bo sung thong tin.";
    }
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

// tab quan ly nguoi dung
QWidget* MainWindow::createUsersTab() {
    QWidget* tab = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(tab);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(12);

    // form them hoc vien
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
    stEmailInput    = new QLineEdit(); stEmailInput->setPlaceholderText("VD: abc@gmail.com");
    stSubjectsInput = new QLineEdit(); stSubjectsInput->setPlaceholderText("VD: Toan, Ly");
    stBudgetInput   = new QLineEdit(); stBudgetInput->setPlaceholderText("VD: 150000");

    QPushButton* btnAddSt = new QPushButton("Luu Hoc Vien");
    btnAddSt->setObjectName("btnSuccess");

    stForm->addRow("Ma Hoc Vien:", stGeneratedIdLabel);
    stForm->addRow("Ho va Ten:",   stNameInput);
    stForm->addRow("Lop Hoc:",     stGradeInput);
    stForm->addRow("So DT:",       stPhoneInput);
    stForm->addRow("Email:",       stEmailInput);
    stForm->addRow("Mon hoc:",     stSubjectsInput);
    stForm->addRow("Hoc phi:",     stBudgetInput);
    stForm->addRow("",             btnAddSt);
    leftLayout->addWidget(stBox);

    // form them gia su
    QGroupBox* tuBox = new QGroupBox("Them Gia Su Moi");
    QFormLayout* tuForm = new QFormLayout(tuBox);
    tuForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tuForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    tuForm->setSpacing(10);

    tuGeneratedIdLabel = new QLabel("(Tu dong sinh sau khi luu)");
    tuGeneratedIdLabel->setStyleSheet("color:#1d4ed8;font-weight:600;font-style:italic;");
    tuNameInput     = new QLineEdit(); tuNameInput->setPlaceholderText("VD: Nguyen Thi D");
    tuPhoneInput    = new QLineEdit(); tuPhoneInput->setPlaceholderText("VD: 0987654321");
    tuEmailInput    = new QLineEdit(); tuEmailInput->setPlaceholderText("VD: abc@gmail.com");
    tuSubjectsInput = new QLineEdit(); tuSubjectsInput->setPlaceholderText("VD: Toan, Hoa");
    tuExpInput      = new QLineEdit(); tuExpInput->setPlaceholderText("VD: 3");

    QPushButton* btnAddTu = new QPushButton("Luu Gia Su");
    btnAddTu->setObjectName("btnSuccess");

    tuForm->addRow("Ma Gia Su:", tuGeneratedIdLabel);
    tuForm->addRow("Ho va Ten:", tuNameInput);
    tuForm->addRow("So DT:",     tuPhoneInput);
    tuForm->addRow("Email:",     tuEmailInput);
    tuForm->addRow("Mon day:",   tuSubjectsInput);
    tuForm->addRow("KN (nam):",  tuExpInput);
    tuForm->addRow("",           btnAddTu);
    leftLayout->addWidget(tuBox);

    // panel phai: cac bang
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(12);

    QGroupBox* tableStBox = new QGroupBox("Danh Sach Hoc Vien");
    QVBoxLayout* v1 = new QVBoxLayout(tableStBox);
    studentTableWidget = new QTableWidget();
    studentTableWidget->setColumnCount(10);
    studentTableWidget->setHorizontalHeaderLabels({"Ma SV", "Ho Ten", "Gioi Tinh", "Ngay Sinh", "Lop", "SDT",
                                                   "Email", "Dia Chi", "Mon Hoc", "Hoc Phi Mong Muon"});
    studentTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    studentTableWidget->horizontalHeader()->setStretchLastSection(true);
    studentTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v1->addWidget(studentTableWidget);
    rightLayout->addWidget(tableStBox);

    QGroupBox* tableTuBox = new QGroupBox("Danh Sach Gia Su");
    QVBoxLayout* v2 = new QVBoxLayout(tableTuBox);
    tutorTableWidget = new QTableWidget();
    tutorTableWidget->setColumnCount(11);
    tutorTableWidget->setHorizontalHeaderLabels({"Ma GS", "Ho Ten", "Gioi Tinh", "Ngay Sinh", "SDT", "Email",
                                                 "Dia Chi", "Mon Day", "Bang Cap", "Kinh Nghiem", "Thanh Tich"});
    tutorTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tutorTableWidget->horizontalHeader()->setStretchLastSection(true);
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

// tab tim kiem & dang ky
QWidget* MainWindow::createBookingTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(12);
    layout->setContentsMargins(12, 12, 12, 12);

    // thanh tim kiem
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

    // bang ca hoc
    QGroupBox* classBox = new QGroupBox("Cac Ca Hoc Con Trong - Click vao hang de chon");
    QVBoxLayout* vClass = new QVBoxLayout(classBox);
    availableClassesTable = new QTableWidget();
    availableClassesTable->setColumnCount(7);
    availableClassesTable->setHorizontalHeaderLabels({"Ma Lop", "Mon Hoc", "Lich Hoc", "Hoc Phi (VND)", "Ma GS", "Ten Gia Su", "Trang Thai"});
    availableClassesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    availableClassesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    availableClassesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    availableClassesTable->setWordWrap(true);
    vClass->addWidget(availableClassesTable);
    layout->addWidget(classBox);

    // form dang ky, hoc vien lay tu tai khoan
    QGroupBox* bookBox = new QGroupBox("Dang Ky Ca Hoc");
    QFormLayout* bookForm = new QFormLayout(bookBox);
    bookForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    bookForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    bookForm->setSpacing(10);

    selectedClassIdInput = new QLineEdit();
    selectedClassIdInput->setReadOnly(true);
    selectedClassIdInput->setPlaceholderText("(Click vao hang o bang tren de chon lop)");

    bookingActorLabel = new QLabel();
    bookingActorLabel->setStyleSheet("color:#1d4ed8;font-weight:600;");
    bookingStudentCombo = new QComboBox();  // chi Admin thay
    QWidget* actorBox = new QWidget();
    QHBoxLayout* actorLayout = new QHBoxLayout(actorBox);
    actorLayout->setContentsMargins(0, 0, 0, 0);
    actorLayout->addWidget(bookingActorLabel, 1);
    actorLayout->addWidget(bookingStudentCombo, 1);

    btnBook = new QPushButton("Dang Ky Ca Hoc Nay");
    btnBook->setObjectName("btnSuccess");

    bookForm->addRow("Lop Da Chon:", selectedClassIdInput);
    bookForm->addRow("Hoc Vien:",    actorBox);
    bookForm->addRow("",             btnBook);
    layout->addWidget(bookBox);

    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::onSearchAvailableClasses);
    connect(btnReset,  &QPushButton::clicked, this, &MainWindow::onResetSearchAvailableClasses);
    connect(availableClassesTable, &QTableWidget::cellClicked, this, &MainWindow::onAvailableClassSelected);
    connect(btnBook,   &QPushButton::clicked, this, &MainWindow::onBookClass);

    return tab;
}

// tab gia su & phe duyet
QWidget* MainWindow::createTutorTab() {
    QWidget* tab = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(tab);
    layout->setSpacing(16);
    layout->setContentsMargins(12, 12, 12, 12);

    // cot trai: gia su + form mo ca
    QWidget* leftPanel = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 8, 0);
    leftLayout->setSpacing(12);

    QGroupBox* actorBox = new QGroupBox("Gia Su Dang Thao Tac");
    QHBoxLayout* actorLayout = new QHBoxLayout(actorBox);
    tutorActorLabel = new QLabel();
    tutorActorLabel->setStyleSheet("color:#1d4ed8;font-weight:600;");
    tutorActorCombo = new QComboBox();  // chi Admin thay
    actorLayout->addWidget(tutorActorLabel, 1);
    actorLayout->addWidget(tutorActorCombo, 1);
    leftLayout->addWidget(actorBox);

    QGroupBox* openBox = new QGroupBox("Mo Khung Gio Day Moi");
    QFormLayout* openForm = new QFormLayout(openBox);
    openForm->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    openForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    openForm->setSpacing(10);

    QLabel* autoIdNote = new QLabel("(He thong tu dong sinh)");
    autoIdNote->setStyleSheet("color:#1d4ed8;font-style:italic;font-weight:600;");

    openSubjectInput = new QLineEdit(); openSubjectInput->setPlaceholderText("Mon hoc (VD: Toan 12)");
    schedulePicker   = new SchedulePicker();
    openFeeInput     = new QLineEdit(); openFeeInput->setPlaceholderText("Hoc phi moi buoi (VD: 140000)");

    QPushButton* btnOpen = new QPushButton("Mo Ca Day");
    btnOpen->setObjectName("btnSuccess");

    openForm->addRow("Ma Lop (tu sinh):", autoIdNote);
    openForm->addRow("Mon Hoc:",          openSubjectInput);
    openForm->addRow("Hoc Phi/Buoi:",     openFeeInput);
    openForm->addRow("Lich Hoc:",         schedulePicker);
    leftLayout->addWidget(openBox);
    leftLayout->addStretch();

    // form dai thi cuon, nut Mo Ca Day luon hien o duoi
    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setWidget(leftPanel);

    QWidget* leftColumn = new QWidget();
    QVBoxLayout* leftColumnLayout = new QVBoxLayout(leftColumn);
    leftColumnLayout->setContentsMargins(0, 0, 0, 0);
    leftColumnLayout->setSpacing(8);
    leftColumnLayout->addWidget(scroll, 1);
    leftColumnLayout->addWidget(btnOpen);
    leftColumn->setMinimumWidth(480);
    layout->addWidget(leftColumn, 45);

    // cot phai: yeu cau cho duyet
    QGroupBox* pendingBox = new QGroupBox("Yeu Cau Cho Duyet (PENDING)");
    QVBoxLayout* vPending = new QVBoxLayout(pendingBox);

    pendingRequestsTable = new QTableWidget();
    pendingRequestsTable->setColumnCount(7);
    pendingRequestsTable->setHorizontalHeaderLabels({"Ma Lop", "Mon Hoc", "Lich Hoc", "Ma Hoc Vien", "Ten Hoc Vien", "SDT", "Trang Thai"});
    pendingRequestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pendingRequestsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pendingRequestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pendingRequestsTable->setWordWrap(true);
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

    layout->addWidget(pendingBox, 55);

    connect(btnOpen,    &QPushButton::clicked, this, &MainWindow::onOpenClass);
    connect(btnApprove, &QPushButton::clicked, this, &MainWindow::onApproveRequest);
    connect(btnReject,  &QPushButton::clicked, this, &MainWindow::onRejectRequest);
    connect(tutorActorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() { refreshPendingRequests(); });

    return tab;
}

// tab lop cua toi & hop dong
QWidget* MainWindow::createOverviewTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(12);
    layout->setContentsMargins(12, 12, 12, 12);

    // hien theo tai khoan dang nhap
    myClassesBox = new QGroupBox("Lop Hoc Cua Toi");
    QVBoxLayout* vMy = new QVBoxLayout(myClassesBox);

    myClassesTable = new QTableWidget();
    myClassesTable->setColumnCount(7);
    myClassesTable->setHorizontalHeaderLabels({"Ma Lop", "Mon Hoc", "Lich Hoc", "Hoc Phi", "Doi Tac", "Trang Thai", "Ma Hop Dong"});
    myClassesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    myClassesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    myClassesTable->setWordWrap(true);
    vMy->addWidget(myClassesTable);
    layout->addWidget(myClassesBox);

    connect(myClassesTable, &QTableWidget::cellClicked, this, [this](int row, int col) {
        if (col != 4 || m_currentRole != "Student") return;
        QTableWidgetItem* item = myClassesTable->item(row, 4);
        if (item) showTutorProfile(item->data(Qt::UserRole).toString());
    });

    contractsBox = new QGroupBox("Hop Dong Cua Toi");
    QVBoxLayout* vCt = new QVBoxLayout(contractsBox);
    contractsTableWidget = new QTableWidget();
    contractsTableWidget->setColumnCount(6);
    contractsTableWidget->setHorizontalHeaderLabels({"Ma Hop Dong", "Ma Hoc Vien", "Ma Gia Su", "Mon Hoc", "Lich Trinh", "Trang Thai"});
    contractsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    contractsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vCt->addWidget(contractsTableWidget);
    layout->addWidget(contractsBox);

    return tab;
}

// xem ho so gia su

// chi hien thong tin cong khai, khong lo sdt / email / ngay sinh / dia chi
void MainWindow::showTutorProfile(const QString& tutorId) {
    Tutor* t = service.findTutorById(tutorId.toStdString());
    if (!t) {
        QMessageBox::warning(this, "Canh bao", "Khong tim thay ho so gia su " + tutorId + "!");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Ho So Gia Su - " + UiHelper::toQString(t->getFullName()));
    dlg.setMinimumWidth(480);

    QVBoxLayout* root = new QVBoxLayout(&dlg);
    QFormLayout* form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    form->setSpacing(10);

    auto value = [](const QString& text) {
        QLabel* l = new QLabel(text.isEmpty() ? QString("(chua cap nhat)") : text);
        l->setWordWrap(true);
        l->setTextInteractionFlags(Qt::TextSelectableByMouse);
        return l;
    };

    QLabel* name = new QLabel(UiHelper::toQString(t->getFullName()));
    name->setStyleSheet("font-size:16px;font-weight:bold;color:#1e40af;");
    form->addRow("Ho va ten:", name);
    form->addRow("Ma gia su:", value(UiHelper::toQString(t->getId())));
    form->addRow("Gioi tinh:", value(UiHelper::toQString(t->getGender())));
    form->addRow("Mon day:", value(UiHelper::toQString(t->getSubjects())));
    form->addRow("Bang cap:", value(UiHelper::toQString(t->getQualification())));
    form->addRow("Kinh nghiem:", value(QString::number(t->getExpYears()) + " nam"));
    form->addRow("Thanh tich:", value(UiHelper::toQString(t->getAchievements())));
    form->addRow("Lich ranh:", value(UiHelper::toQString(t->getSchedule())));
    root->addLayout(form);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    root->addWidget(buttons);

    dlg.exec();
}

// ten hien thi

QString MainWindow::identityText() const {
    QString info = (m_currentRole == "Guest")
        ? "Dang nhap: Khach (chi xem)"
        : "Xin chao, " + m_currentName + "  |  Vai tro: " + m_currentRole +
          (m_currentPersonId.isEmpty() ? "" : "  |  Ma dinh danh: " + m_currentPersonId);
    return "  " + info + "  ";
}

// goi lai khi doi ten
void MainWindow::refreshIdentity() {
    userInfoLabel->setText(identityText());
    bookingActorLabel->setText(m_currentRole == "Student"
        ? m_currentName + " (" + m_currentPersonId + ")"
        : "Khach - can dang nhap tai khoan Hoc Vien de dang ky");
    tutorActorLabel->setText(m_currentName + " (" + m_currentPersonId + ")");
}

// tab ho so ca nhan

QWidget* MainWindow::createProfileTab() {
    bool isTutor = (m_currentRole == "Tutor");

    QWidget* tab = new QWidget();
    QVBoxLayout* outer = new QVBoxLayout(tab);
    outer->setContentsMargins(12, 12, 12, 12);

    QGroupBox* box = new QGroupBox(isTutor ? "Ho So Gia Su" : "Ho So Hoc Vien");
    QFormLayout* form = new QFormLayout(box);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    form->setSpacing(10);

    pfIdLabel = new QLabel();
    pfIdLabel->setStyleSheet("color:#1d4ed8;font-weight:600;");
    pfName    = new QLineEdit();
    pfPhone   = new QLineEdit(); pfPhone->setMaxLength(10);
    pfEmail   = new QLineEdit();
    pfGender  = new QComboBox(); pfGender->addItems({"Nam", "Nu"});
    pfDob     = new QLineEdit(); pfDob->setPlaceholderText("dd/mm/yyyy (VD: 15/03/2007)");
    pfAddress = new QLineEdit();
    pfSubjects = new QLineEdit(); pfSubjects->setPlaceholderText("VD: Toan, Ly (ngan cach bang dau phay)");
    pfSchedule = new QLineEdit(); pfSchedule->setPlaceholderText("VD: T2, T4, T6 (ghi chu lich ranh)");
    pfGrade = new QLineEdit();
    pfBudget = new QLineEdit(); pfBudget->setPlaceholderText("VD: 100000");
    pfQualification = new QLineEdit(); pfQualification->setPlaceholderText("VD: Dai hoc Su Pham, IELTS 7.5");
    pfExp = new QLineEdit(); pfExp->setPlaceholderText("VD: 3");
    pfAchievements = new QPlainTextEdit();
    pfAchievements->setPlaceholderText("Mo ta thanh tich (giai thuong, chung chi, ket qua hoc sinh...)");
    pfAchievements->setFixedHeight(90);

    form->addRow(isTutor ? "Ma Gia Su:" : "Ma Hoc Vien:", pfIdLabel);
    form->addRow("Ho va Ten:", pfName);
    form->addRow("So DT:",     pfPhone);
    form->addRow("Email:",     pfEmail);
    form->addRow("Gioi tinh:", pfGender);
    form->addRow("Ngay sinh:", pfDob);
    form->addRow("Dia chi:",   pfAddress);
    if (isTutor) {
        form->addRow("Mon day:",         pfSubjects);
        form->addRow("Bang cap:",        pfQualification);
        form->addRow("So nam kinh nghiem:", pfExp);
        form->addRow("Thanh tich:",      pfAchievements);
        form->addRow("Lich ranh:",       pfSchedule);
    } else {
        form->addRow("Lop:",             pfGrade);
        form->addRow("Mon quan tam:",    pfSubjects);
        form->addRow("Hoc phi mong muon:", pfBudget);
        form->addRow("Lich ranh:",       pfSchedule);
    }

    btnSaveProfile = new QPushButton("Luu Ho So");
    btnSaveProfile->setObjectName("btnSuccess");
    btnResetProfile = new QPushButton("Hoan Tac");
    btnResetProfile->setObjectName("btnSecondary");
    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(btnSaveProfile);
    buttons->addWidget(btnResetProfile);
    buttons->addStretch();
    form->addRow("", buttons);

    // o cua vai tro khac: gan vao tab va an
    QWidget* notUsed[3] = {nullptr, nullptr, nullptr};
    if (isTutor) { notUsed[0] = pfGrade; notUsed[1] = pfBudget; }
    else         { notUsed[0] = pfQualification; notUsed[1] = pfExp; notUsed[2] = pfAchievements; }
    for (QWidget* w : notUsed) {
        if (w) { w->setParent(tab); w->hide(); }
    }

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    box->setMaximumWidth(760);
    QWidget* holder = new QWidget();
    QHBoxLayout* hl = new QHBoxLayout(holder);
    hl->addWidget(box);
    hl->addStretch();
    scroll->setWidget(holder);
    outer->addWidget(scroll);

    connect(btnSaveProfile,  &QPushButton::clicked, this, &MainWindow::onSaveProfile);
    connect(btnResetProfile, &QPushButton::clicked, this, &MainWindow::loadProfileForm);

    loadProfileForm();
    return tab;
}

void MainWindow::loadProfileForm() {
    bool isTutor = (m_currentRole == "Tutor");
    bool isStudent = (m_currentRole == "Student");
    pfIdLabel->setText(m_currentPersonId);

    bool found = false;
    std::string id = m_currentPersonId.toStdString();
    if (isTutor) {
        Tutor* t = service.findTutorById(id);
        if (t) {
            found = true;
            pfName->setText(UiHelper::toQString(t->getFullName()));
            pfPhone->setText(UiHelper::toQString(t->getPhone()));
            pfEmail->setText(UiHelper::toQString(t->getEmail()));
            pfGender->setCurrentText(t->getGender() == "Nu" ? "Nu" : "Nam");
            pfDob->setText(UiHelper::toQString(t->getDateOfBirth()));
            pfAddress->setText(UiHelper::toQString(t->getAddress()));
            pfSubjects->setText(UiHelper::toQString(t->getSubjects()));
            pfSchedule->setText(UiHelper::toQString(t->getSchedule()));
            pfQualification->setText(UiHelper::toQString(t->getQualification()));
            pfExp->setText(QString::number(t->getExpYears()));
            pfAchievements->setPlainText(UiHelper::toQString(t->getAchievements()));
        }
    } else if (isStudent) {
        Student* s = service.findStudentById(id);
        if (s) {
            found = true;
            pfName->setText(UiHelper::toQString(s->getFullName()));
            pfPhone->setText(UiHelper::toQString(s->getPhone()));
            pfEmail->setText(UiHelper::toQString(s->getEmail()));
            pfGender->setCurrentText(s->getGender() == "Nu" ? "Nu" : "Nam");
            pfDob->setText(UiHelper::toQString(s->getDateOfBirth()));
            pfAddress->setText(UiHelper::toQString(s->getAddress()));
            pfSubjects->setText(UiHelper::toQString(s->getSubjects()));
            pfSchedule->setText(UiHelper::toQString(s->getSchedule()));
            pfGrade->setText(UiHelper::toQString(s->getGrade()));
            pfBudget->setText(QString::number(s->getBudget(), 'f', 0));
        }
    }
    // chua co ho so thi khoa nut luu
    btnSaveProfile->setEnabled(found);
}

void MainWindow::onSaveProfile() {
    std::string id = m_currentPersonId.toStdString();
    std::string error;
    bool ok = false;

    if (m_currentRole == "Tutor") {
        bool expOk = false;
        int exp = pfExp->text().trimmed().toInt(&expOk);
        if (!expOk) {
            QMessageBox::warning(this, "Canh bao", "So nam kinh nghiem phai la so nguyen (VD: 3)!");
            return;
        }
        TutorProfile p;
        p.fullName = pfName->text().trimmed().toStdString();
        p.phone = pfPhone->text().trimmed().toStdString();
        p.email = pfEmail->text().trimmed().toStdString();
        p.gender = pfGender->currentText().toStdString();
        p.dateOfBirth = pfDob->text().trimmed().toStdString();
        p.address = pfAddress->text().trimmed().toStdString();
        p.subjects = pfSubjects->text().trimmed().toStdString();
        p.schedule = pfSchedule->text().trimmed().toStdString();
        p.qualification = pfQualification->text().trimmed().toStdString();
        p.achievements = pfAchievements->toPlainText().trimmed().toStdString();
        p.expYears = exp;
        ok = service.updateTutorProfile(id, p, &error);
    } else if (m_currentRole == "Student") {
        bool budgetOk = true;
        double budget = 0.0;
        QString budgetText = pfBudget->text().trimmed();
        if (!budgetText.isEmpty()) budget = budgetText.toDouble(&budgetOk);
        if (!budgetOk) {
            QMessageBox::warning(this, "Canh bao", "Hoc phi mong muon phai la so (VD: 100000)!");
            return;
        }
        StudentProfile p;
        p.fullName = pfName->text().trimmed().toStdString();
        p.phone = pfPhone->text().trimmed().toStdString();
        p.email = pfEmail->text().trimmed().toStdString();
        p.gender = pfGender->currentText().toStdString();
        p.dateOfBirth = pfDob->text().trimmed().toStdString();
        p.address = pfAddress->text().trimmed().toStdString();
        p.subjects = pfSubjects->text().trimmed().toStdString();
        p.schedule = pfSchedule->text().trimmed().toStdString();
        p.grade = pfGrade->text().trimmed().toStdString();
        p.budget = budget;
        ok = service.updateStudentProfile(id, p, &error);
    } else {
        return;
    }

    if (!ok) {
        QMessageBox::critical(this, "Loi", "Luu ho so that bai!\n" + UiHelper::toQString(error));
        return;
    }

    // dong bo ten / sdt / email sang tai khoan
    if (m_auth && m_auth->getCurrentUser()) {
        Account* acc = m_auth->getCurrentUser();
        acc->setFullName(pfName->text().trimmed().toStdString());
        acc->setPhone(pfPhone->text().trimmed().toStdString());
        acc->setEmail(pfEmail->text().trimmed().toStdString());
        m_auth->saveToFile();
    }
    m_currentName = pfName->text().trimmed();
    refreshIdentity();
    refreshAllTables();
    QMessageBox::information(this, "Thanh cong", "Da luu ho so!");
}

// nguoi dang thao tac

QString MainWindow::actingStudentId() const {
    if (m_currentRole == "Student") return m_currentPersonId;
    if (m_currentRole == "Admin")   return bookingStudentCombo->currentData().toString();
    return QString();
}

QString MainWindow::actingTutorId() const {
    if (m_currentRole == "Tutor") return m_currentPersonId;
    if (m_currentRole == "Admin") return tutorActorCombo->currentData().toString();
    return QString();
}

// Admin khong co ma rieng nen chon nguoi de thao tac ho
void MainWindow::populateActorCombos() {
    if (m_currentRole != "Admin") return;

    QString prevStudent = bookingStudentCombo->currentData().toString();
    QString prevTutor   = tutorActorCombo->currentData().toString();
    QSignalBlocker b1(bookingStudentCombo);
    QSignalBlocker b2(tutorActorCombo);

    bookingStudentCombo->clear();
    for (Student* st : service.getStudents()) {
        bookingStudentCombo->addItem(UiHelper::nameWithId(st->getFullName(), st->getId()),
                                     UiHelper::toQString(st->getId()));
    }
    int si = bookingStudentCombo->findData(prevStudent);
    if (si >= 0) bookingStudentCombo->setCurrentIndex(si);

    tutorActorCombo->clear();
    for (Tutor* tu : service.getTutors()) {
        tutorActorCombo->addItem(UiHelper::nameWithId(tu->getFullName(), tu->getId()),
                                 UiHelper::toQString(tu->getId()));
    }
    int ti = tutorActorCombo->findData(prevTutor);
    if (ti >= 0) tutorActorCombo->setCurrentIndex(ti);
}

// slots

void MainWindow::onAddStudent() {
    string name     = stNameInput->text().trimmed().toStdString();
    string grade    = stGradeInput->text().trimmed().toStdString();
    string phone    = stPhoneInput->text().trimmed().toStdString();
    string email    = stEmailInput->text().trimmed().toStdString();
    string subjects = stSubjectsInput->text().trimmed().toStdString();
    double budget   = stBudgetInput->text().trimmed().toDouble();

    if (name.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap Ho va Ten!");
        return;
    }
    if (!Validator::isValidPhone(phone)) {
        QMessageBox::warning(this, "Canh bao", UiHelper::toQString(Validator::getPhoneError(phone)));
        return;
    }
    if (email.empty() || !Validator::isValidEmail(email)) {
        QMessageBox::warning(this, "Canh bao", email.empty() ? "Vui long nhap Email!" : UiHelper::toQString(Validator::getEmailError(email)));
        return;
    }

    Student* st = new Student("", name, grade, phone, email, "", "Nam", "", subjects, "", budget);
    if (service.addStudent(st)) {
        stGeneratedIdLabel->setText(UiHelper::toQString(st->getId()));
        QMessageBox::information(this, "Thanh cong",
            QString("Them hoc vien thanh cong!\nMa tu dong sinh: %1").arg(UiHelper::toQString(st->getId())));
        stNameInput->clear(); stGradeInput->clear(); stPhoneInput->clear();
        stEmailInput->clear(); stSubjectsInput->clear(); stBudgetInput->clear();
        refreshAllTables();
    } else {
        delete st;
        QMessageBox::critical(this, "Loi", "Them hoc vien that bai!");
    }
}

void MainWindow::onAddTutor() {
    string name     = tuNameInput->text().trimmed().toStdString();
    string phone    = tuPhoneInput->text().trimmed().toStdString();
    string email    = tuEmailInput->text().trimmed().toStdString();
    string subjects = tuSubjectsInput->text().trimmed().toStdString();
    int    exp      = tuExpInput->text().trimmed().toInt();

    if (name.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap Ho va Ten!");
        return;
    }
    if (!Validator::isValidPhone(phone)) {
        QMessageBox::warning(this, "Canh bao", UiHelper::toQString(Validator::getPhoneError(phone)));
        return;
    }
    if (email.empty() || !Validator::isValidEmail(email)) {
        QMessageBox::warning(this, "Canh bao", email.empty() ? "Vui long nhap Email!" : UiHelper::toQString(Validator::getEmailError(email)));
        return;
    }

    Tutor* tu = new Tutor("", name, phone, email, "", "Nam", "", subjects, "", exp, 0.0, "");
    if (service.addTutor(tu)) {
        tuGeneratedIdLabel->setText(UiHelper::toQString(tu->getId()));
        QMessageBox::information(this, "Thanh cong",
            QString("Them gia su thanh cong!\nMa tu dong sinh: %1").arg(UiHelper::toQString(tu->getId())));
        tuNameInput->clear(); tuPhoneInput->clear(); tuEmailInput->clear();
        tuSubjectsInput->clear(); tuExpInput->clear();
        refreshAllTables();
    } else {
        delete tu;
        QMessageBox::critical(this, "Loi", "Them gia su that bai!");
    }
}

void MainWindow::onSearchAvailableClasses() {
    QString subKey = searchSubjectInput->text().trimmed().toLower();
    availableClassesTable->setRowCount(0);

    int row = 0;

    for (Class* c : service.getClasses()) {
        if (c->getStatus() == "AVAILABLE") {
            QString cSub = UiHelper::toQString(c->getSubject()).toLower();
            if (subKey.isEmpty() || cSub.contains(subKey)) {
                availableClassesTable->insertRow(row);
                availableClassesTable->setItem(row, 0, UiHelper::tableCell(UiHelper::toQString(c->getClassId())));
                availableClassesTable->setItem(row, 1, UiHelper::tableCell(UiHelper::toQString(c->getSubject())));
                availableClassesTable->setItem(row, 2, UiHelper::tableCell(UiHelper::scheduleText(c->getSchedule())));
                availableClassesTable->setItem(row, 3, UiHelper::tableCell(QString::number(c->getTuitionFee())));
                availableClassesTable->setItem(row, 4, UiHelper::tableCell(UiHelper::toQString(c->getTutorId())));
                Tutor* t = service.findTutorById(c->getTutorId());
                availableClassesTable->setItem(row, 5, UiHelper::tutorLinkCell(t ? UiHelper::toQString(t->getFullName()) : "N/A", c->getTutorId()));
                availableClassesTable->setItem(row, 6, UiHelper::tableCell("AVAILABLE"));
                row++;
            }
        }
    }
    availableClassesTable->resizeRowsToContents();
}

void MainWindow::onResetSearchAvailableClasses() {
    searchSubjectInput->clear();
    onSearchAvailableClasses();
}

void MainWindow::onAvailableClassSelected(int row, int col) {
    if (row >= 0 && row < availableClassesTable->rowCount()) {
        selectedClassIdInput->setText(availableClassesTable->item(row, 0)->text());
        // bam cot ten gia su de xem ho so (Khach khong xem)
        if (col == 5 && m_currentRole != "Guest") {
            showTutorProfile(availableClassesTable->item(row, 5)->data(Qt::UserRole).toString());
        }
    }
}

void MainWindow::onBookClass() {
    if (m_currentRole == "Guest") {
        QMessageBox::warning(this, "Canh bao", "Che do Khach khong the dang ky. Vui long dang nhap tai khoan Hoc Vien!");
        return;
    }

    string classId   = selectedClassIdInput->text().trimmed().toStdString();
    string studentId = actingStudentId().toStdString();  // lay tu tai khoan

    if (classId.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon ca hoc tu danh sach!");
        return;
    }
    if (studentId.empty()) {
        QMessageBox::warning(this, "Canh bao", "Chua co hoc vien de dang ky!");
        return;
    }

    string error;
    if (service.studentBookClass(studentId, classId, &error)) {
        QMessageBox::information(this, "Thanh cong",
            QString("Dang ky ca hoc %1 thanh cong!\nCa hoc da chuyen sang PENDING va an khoi hoc vien khac.")
            .arg(UiHelper::toQString(classId)));
        selectedClassIdInput->clear();
        refreshAllTables();
    } else {
        QMessageBox::critical(this, "Loi", "Dang ky that bai!\n" + UiHelper::toQString(error));
        refreshAllTables();  // ca co the vua bi nguoi khac chon
    }
}

void MainWindow::onOpenClass() {
    string tutorId = actingTutorId().toStdString();  // lay tu tai khoan
    string subject = openSubjectInput->text().trimmed().toStdString();

    if (tutorId.empty()) {
        QMessageBox::warning(this, "Canh bao", "Chua co gia su de mo ca day!");
        return;
    }
    if (subject.empty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap Mon hoc!");
        return;
    }

    bool feeOk = false;
    double fee = openFeeInput->text().trimmed().toDouble(&feeOk);
    if (!feeOk || fee <= 0) {
        QMessageBox::warning(this, "Canh bao", "Hoc phi phai la so lon hon 0 (VD: 140000)!");
        return;
    }

    Schedule schedule;
    QString scheduleError;
    if (!schedulePicker->getSchedule(schedule, &scheduleError)) {
        QMessageBox::warning(this, "Lich hoc chua hop le", scheduleError);
        return;
    }

    string newClassId, error;
    if (service.tutorOpenClass(tutorId, subject, schedule.toString(), fee, "", "", &newClassId, &error)) {
        QMessageBox::information(this, "Thanh cong",
            QString("Mo ca day thanh cong!\nMa lop tu dong sinh: %1\nLich: %2")
            .arg(UiHelper::toQString(newClassId), UiHelper::toQString(schedule.toDisplayString())));
        openSubjectInput->clear();
        openFeeInput->clear();
        schedulePicker->reset();
        refreshAllTables();
    } else {
        QMessageBox::critical(this, "Loi", "Mo ca day that bai!\n" + UiHelper::toQString(error));
    }
}

void MainWindow::onApproveRequest() {
    int row = pendingRequestsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon mot yeu cau tu danh sach!");
        return;
    }

    string classId = pendingRequestsTable->item(row, 0)->text().toStdString();
    string tutorId = actingTutorId().toStdString();
    if (service.tutorApproveClass(tutorId, classId)) {
        QMessageBox::information(this, "Thanh cong",
            QString("Da phe duyet lop %1 va tao hop dong!").arg(UiHelper::toQString(classId)));
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
    string tutorId = actingTutorId().toStdString();
    if (service.tutorRejectClass(tutorId, classId)) {
        QMessageBox::information(this, "Thong bao",
            QString("Da tu choi yeu cau lop %1. Khung gio da mo lai.").arg(UiHelper::toQString(classId)));
        refreshAllTables();
    } else {
        QMessageBox::critical(this, "Loi", "Tu choi that bai!");
    }
}

// lam moi bang

// yeu cau PENDING cua gia su dang thao tac
void MainWindow::refreshPendingRequests() {
    pendingRequestsTable->setRowCount(0);
    string tutorId = actingTutorId().toStdString();
    if (tutorId.empty()) return;

    int row = 0;
    for (Class* c : service.getClasses()) {
        if (c->getTutorId() == tutorId && c->getStatus() == "PENDING") {
            Student* s = service.findStudentById(c->getStudentId());
            pendingRequestsTable->insertRow(row);
            pendingRequestsTable->setItem(row, 0, UiHelper::tableCell(UiHelper::toQString(c->getClassId())));
            pendingRequestsTable->setItem(row, 1, UiHelper::tableCell(UiHelper::toQString(c->getSubject())));
            pendingRequestsTable->setItem(row, 2, UiHelper::tableCell(UiHelper::scheduleText(c->getSchedule())));
            pendingRequestsTable->setItem(row, 3, UiHelper::tableCell(UiHelper::toQString(c->getStudentId())));
            pendingRequestsTable->setItem(row, 4, UiHelper::tableCell(s ? UiHelper::toQString(s->getFullName()) : "N/A"));
            pendingRequestsTable->setItem(row, 5, UiHelper::tableCell(s ? UiHelper::toQString(s->getPhone()) : "N/A"));
            pendingRequestsTable->setItem(row, 6, UiHelper::tableCell("PENDING"));
            row++;
        }
    }
    pendingRequestsTable->resizeRowsToContents();
}

// hoc vien: lop da dang ky, gia su: lop cua minh, Admin: tat ca, Khach: khong co
void MainWindow::refreshMyClasses() {
    myClassesTable->setRowCount(0);

    bool isStudent = (m_currentRole == "Student");
    bool isTutor   = (m_currentRole == "Tutor");
    bool isAdmin   = (m_currentRole == "Admin");

    if (isAdmin) {
        myClassesBox->setTitle("Tat Ca Lop Hoc Trong He Thong");
    } else if (isStudent || isTutor) {
        myClassesBox->setTitle("Lop Hoc Cua " + m_currentName + " (" + m_currentPersonId + ")");
    } else {
        myClassesBox->setTitle("Lop Hoc Cua Toi (Can dang nhap de xem)");
        return;
    }

    string me = m_currentPersonId.toStdString();
    int row = 0;
    for (Class* c : service.getClasses()) {
        bool match = isAdmin ||
            (isStudent && c->getStudentId() == me && c->getStatus() != "AVAILABLE") ||
            (isTutor   && c->getTutorId()   == me);

        if (match) {
            myClassesTable->insertRow(row);
            myClassesTable->setItem(row, 0, UiHelper::tableCell(UiHelper::toQString(c->getClassId())));
            myClassesTable->setItem(row, 1, UiHelper::tableCell(UiHelper::toQString(c->getSubject())));
            myClassesTable->setItem(row, 2, UiHelper::tableCell(UiHelper::scheduleText(c->getSchedule())));
            myClassesTable->setItem(row, 3, UiHelper::tableCell(QString::number(c->getTuitionFee())));

            Tutor* t = service.findTutorById(c->getTutorId());
            Student* s = service.findStudentById(c->getStudentId());
            QString tutorName   = t ? UiHelper::toQString(t->getFullName()) : UiHelper::toQString(c->getTutorId());
            QString studentName = c->getStudentId().empty() ? QString("(chua co hoc vien)")
                                : (s ? UiHelper::toQString(s->getFullName()) : UiHelper::toQString(c->getStudentId()));

            QString partner;
            if (isStudent)    partner = tutorName;
            else if (isTutor) partner = studentName;
            else              partner = studentName + "  -  " + tutorName;
            // bam ten gia su de xem ho so
            myClassesTable->setItem(row, 4, isStudent ? UiHelper::tutorLinkCell(partner, c->getTutorId()) : UiHelper::tableCell(partner));
            myClassesTable->setItem(row, 5, UiHelper::tableCell(UiHelper::toQString(c->getStatus())));
            myClassesTable->setItem(row, 6, UiHelper::tableCell(UiHelper::toQString(c->getContractId())));
            row++;
        }
    }
    myClassesTable->resizeRowsToContents();
}

// chi hien hop dong cua minh, Admin xem tat ca
void MainWindow::refreshContracts() {
    contractsTableWidget->setRowCount(0);

    bool isStudent = (m_currentRole == "Student");
    bool isTutor   = (m_currentRole == "Tutor");
    bool isAdmin   = (m_currentRole == "Admin");

    contractsBox->setTitle(isAdmin ? "Toan Bo Hop Dong Trong He Thong" : "Hop Dong Cua Toi");
    if (!isAdmin && !isStudent && !isTutor) return;

    string me = m_currentPersonId.toStdString();
    int row = 0;
    for (Contract* ct : service.getContracts()) {
        bool match = isAdmin ||
            (isStudent && ct->getStudentId() == me) ||
            (isTutor   && ct->getTutorId()   == me);

        if (match) {
            QString period = (ct->getStartDate().empty() && ct->getEndDate().empty())
                ? QString("-")
                : UiHelper::toQString(ct->getStartDate()) + " -> " + UiHelper::toQString(ct->getEndDate());
            contractsTableWidget->insertRow(row);
            contractsTableWidget->setItem(row, 0, UiHelper::tableCell(UiHelper::toQString(ct->getContractId())));
            contractsTableWidget->setItem(row, 1, UiHelper::tableCell(UiHelper::toQString(ct->getStudentId())));
            contractsTableWidget->setItem(row, 2, UiHelper::tableCell(UiHelper::toQString(ct->getTutorId())));
            contractsTableWidget->setItem(row, 3, UiHelper::tableCell(UiHelper::toQString(ct->getSubjects())));
            contractsTableWidget->setItem(row, 4, UiHelper::tableCell(period));
            contractsTableWidget->setItem(row, 5, UiHelper::tableCell(UiHelper::toQString(ct->getStatus())));
            row++;
        }
    }
}

void MainWindow::refreshAllTables() {
    populateActorCombos();

    // bang hoc vien
    studentTableWidget->setRowCount(0);
    int sRow = 0;
    for (Student* s : service.getStudents()) {
        studentTableWidget->insertRow(sRow);
        studentTableWidget->setItem(sRow, 0, UiHelper::tableCell(UiHelper::toQString(s->getId())));
        studentTableWidget->setItem(sRow, 1, UiHelper::tableCell(UiHelper::toQString(s->getFullName())));
        studentTableWidget->setItem(sRow, 2, UiHelper::tableCell(UiHelper::toQString(s->getGender())));
        studentTableWidget->setItem(sRow, 3, UiHelper::tableCell(UiHelper::toQString(s->getDateOfBirth())));
        studentTableWidget->setItem(sRow, 4, UiHelper::tableCell(UiHelper::toQString(s->getGrade())));
        studentTableWidget->setItem(sRow, 5, UiHelper::tableCell(UiHelper::toQString(s->getPhone())));
        studentTableWidget->setItem(sRow, 6, UiHelper::tableCell(UiHelper::toQString(s->getEmail())));
        studentTableWidget->setItem(sRow, 7, UiHelper::tableCell(UiHelper::toQString(s->getAddress())));
        studentTableWidget->setItem(sRow, 8, UiHelper::tableCell(UiHelper::toQString(s->getSubjects())));
        studentTableWidget->setItem(sRow, 9, UiHelper::tableCell(QString::number(s->getBudget(), 'f', 0)));
        sRow++;
    }

    // bang gia su, hoc phi dat theo tung ca nen khong co cot
    tutorTableWidget->setRowCount(0);
    int tRow = 0;
    for (Tutor* t : service.getTutors()) {
        tutorTableWidget->insertRow(tRow);
        tutorTableWidget->setItem(tRow, 0, UiHelper::tableCell(UiHelper::toQString(t->getId())));
        tutorTableWidget->setItem(tRow, 1, UiHelper::tableCell(UiHelper::toQString(t->getFullName())));
        tutorTableWidget->setItem(tRow, 2, UiHelper::tableCell(UiHelper::toQString(t->getGender())));
        tutorTableWidget->setItem(tRow, 3, UiHelper::tableCell(UiHelper::toQString(t->getDateOfBirth())));
        tutorTableWidget->setItem(tRow, 4, UiHelper::tableCell(UiHelper::toQString(t->getPhone())));
        tutorTableWidget->setItem(tRow, 5, UiHelper::tableCell(UiHelper::toQString(t->getEmail())));
        tutorTableWidget->setItem(tRow, 6, UiHelper::tableCell(UiHelper::toQString(t->getAddress())));
        tutorTableWidget->setItem(tRow, 7, UiHelper::tableCell(UiHelper::toQString(t->getSubjects())));
        tutorTableWidget->setItem(tRow, 8, UiHelper::tableCell(UiHelper::toQString(t->getQualification())));
        tutorTableWidget->setItem(tRow, 9, UiHelper::tableCell(QString::number(t->getExpYears()) + " nam"));
        tutorTableWidget->setItem(tRow, 10, UiHelper::tableCell(UiHelper::toQString(t->getAchievements()).replace('\n', ' ')));
        tRow++;
    }

    // ca hoc, yeu cau, lop cua toi, hop dong
    onSearchAvailableClasses();
    refreshPendingRequests();
    refreshMyClasses();
    refreshContracts();
}

// phan quyen theo role
void MainWindow::applyRolePermissions() {
    // Student/Tutor lay tu tai khoan, Admin chon tu danh sach
    bool isAdmin = (m_currentRole == "Admin");
    bookingStudentCombo->setVisible(isAdmin);
    bookingActorLabel->setVisible(!isAdmin);
    tutorActorCombo->setVisible(isAdmin);
    tutorActorLabel->setVisible(!isAdmin);

    refreshIdentity();

    // bo tab de an
    if (m_currentRole == "Student") {
        // Student: tim kiem + lop cua toi
        // xoa tu index lon xuong de khong lech
        tabWidget->removeTab(2);  // gia su & phe duyet
        tabWidget->removeTab(0);  // quan ly nguoi dung
        tabWidget->setCurrentIndex(0);
    } else if (m_currentRole == "Tutor") {
        // Tutor: gia su + lop cua toi
        tabWidget->removeTab(1);  // tim kiem
        tabWidget->removeTab(0);  // quan ly nguoi dung
        tabWidget->setCurrentIndex(0);
    } else if (m_currentRole == "Admin") {
        // Admin: toan quyen, khong co ho so ca nhan
        tabWidget->removeTab(4);
        tabWidget->setCurrentIndex(0);
    } else {
        // Guest: chi xem tim kiem + lop cua toi
        tabWidget->removeTab(4);
        tabWidget->removeTab(2);
        tabWidget->removeTab(0);
        tabWidget->setCurrentIndex(0);
        btnBook->setEnabled(false);
    }
}

// dang xuat
void MainWindow::onLogout() {
    if (!m_auth) {  // khong co AuthService thi chi dong cua so
        close();
        return;
    }
    m_auth->logout();

    this->hide();

    // CenterService dung chung nen doi tai khoan khong mat du lieu
    LoginWindow* loginWin = new LoginWindow(*m_auth);
    if (loginWin->exec() == QDialog::Accepted) {
        // che do Khach: da logout nen la Guest
        MainWindow* newMain = new MainWindow(m_auth, service);
        newMain->show();
    }
    delete loginWin;
    this->close();
}
