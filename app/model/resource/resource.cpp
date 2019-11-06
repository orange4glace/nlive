#include "model/resource/resource.h"

#include <QDebug>

namespace nlive {

Resource::Resource(std::string type, std::string path, QString name) :
  type_(type), path_(path), name_(name) {
}

std::string const& Resource::type() const {
  return type_;
}

std::string const& Resource::path() const {
  return path_;
}

QString const& Resource::name() const {
  return name_;
}

}