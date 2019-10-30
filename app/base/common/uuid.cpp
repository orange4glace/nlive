#include "base/common/uuid.h"

#include <string>
#include <boost/lexical_cast.hpp>

namespace nlive {

UUID* UUID::instance_ = new UUID();

UUID::UUID() {}

UUID* UUID::instance() {
  return UUID::instance_;
}

std::string UUID::generateUUID() {
  boost::uuids::uuid uuid = generator_();
  return boost::lexical_cast<std::string>(uuid);
}

}