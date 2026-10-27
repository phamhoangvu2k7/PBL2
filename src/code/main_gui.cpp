#include <QApplication>
#include "../include/ui/MainWindow.h"
#include "../include/ui/LoginWindow.h"
#include "../include/services/AuthService.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Khởi tạo AuthService (tài khoản admin mặc định: admin/admin123)
    AuthService auth;

    // Mở màn hình đăng nhập
    LoginWindow loginWin(auth);
    if (loginWin.exec() != QDialog::Accepted) {
        return 0; // Người dùng đóng cửa sổ đăng nhập
    }

    // Mở MainWindow với AuthService và thông tin phân quyền
    MainWindow* mainWin = nullptr;
    if (loginWin.isGuestMode()) {
        mainWin = new MainWindow(nullptr);  // Chế độ khách
    } else {
        mainWin = new MainWindow(&auth);    // Chế độ đăng nhập
    }

    mainWin->show();
    int ret = app.exec();
    delete mainWin;
    return ret;
}
