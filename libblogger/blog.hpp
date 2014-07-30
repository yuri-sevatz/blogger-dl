#ifndef BLOG_HPP
#define BLOG_HPP

#include <QSharedDataPointer>

#include <QList>
#include <QString>
#include <QUrl>

namespace blogger {

class BlogData;

class Blog {
public:
    Blog();
    Blog(const Blog& other);
    virtual ~Blog();
    
    const QUrl & url() const;
    void setUrl(const QUrl & url);
    void resetUrl();
    
    const QUrl & imageUrl() const;
    void setImageUrl(const QUrl & imageUrl);
    void resetImageUrl();
    
    const QString & blogId() const;
    void setBlogId(const QString & blogId);
    void resetBlogId();

    const QString & postId() const;
    void setPostId(const QString & postId);
    void resetPostId();
    
    const QString & title() const;
    void setTitle(const QString & title);
    void resetTitle();
    
    const QList<QUrl> & images() const;
    void setImages(const QList<QUrl> & images);
    void resetImages();
    
    Blog& operator=(const Blog& other);
    bool operator==(const Blog& other);

private:
    friend class BlogData;
    
    QSharedDataPointer<BlogData> d_ptr;
};

} /* namespace blogger */

#endif // BLOG_HPP
