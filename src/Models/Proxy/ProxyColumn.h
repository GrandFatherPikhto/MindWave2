//
// Created by denis on 08.11.2021.
//

#ifndef TESTIDENTITY_PROXYCOLUMN_H
#define TESTIDENTITY_PROXYCOLUMN_H


#include <QString>

class ProxyColumn {
public:
    inline ProxyColumn(int orig, int proxy, QString name, QString title,
                Qt::ItemFlags flags = (Qt::ItemIsEnabled | Qt::ItemIsSelectable))
                : m_orig(orig)
                , m_proxy(proxy)
                , m_name(std::move(name))
                , m_title(std::move(title))
                , m_flags(flags) {}

    inline ProxyColumn() :
        m_orig(-1)
        , m_proxy(-1)
        , m_name("")
        , m_title("")
        , m_flags(Qt::ItemIsEnabled | Qt::ItemIsSelectable)
    {}

    inline ProxyColumn(const ProxyColumn &column) :
        m_orig(column.m_orig)
        , m_proxy(column.m_proxy)
        , m_name(std::move(column.m_name))
        , m_title(std::move(column.m_title))
    {}

    // ~ProxyColumn() {}

    inline int orig() const { return m_orig; }
    inline int proxy() const { return m_proxy; }
    inline QString name() const { return  m_name; }
    inline QString title() const { return  m_title; }
    inline Qt::ItemFlags flags() const { return m_flags; }

    inline void setOrig(int value) { m_orig = value; }
    inline void setProxy(int value) { m_proxy = value; }
    inline void setName(QString name) { m_name = name; }
    inline void setTitle(QString title) { m_title = title; }
    inline void setFlags(Qt::ItemFlags flags) { m_flags = flags; }

    inline bool operator == (const ProxyColumn &column) {
        if (m_orig != column.m_orig) return false;
        if (m_proxy != column.m_proxy) return false;
        return true;
    }

    inline bool operator != (const ProxyColumn &column) {
        if (m_orig != column.m_orig) return true;
        if (m_proxy != column.m_proxy) return true;
        return false;
    }

    inline ProxyColumn operator = (const ProxyColumn &column) {
        m_orig = column.m_orig;
        m_proxy = column.m_proxy;
        m_name = std::move(column.m_name);
        m_title = std::move(column.m_title);

        return *this;
    }

protected:
    int m_orig;
    int m_proxy;
    QString m_name;
    QString m_title;
    Qt::ItemFlags m_flags;
};


#endif //TESTIDENTITY_PROXYCOLUMN_H
