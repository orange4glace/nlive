#ifndef _NLIVE_RESOURCE_H_
#define _NLIVE_RESOURCE_H_

#include <QObject>
#include <QString>

namespace nlive {

class Resource : public QObject {
  Q_OBJECT

private:
  QString type_;
  QString path_;

protected:
  Resource(QString type, QString path);

public:
  QString& type() const;
  QString& path() const;

};

}

#endif