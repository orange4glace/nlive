#include "model/resource/resource.h"

namespace nlive {

Resource::Resource(std::string type, std::string path) :
  type_(type), path_(path) {

}

std::string const& Resource::type() const {
  return type_;
}

std::string const& Resource::path() const {
  return path_;
}

}