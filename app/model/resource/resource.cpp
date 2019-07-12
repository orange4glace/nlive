#include "model/resource/resource.h"

namespace nlive {

Resource::Resource(QString type, QUrl url) :
  type_(type), url_(url) {

}

QString const& Resource::type() const {
  return type_;
}

QUrl const& Resource::url() const {
  return url_;
}

}