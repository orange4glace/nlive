#ifndef NLIVE_RESOURCE_H_
#define NLIVE_RESOURCE_H_

#include <QObject>
#include <QString>
#include <QUrl>
#include "base/common/serialize.h"

namespace nlive {

using ResourceIdentifier = QString;

class Resource : public QObject {
  Q_OBJECT

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & id_ & type_ & path_ & name_;
  }

protected:
  ResourceIdentifier id_;
  std::string type_;
  std::string path_;
  QString name_;

protected:
  Resource() = default;
  Resource(std::string type, std::string path, QString name);

public:
  std::string const& type() const;
  std::string const& path() const;
  QString const& name() const;

};

}

#endif