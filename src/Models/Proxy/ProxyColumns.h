//
// Created by denis on 08.11.2021.
//

#ifndef TESTIDENTITY_PROXYCOLUMNS_H
#define TESTIDENTITY_PROXYCOLUMNS_H


#include <QString>
#include <list>
#include <vector>

#include "ProxyColumn.h"

class ProxyColumns {
public:
    explicit ProxyColumns() {}
    inline ~ProxyColumns() noexcept {
        for(ProxyColumn *column : m_columns) {
            delete column;
        }
    }

    inline void addProxyColumn(int column,
                               const QString &name, const QString &title,
                               Qt::ItemFlags flags = (Qt::ItemIsEnabled | Qt::ItemIsSelectable)) {
        m_columns.push_back(new ProxyColumn(column, m_columns.size(), name, title, flags));
    }

    inline int origColumnId(int proxy) const {
        for(ProxyColumn *column : m_columns) {
            if(column->proxy() == proxy) {
                return column->orig();
            }
        }

        return -1;
    }

    inline int proxyColumnId(int orig) const {
        for(ProxyColumn *column : m_columns) {
            if(column->proxy() == orig) {
                return column->proxy();
            }
        }

        return -1;
    }

    inline ProxyColumn * proxyColumn(int proxy) const {
        for(ProxyColumn *column : m_columns) {
            if (column->proxy() == proxy) {
                return column;
            }
        }

        return nullptr;
    }

    inline int proxyColumnsSize() const {
        return m_columns.size();
    }

    inline int getColumnByName(const QString &name) const {
        for(ProxyColumn *column : m_columns) {
            if(column->name() == name) {
                return column->proxy();
            }
        }

        return -1;
    }

    inline std::vector<ProxyColumn *> proxyColumns() const { return m_columns; }

protected:
    std::vector<ProxyColumn *> m_columns;
};


#endif //TESTIDENTITY_PROXYCOLUMNS_H
