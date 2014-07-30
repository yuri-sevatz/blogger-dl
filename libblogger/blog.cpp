#include "blog.hpp"
#include "blogdata.hpp"

namespace blogger {

BlogData::BlogData() {}
BlogData::~BlogData() {}

Blog::Blog() : d_ptr(new BlogData()) {}
Blog::Blog(const Blog& other) : d_ptr(other.d_ptr) {}
Blog::~Blog() {}

const QUrl & Blog::url() const {
    return d_ptr->url;
}

void Blog::setUrl(const QUrl & url) {
    d_ptr->url = url;
}

void Blog::resetUrl() {
    d_ptr->url.clear();
}

const QUrl & Blog::imageUrl() const {
    return d_ptr->imageUrl;
}

void Blog::setImageUrl(const QUrl & imageUrl) {
    d_ptr->imageUrl = imageUrl;
}

void Blog::resetImageUrl() {
    d_ptr->imageUrl.clear();
}

const QString & Blog::blogId() const {
    return d_ptr->blogId;
}

void Blog::setBlogId(const QString & blogId) {
    d_ptr->blogId = blogId;
}

void Blog::resetBlogId() {
    d_ptr->blogId.clear();
}

const QString & Blog::postId() const {
    return d_ptr->postId;
}

void Blog::setPostId(const QString & postId) {
    d_ptr->postId = postId;
}

void Blog::resetPostId() {
    d_ptr->postId.clear();
}

const QString & Blog::title() const {
    return d_ptr->title;
}

void Blog::setTitle(const QString & title) {
    d_ptr->title = title;
}

void Blog::resetTitle() {
    d_ptr->title.clear();
}

const QList<QUrl> & Blog::images() const {
    return d_ptr->images;
}

void Blog::setImages(const QList<QUrl> & images) {
    d_ptr->images = images;
}

void Blog::resetImages() {
    d_ptr->images.clear();
}

Blog& Blog::operator=(const Blog& other) {
    d_ptr = other.d_ptr;
    return *this;
}

bool Blog::operator==(const Blog& other) {
    return d_ptr == other.d_ptr;
}

} /* namespace blogger */
