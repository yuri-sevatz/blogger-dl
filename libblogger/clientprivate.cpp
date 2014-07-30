#include "clientprivate.hpp"

#include <libblogger/include/config.h>

#ifdef VERBOSE_OUTPUT
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QUrl>
#include <QUrlQuery>

#include <QWebFrame>
#include <QWebElement>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <QtXml/QDomDocument>

namespace blogger {

const QUrl ClientPrivate::cookieUrl("http://blogspot.ca");

ClientPrivate::ClientPrivate() :
    frame(*page.mainFrame()),
    manager(*page.networkAccessManager()),
    pageLoader(page)
{
    resetMedia();
}

ClientPrivate::~ClientPrivate() {}

inline bool ClientPrivate::load() {
#ifdef VERBOSE_OUTPUT
    qDebug() << "***Click***";
#endif
    const bool result = pageLoader.load();
#ifdef VERBOSE_OUTPUT
    qDebug() << "***Done***";
#endif
    return result;
}

inline bool ClientPrivate::load(const QUrl & url) {
#ifdef VERBOSE_OUTPUT
    qDebug() << "***Link***";
#endif

    const bool result = pageLoader.load(url);
#ifdef VERBOSE_OUTPUT
    if (result) {
        qDebug() << "***Done***";
    }
#endif
    return result;
}

QByteArray ClientPrivate::read(QNetworkReply * const reply) {
    return reply ? QScopedPointer<QNetworkReply>(requestLoader.load(reply))->readAll() : QByteArray();
}

QUrl ClientPrivate::newer() const {
    return next;
}

QUrl ClientPrivate::older() const {
    return prev;
}

QList<Blog> ClientPrivate::open(const QUrl & url) {
    QList<Blog> ret;
    if (load(url)) {
        const QWebElementCollection blogs = frame.findAllElements("[itemtype=\"http://schema.org/BlogPosting\"]");
        
        Q_FOREACH(const QWebElement & blog, blogs) {
            ret << decode(blog);
        }
        
        const QWebElement newer = frame.findFirstElement("#blog-pager-newer-link a");
        next = newer.isNull() ? QUrl() : QUrl(newer.attribute("href"));
        
        const QWebElement older = frame.findFirstElement("#blog-pager-older-link a");   
        prev = older.isNull() ? QUrl() : QUrl(older.attribute("href"));
    } else {
        next.clear();
        prev.clear();
    }
    return ret;
}

Blog ClientPrivate::decode(const QWebElement & post) {
    Blog blog;
    
    blog.setUrl(frame.url());
    blog.setImageUrl(imageUrl(post.findFirst("[itemprop=\"image_url\"]")));
    blog.setBlogId(blogId(post.findFirst("[itemprop=\"blogId\"]")));
    blog.setPostId(postId(post.findFirst("[itemprop=\"postId\"]")));
    blog.setTitle(title(post.findFirst("[itemprop=\"name\"]")));
    blog.setImages(images(post.findFirst("[itemprop~=\"articleBody\"]")));
    
    return blog;
}

QString ClientPrivate::imageUrl(const QWebElement & imageUrl) {
    return imageUrl.attribute("content");
}

QString ClientPrivate::blogId(const QWebElement & blogId) {
    return blogId.attribute("content");
}

QString ClientPrivate::postId(const QWebElement & postId) {
    return postId.attribute("content");
}

QString ClientPrivate::title(const QWebElement & title) {
    // Very silly/inefficient.  Probably a better way to do this!
    QDomDocument doc;
    doc.setContent(title.toOuterXml());
    return doc.documentElement().text().simplified();
}

QList<QUrl> ClientPrivate::images(const QWebElement & body) {
    QList<QUrl> ret;
    const QWebElementCollection images = body.findAll("a > img");
    Q_FOREACH(const QWebElement & image, images) {
        QWebElement link = image.parent();
        do {
            if (link.tagName().startsWith('a', Qt::CaseInsensitive)) {
                break;
            } else { 
                link = link.parent();
            }
        } while (!link.isNull());
        
        const QString src = link.attribute("href");
        if (!src.isEmpty()) {
            ret << src;
        }
    }
    return ret;
}

QByteArray ClientPrivate::save(const QUrl& url) {
    QUrl src = url;
    
    do {
        const QScopedPointer<QNetworkReply>reply(requestLoader.load(manager.get(QNetworkRequest(src))));
        const QVariant contentType = reply->header(QNetworkRequest::ContentTypeHeader);
        const QByteArray data = reply->readAll();
        // We use an HTML decoder to basically handle these...
        // (Whoever does this for downloads is breaking pretty much ever rule ever!)
        if(contentType.toString().startsWith("text/html")) {
            frame.setHtml(data, reply->url());
            src = frame.findFirstElement("img").attribute("src");
            if (!src.isEmpty()) {
                continue;
            }
        }
        return data;
    } while (true);
}

void ClientPrivate::setMedia(bool media) {
    QWebSettings * const settings = page.settings();
    settings->setAttribute(QWebSettings::AutoLoadImages, media);
    settings->setAttribute(QWebSettings::PrintElementBackgrounds, media);
}

void ClientPrivate::resetMedia() {
        setMedia(false);
}

} /* namespace blogger */
