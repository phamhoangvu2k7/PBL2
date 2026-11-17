#ifndef UI_HELPER_H
#define UI_HELPER_H

#include <QString>
#include <QTableWidgetItem>
#include <string>

class UiHelper {
public:
    UiHelper() = delete;

    static QString sang_qstring(const std::string& s);


    static QString ten_kem_ma(const std::string& name, const std::string& id);

    static QTableWidgetItem* o_bang(const QString& text);

    static QTableWidgetItem* o_link_gia_su(const QString& text, const std::string& tutorId);
};

#endif
