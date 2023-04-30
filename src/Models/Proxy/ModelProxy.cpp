//
// Created by denis on 07.11.2021.
//

#include <QSqlTableModel>
#include "ModelProxy.h"
#include "../Database.h"
#include "ProxyColumns.h"

ModelProxy::ModelProxy(QObject *parent) :
    QIdentityProxyModel(parent)
{
}

ModelProxy::~ModelProxy() {
}

void ModelProxy::setHeaders() {
    for(ProxyColumn *column: m_columns) {
        QAbstractProxyModel::setHeaderData(column->proxy(), Qt::Horizontal, column->title());
    }
}

int ModelProxy::columnCount(const QModelIndex & parent) const {
    return proxyColumnsSize();
}

QVariant ModelProxy::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Horizontal) {
        return sourceModel()->headerData(origColumnId(section), Qt::Horizontal, role);
    }
    return sourceModel()->headerData(section, orientation, role);
}

QModelIndex ModelProxy::mapToSource(const QModelIndex & proxyIndex) const {
    if( !proxyIndex.isValid() ){
        return {};
    }

    return sourceModel()->index(proxyIndex.row(), origColumnId(proxyIndex.column()));
}

QModelIndex ModelProxy::mapFromSource(const QModelIndex & sourceIndex) const {
    if(!sourceIndex.isValid()) {
        return {};
    }

    QModelIndex proxyIndex = index(sourceIndex.row(), proxyColumnId(sourceIndex.column()), sourceIndex);
    return proxyIndex;
}


QVariant ModelProxy::data(const QModelIndex & index, int role) const {
    QModelIndex proxyIdx = mapFromSource(index);
    if(role == Qt::DisplayRole || role == Qt::EditRole) {
        // Здесь можно подменять данные, понеже, есть желание
    }

    return sourceModel()->data(proxyIdx, role);
}

bool ModelProxy::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(role == Qt::EditRole) {
        sourceModel()->setData(mapFromSource(index), value);
        return true;
    }
    return false;
}

QModelIndex ModelProxy::index(int row, int column, const QModelIndex & parent) const {
    if (!parent.isValid()) {
        QModelIndex idx = createIndex(row, column);
        return idx;
    }

    QModelIndex idx = createSourceIndex(row, column, parent.internalPointer());
    if(idx.isValid()) {
        return mapToSource(idx);
    }

    return {};
}

void ModelProxy::sort(int column, Qt::SortOrder order) {
    QIdentityProxyModel::sort(origColumnId(column), order);
    emit dataChanged(
            createIndex(0, 0),
            createIndex(rowCount(QModelIndex()) - 1,columnCount(QModelIndex()) - 1));
}

Qt::ItemFlags ModelProxy::flags(const QModelIndex &index) const {
    if(index.isValid()) {
        return proxyColumn(index.column())->flags();
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

/**
 * Почему надо обязательно реализовывать этот метод, чтобы работали делегаты?
 * @param index
 * @return
 */
QModelIndex ModelProxy::buddy(const QModelIndex &index) const {
    return index;
}
