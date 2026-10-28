#ifndef UI_HELPER_H
#define UI_HELPER_H

#include <QString>
#include <QTableWidgetItem>
#include <string>

class UiHelper {
public:
    UiHelper() = delete;

    static QString toQString(const std::string& s);

    static QString scheduleText(const std::string& raw);

    static QString nameWithId(const std::string& name, const std::string& id);

    static QTableWidgetItem* tableCell(const QString& text);

    static QTableWidgetItem* tutorLinkCell(const QString& text, const std::string& tutorId);
};

#endif
