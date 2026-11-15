#ifndef SCHEDULE_PICKER_H
#define SCHEDULE_PICKER_H

#include <QWidget>
#include <QVector>
#include <QString>
#include "../entities/Schedule.h"

class QVBoxLayout;
class QFrame;
class QLabel;
class QPushButton;
class QComboBox;

// chon lich: nut bat / tat cho T2..CN,
// dropdown gio bat dau / ket thuc (buoc 15 phut), nhieu khung gio,
// co preview ben duoi
class SchedulePicker : public QWidget {
    Q_OBJECT

public:
    explicit SchedulePicker(QWidget* parent = nullptr);

    // lay lich da chon, false neu thieu thu,
    // gio sai hoac cac khung trung nhau
    bool getSchedule(Schedule& out, QString* error = nullptr) const;

    // ve 1 khung mac dinh
    void reset();

signals:
    void scheduleChanged();

private:
    struct SlotRow {
        QFrame* frame;
        QLabel* title;
        QPushButton* dayButtons[Schedule::DAY_COUNT];
        QComboBox* startCombo;
        QComboBox* endCombo;
        QPushButton* removeButton;
    };

    QVBoxLayout* rowsLayout;
    QPushButton* addButton;
    QLabel* previewLabel;
    QVector<SlotRow*> rows;

    SlotRow* createRow();
    void addRow();
    void removeRow(SlotRow* row);
    void renumberRows();
    void updatePreview();
};

#endif
