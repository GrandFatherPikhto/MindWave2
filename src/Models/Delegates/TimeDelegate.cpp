//
// Created by denis on 09.11.2021.
//
#include <QObject>
#include <QRect>
#include <QMargins>
#include <QApplication>
#include <QDateTime>

#include "TimeDelegate.h"

TimeDelegate::TimeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

TimeDelegate::~TimeDelegate() {

}

void TimeDelegate::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
    qint64 timestamp = index.data().toLongLong();
    QApplication::style()->drawItemText(
            painter,
            option.rect - QMargins(4, 4, 4, 4),
            Qt::AlignLeft | Qt::AlignVCenter,
            option.palette,
            true,
            QDateTime::fromSecsSinceEpoch(timestamp).toString("dd MMMM yyyy hh:mm:ss"));
}
