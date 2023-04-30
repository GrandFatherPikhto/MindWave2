//
// Created by denis on 09.11.2021.
//

#ifndef TESTIDENTITY_TIMEDELEGATE_H
#define TESTIDENTITY_TIMEDELEGATE_H

#include <QStyledItemDelegate>


class TimeDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit TimeDelegate(QObject *parent = nullptr);
    ~TimeDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

protected:

};


#endif //TESTIDENTITY_TIMEDELEGATE_H
