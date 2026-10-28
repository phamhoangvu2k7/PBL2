#include "../../include/helper/UiHelper.h"
#include "../../include/entities/Schedule.h"
#include <QColor>
#include <QFont>

QString UiHelper::toQString(const std::string& s) {
    return QString::fromStdString(s);
}

QString UiHelper::scheduleText(const std::string& raw) {
    Schedule s;
    if (Schedule::parse(raw, s, nullptr)) return toQString(s.toDisplayString());
    return toQString(raw);
}

QString UiHelper::nameWithId(const std::string& name, const std::string& id) {
    return name.empty() ? toQString(id) : toQString(name) + " (" + toQString(id) + ")";
}

QTableWidgetItem* UiHelper::tableCell(const QString& text) {
    return new QTableWidgetItem(text);
}

QTableWidgetItem* UiHelper::tutorLinkCell(const QString& text, const std::string& tutorId) {
    QTableWidgetItem* item = new QTableWidgetItem(text);
    item->setData(Qt::UserRole, toQString(tutorId));
    item->setForeground(QColor("#1d4ed8"));
    QFont f = item->font();
    f.setUnderline(true);
    item->setFont(f);
    item->setToolTip("Click de xem ho so gia su");
    return item;
}
