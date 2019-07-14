#ifndef _NLIVE_RESOURCE_H_
#define _NLIVE_RESOURCE_H_

#include <QObject>
#include <QString>
#include <QUrl>

namespace nlive {

using ResourceIdentifier = QString;

class Resource : public QObject {
  Q_OBJECT

private:
  ResourceIdentifier id_;
  std::string type_;
  QUrl url_;

protected:
  Resource(std::string type, QUrl url);

public:
  std::string const& type() const;
  QUrl const& url() const;

};

}

#endif