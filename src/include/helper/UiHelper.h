#ifndef UI_HELPER_H
#define UI_HELPER_H

#include <QString>
#include <QTableWidgetItem>
#include <string>

// tien ich cho giao dien Qt, khong chua nghiep vu
class UiHelper {
public:
    UiHelper() = delete;

    static QString toQString(const std::string& s);

    // "T2,T4 (19:00-21:00)" -> "Thu 2, Thu 4: 19:00 - 21:00", sai dinh dang thi giu nguyen
    static QString scheduleText(const std::string& raw);

    // "Ten (Ma)", thieu ten thi chi hien Ma
    static QString nameWithId(const std::string& name, const std::string& id);

    static QTableWidgetItem* tableCell(const QString& text);

    // o ten gia su nhu link, ma GS luu trong data de mo ho so
    static QTableWidgetItem* tutorLinkCell(const QString& text, const std::string& tutorId);
};

#endif
