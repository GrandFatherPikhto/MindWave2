//
// Created by denis on 07.11.2021.
//

#ifndef UNTITLED2_EFFECTPROXY_H
#define UNTITLED2_EFFECTPROXY_H

#include <QAbstractProxyModel>
#include <QIdentityProxyModel>
#include <QModelIndex>
#include <QVariant>
#include <QModelIndex>

#include <list>
#include <vector>
#include <algorithm>
#include <utility>
#include "ProxyColumns.h"

class QModelIndex;
class QSqlTableModel;
class ProxyColumn;
class ProxyColumns;

class ModelProxy : public QIdentityProxyModel, public ProxyColumns {
    Q_OBJECT
public:
    explicit ModelProxy(QObject *parent = nullptr);
    ~ModelProxy() override;
    int columnCount(const QModelIndex & parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex mapToSource(const QModelIndex & proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex & sourceIndex) const override;
    QVariant data(const QModelIndex & index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex & parent) const override;
    void sort(int column, Qt::SortOrder order) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex buddy(const QModelIndex &index) const override;

    void setHeaders();

protected:

};


#endif //UNTITLED2_EFFECTPROXY_H
