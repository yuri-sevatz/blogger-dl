#include "client.hpp"

#include "clientprivate.hpp"

namespace blogger {

Client::Client() :
    d_ptr(new ClientPrivate()) {

}

Client::~Client() {

}

QList<Blog> Client::open(const QUrl & url) {
    return d_ptr->open(url);
}

QByteArray Client::save(const QUrl & url) {
    return d_ptr->save(url);
}

QUrl Client::newer() const {
    return d_ptr->newer();
}

QUrl Client::older() const {
    return d_ptr->older();
}

} /* namespace blogger */
