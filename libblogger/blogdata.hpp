#ifndef BLOGDATA_HPP
#define BLOGDATA_HPP

#include <QList>
#include <QSharedData>
#include <QString>
#include <QUrl>

namespace blogger {

class BlogData : public QSharedData {
public:
    BlogData();
    virtual ~BlogData();

private:
    friend class Blog;
    
    QUrl url;
    QUrl imageUrl;
    QString blogId;
    QString postId;
    QString title;
    QList<QUrl> images;
};

} /* namespace blogger */

#endif // BLOGDATA_HPP
