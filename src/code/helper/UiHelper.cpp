#include "../../include/helper/UiHelper.h"
#include "../../include/entities/Schedule.h"
#include <QColor>
#include <QFont>

QString UiHelper::sang_qstring(const std::string& s) {
    return QString::fromStdString(s);
}

QString UiHelper::chu_lich(const std::string& raw) {
    Schedule s;
    if (Schedule::doc_lich(raw, s, nullptr)) return sang_qstring(s.chuoi_hien_thi());
    return sang_qstring(raw);
}

QString UiHelper::ten_kem_ma(const std::string& name, const std::string& id) {
    return name.empty() ? sang_qstring(id) : sang_qstring(name) + " (" + sang_qstring(id) + ")";
}

QTableWidgetItem* UiHelper::o_bang(const QString& text) {
    return new QTableWidgetItem(text);
}

QTableWidgetItem* UiHelper::o_link_gia_su(const QString& text, const std::string& tutorId) {
    QTableWidgetItem* item = new QTableWidgetItem(text);
    item->setData(Qt::UserRole, sang_qstring(tutorId));
    item->setForeground(QColor("#1d4ed8"));
    QFont f = item->font();
    f.setUnderline(true);
    item->setFont(f);
    item->setToolTip("Click de xem ho so gia su");
    return item;
}
