#ifndef _NLIVE_STORAGE_ITEM_H_
#define _NLIVE_STORAGE_ITEM_H_

#include <QObject>
#include <QSharedPointer>
#include <QString>

#include "model/common/rational.h"

namespace nlive {

class Clip;

class StorageItem : public QObject {
  Q_OBJECT

private:
  QString uuid_;
  QString type_;
  QString name_;

  StorageItem* parent_;

protected:
  StorageItem(QString type, QString name, QSharedPointer<StorageItem> parent = nullptr, QString uuid = QString());
  
public:
  void setParent(StorageItem* item);
  QString getAbsolutePath() const;

  virtual QSharedPointer<Clip> cliperize(Rational time_base) = 0;

  StorageItem* parent();

  QString const& uuid() const;
  QString const& name() const;
  QString const& type() const;
  virtual bool is_directory() const = 0;

};

}

#endif