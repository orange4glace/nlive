#ifndef _NLIVE_RESOURCE_H_
#define _NLIVE_RESOURCE_H_

#include <QObject>
#include <QString>
#include <QUrl>

namespace nlive {

using ResourceIdentifier = QString;

class Resource : public QObject {
  Q_OBJECT

protected:
  ResourceIdentifier id_;
  std::string type_;
  std::string path_;

protected:
  Resource(std::string type, std::string path);

public:
  std::string const& type() const;
  std::string const& path() const;

};

}

#endif