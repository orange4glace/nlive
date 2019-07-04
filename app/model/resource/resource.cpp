#include "model/resource/resource.h"

namespace nlive {

Resource::Resource(QString type, QString path) :
  type_(type), path_(path) {

}

QString const& Resource::type() const {
  return type_;
}

QString const& Resource::path() const {
  return path_;
}

}