#include <QApplication>
#include "../include/ui/MainWindow.h"
#include "../include/ui/LoginWindow.h"
#include "../include/services/AuthService.h"
#include "../include/services/CenterService.h"
#include "../include/services/DemoData.h"
#include "../include/repositories/AccountRepository.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    //5 repo
    AccountRepository accountRepo("src/data/accounts.txt");
    StudentRepository studentRepo("src/data/center_students.txt");
    TutorRepository tutorRepo("src/data/center_tutors.txt");
    ClassRepository classRepo("src/data/center_classes.txt");
    ContractRepository contractRepo("src/data/center_contracts.txt");
    accountRepo.load();

    //service
    AuthService auth(accountRepo);  // admin/admin123
    CenterService center(studentRepo, tutorRepo, classRepo, contractRepo);

    if (!center.nap_du_lieu()) {
        tao_du_lieu_mau(center);
    }

    // man hinh dang nhap
    LoginWindow loginWin(auth);
    if (loginWin.exec() != QDialog::Accepted) {
        return 0; 
    }

    // chua dang nhap thi la che do khach
    MainWindow* mainWin = new MainWindow(&auth, center);
    mainWin->show();
    return app.exec();
}
