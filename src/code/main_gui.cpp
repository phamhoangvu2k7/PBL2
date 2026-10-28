#include <QApplication>
#include "../include/ui/MainWindow.h"
#include "../include/ui/LoginWindow.h"
#include "../include/services/AuthService.h"
#include "../include/services/CenterService.h"
#include "../include/services/DemoData.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // auth, admin mac dinh la admin/admin123
    AuthService auth;

    // du lieu dung chung cho moi cua so,
    // doi tai khoan khong mat ca day / hop dong
    CenterService center;

    // nap du lieu da luu, tu day moi thay doi tu ghi lai
    // lan dau chua co file thi nap du lieu mau
    if (!center.enablePersistence("src/data")) {
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
