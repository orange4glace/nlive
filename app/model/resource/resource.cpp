#include "model/resource/resource.h"

namespace nlive {

Resource::Resource(std::string type, QUrl url) :
  type_(type), url_(url) {

}

std::string const& Resource::type() const {
  return type_;
}

QUrl const& Resource::url() const {
  return url_;
}

}