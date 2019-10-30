#ifndef NLIVE_UUID_H_
#define NLIVE_UUID_H_

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace nlive {

class UUID {

private:
  static UUID* instance_;

  UUID();

  boost::uuids::random_generator generator_;

public:
  static UUID* instance();

  std::string generateUUID();

};

}

#endif