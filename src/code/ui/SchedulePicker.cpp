#include "../../include/ui/SchedulePicker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QStyle>

namespace {

const int DEFAULT_START = 18 * 60;
const int DEFAULT_END   = 20 * 60;
const int LAST_MINUTE   = 23 * 60 + 45;

void fillTimeCombo(QComboBox* combo, int selectedMinutes) {
    for (int m = 0; m <= LAST_MINUTE; m += Schedule::STEP_MINUTES) {
        combo->addItem(QString::fromStdString(Schedule::formatTime(m)), m);
    }
    combo->setCurrentIndex(selectedMinutes / Schedule::STEP_MINUTES);
    combo->setMaxVisibleItems(12);
}

}  // namespace

SchedulePicker::SchedulePicker(QWidget* parent) : QWidget(parent) {
    // style rieng, khong phu thuoc stylesheet chung
    setStyleSheet(R"(
        QFrame#slotFrame {
            background-color: #f8fafc;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
        }
        QLabel#slotTitle { font-weight: bold; color: #1e40af; }
        QPushButton#dayBtn {
            background-color: #e2e8f0;
            color: #475569;
            border: 1px solid #cbd5e1;
            border-radius: 6px;
            padding: 6px 0;
            min-width: 38px;
            font-weight: bold;
        }
        QPushButton#dayBtn:hover { background-color: #cbd5e1; }
        QPushButton#dayBtn:checked {
            background-color: #1d4ed8;
            color: #ffffff;
            border-color: #1e40af;
        }
        QPushButton#removeSlotBtn {
            background-color: #dc2626;
            padding: 6px 12px;
        }
        QPushButton#removeSlotBtn:hover { background-color: #b91c1c; }
        QPushButton#removeSlotBtn:disabled { background-color: #cbd5e1; }
        QLabel#schedulePreview {
            background-color: #eff6ff;
            border: 1px solid #bfdbfe;
            border-radius: 6px;
            padding: 8px 10px;
            color: #1e3a8a;
        }
        QLabel#schedulePreview[invalid="true"] {
            background-color: #fef2f2;
            border-color: #fecaca;
            color: #b91c1c;
        }
    )");

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(8);

    rowsLayout = new QVBoxLayout();
    rowsLayout->setSpacing(8);
    root->addLayout(rowsLayout);

    addButton = new QPushButton("+ Them khung gio");
    addButton->setObjectName("btnSecondary");
    addButton->setCursor(Qt::PointingHandCursor);
    root->addWidget(addButton, 0, Qt::AlignLeft);

    previewLabel = new QLabel();
    previewLabel->setObjectName("schedulePreview");
    previewLabel->setWordWrap(true);
    root->addWidget(previewLabel);

    connect(addButton, &QPushButton::clicked, this, [this]() { addRow(); });

    addRow();
}

SchedulePicker::SlotRow* SchedulePicker::createRow() {
    SlotRow* row = new SlotRow();
    row->frame = new QFrame();
    row->frame->setObjectName("slotFrame");

    QVBoxLayout* v = new QVBoxLayout(row->frame);
    v->setContentsMargins(10, 8, 10, 8);
    v->setSpacing(8);

    row->title = new QLabel();
    row->title->setObjectName("slotTitle");

    // hang 1: tieu de + nut xoa
    QHBoxLayout* top = new QHBoxLayout();
    row->removeButton = new QPushButton("Xoa");
    row->removeButton->setObjectName("removeSlotBtn");
    row->removeButton->setCursor(Qt::PointingHandCursor);
    top->addWidget(row->title);
    top->addStretch();
    top->addWidget(row->removeButton);
    v->addLayout(top);

    // hang 2: 7 nut chon thu, chon duoc nhieu thu
    QHBoxLayout* days = new QHBoxLayout();
    days->setSpacing(6);
    for (int d = 0; d < Schedule::DAY_COUNT; d++) {
        QPushButton* b = new QPushButton(QString::fromStdString(Schedule::dayCode(d)));
        b->setObjectName("dayBtn");
        b->setCheckable(true);
        b->setCursor(Qt::PointingHandCursor);
        row->dayButtons[d] = b;
        days->addWidget(b);
        connect(b, &QPushButton::toggled, this, [this]() { updatePreview(); });
    }
    days->addStretch();
    v->addLayout(days);

    // hang 3: gio bat dau - ket thuc
    QHBoxLayout* times = new QHBoxLayout();
    times->setSpacing(8);
    row->startCombo = new QComboBox();
    row->endCombo = new QComboBox();
    fillTimeCombo(row->startCombo, DEFAULT_START);
    fillTimeCombo(row->endCombo, DEFAULT_END);
    times->addWidget(new QLabel("Gio bat dau:"));
    times->addWidget(row->startCombo);
    times->addSpacing(12);
    times->addWidget(new QLabel("Gio ket thuc:"));
    times->addWidget(row->endCombo);
    times->addStretch();
    v->addLayout(times);

    connect(row->startCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() { updatePreview(); });
    connect(row->endCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() { updatePreview(); });
    connect(row->removeButton, &QPushButton::clicked, this, [this, row]() { removeRow(row); });

    return row;
}

void SchedulePicker::addRow() {
    SlotRow* row = createRow();
    rows.append(row);
    rowsLayout->addWidget(row->frame);
    renumberRows();
    updatePreview();
}

void SchedulePicker::removeRow(SlotRow* row) {
    if (rows.size() <= 1) return;  // luon giu it nhat 1 khung
    rows.removeOne(row);
    rowsLayout->removeWidget(row->frame);
    row->frame->deleteLater();
    delete row;
    renumberRows();
    updatePreview();
}

void SchedulePicker::renumberRows() {
    for (int i = 0; i < rows.size(); i++) {
        rows[i]->title->setText(QString("Khung gio %1").arg(i + 1));
        rows[i]->removeButton->setEnabled(rows.size() > 1);
    }
}

void SchedulePicker::reset() {
    while (rows.size() > 1) {
        SlotRow* row = rows.last();
        rows.removeLast();
        rowsLayout->removeWidget(row->frame);
        row->frame->deleteLater();
        delete row;
    }
    SlotRow* only = rows.first();
    for (int d = 0; d < Schedule::DAY_COUNT; d++) only->dayButtons[d]->setChecked(false);
    only->startCombo->setCurrentIndex(DEFAULT_START / Schedule::STEP_MINUTES);
    only->endCombo->setCurrentIndex(DEFAULT_END / Schedule::STEP_MINUTES);
    renumberRows();
    updatePreview();
}

bool SchedulePicker::getSchedule(Schedule& out, QString* error) const {
    out.clear();
    for (int i = 0; i < rows.size(); i++) {
        const SlotRow* row = rows[i];
        unsigned mask = 0;
        for (int d = 0; d < Schedule::DAY_COUNT; d++) {
            if (row->dayButtons[d]->isChecked()) mask |= (1u << d);
        }
        int startMin = row->startCombo->currentData().toInt();
        int endMin = row->endCombo->currentData().toInt();

        std::string err;
        if (!out.addSlot(mask, startMin, endMin, &err)) {
            if (error != nullptr) {
                *error = QString("Khung gio %1: %2").arg(i + 1).arg(QString::fromStdString(err));
            }
            out.clear();
            return false;
        }
    }
    return true;
}

void SchedulePicker::updatePreview() {
    Schedule s;
    QString err;
    bool ok = getSchedule(s, &err);

    previewLabel->setProperty("invalid", !ok);
    if (ok) {
        previewLabel->setText("Lich hoc: " + QString::fromStdString(s.toDisplayString()));
    } else {
        previewLabel->setText(err);
    }
    // ap lai style de property invalid co hieu luc
    previewLabel->style()->unpolish(previewLabel);
    previewLabel->style()->polish(previewLabel);

    emit scheduleChanged();
}
