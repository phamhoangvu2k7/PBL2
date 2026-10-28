#include <QApplication>
#include "../include/ui/MainWindow.h"
#include "../include/ui/LoginWindow.h"
#include "../include/services/AuthService.h"
#include "../include/services/CenterService.h"
#include "../include/services/DemoData.h"
#include "../include/repositories/AccountRepository.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // tang repository: moi repository doc / ghi 1 file txt
    AccountRepository accountRepo("src/data/accounts.txt");
    StudentRepository studentRepo("src/data/center_students.txt");
    TutorRepository tutorRepo("src/data/center_tutors.txt");
    ClassRepository classRepo("src/data/center_classes.txt");
    ContractRepository contractRepo("src/data/center_contracts.txt");
    accountRepo.load();

    // tang service, dung chung cho moi cua so nen doi tai khoan khong mat du lieu
    AuthService auth(accountRepo);  // admin mac dinh la admin/admin123
    CenterService center(studentRepo, tutorRepo, classRepo, contractRepo);

    // nap du lieu da luu, lan dau chua co file thi nap du lieu mau
    if (!center.loadData()) {
        seedDemoData(center);
    }

    // man hinh dang nhap
    LoginWindow loginWin(auth);
    if (loginWin.exec() != QDialog::Accepted) {
        return 0;  // dong cua so dang nhap
    }

    // chua dang nhap thi la che do khach
    // MainWindow tu xoa khi dong nen khong delete
    MainWindow* mainWin = new MainWindow(&auth, center);
    mainWin->show();
    return app.exec();
}
