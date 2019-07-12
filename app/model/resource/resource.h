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
  QString type_;
  QUrl url_;

protected:
  Resource(QString type, QUrl url);

public:
  QString const& type() const;
  QUrl const& url() const;

};

}

#endif