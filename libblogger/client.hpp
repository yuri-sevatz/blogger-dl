#ifndef LIBBLOGGER_CLIENT_HPP
#define LIBBLOGGER_CLIENT_HPP

#include <QObject>

#include <libblogger/blog.hpp>
#include <libtwirl/model/session.hpp>

#include <QList>
#include <QScopedPointer>

namespace blogger {

class ClientPrivate;

class Client : private QObject {
    Q_OBJECT
public:
    Client();
    virtual ~Client();
    
    QList<Blog> open(const QUrl & url);
    QByteArray save(const QUrl & url);
    
    QUrl newer() const;
    QUrl older() const;

private:
    Q_DISABLE_COPY(Client);
    Q_DECLARE_PRIVATE(Client);

    const QScopedPointer<ClientPrivate> d_ptr;
};

} /* namespace blogger */

#endif // LIBBLOGGER_CLIENT_HPP
