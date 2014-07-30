#ifndef LIBFACEBOOK_CLIENTPRIVATE_H
#define LIBFACEBOOK_CLIENTPRIVATE_H

#include <QWebPage>

#include <libblogger/blog.hpp>

#include <libtwirl/web/pageloader.hpp>
#include <libtwirl/web/requestloader.hpp>

class QNetworkAccessManager;
class QNetworkReply;
class QWebFrame;

namespace blogger {

class ClientPrivate {
public:
    ClientPrivate();
    virtual ~ClientPrivate();
    
    QList<Blog> open(const QUrl & url);
    QByteArray save(const QUrl & url);
    
    QUrl newer() const;
    QUrl older() const;

private:
    bool load();
    bool load(const QUrl & url);
    
    QByteArray read(QNetworkReply * const reply);
        
    Blog decode(const QWebElement & post);
    
    QString imageUrl(const QWebElement & imageUrl);
    QString blogId(const QWebElement & blogId);
    QString postId(const QWebElement & postId);
    QString title(const QWebElement & title);
    QList<QUrl> images(const QWebElement & body);
    
    void setMedia(bool media = true);
    void resetMedia();
    
    static const QUrl cookieUrl;

    QWebPage page;
    QWebFrame & frame;
    QNetworkAccessManager & manager;

    twirl::PageLoader pageLoader;
    twirl::RequestLoader requestLoader;
    
    QUrl next;
    QUrl prev;
};

} /* namespace blogger */

#endif // LIBFACEBOOK_CLIENTPRIVATE_H
