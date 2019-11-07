#ifndef NLIVE_STORAGE_ITEM_H_
#define NLIVE_STORAGE_ITEM_H_

#include <QObject>
#include <QSharedPointer>
#include <QString>

#include "model/common/rational.h"

namespace nlive {

class Clip;

class StorageItem : public QObject {
  Q_OBJECT

private:
  std::string uuid_;
  std::string type_;
  QString name_;

  StorageItem* parent_;

protected:
  StorageItem(std::string type, QString name, QSharedPointer<StorageItem> parent = nullptr, std::string uuid = std::string());
  
public:
  void setParent(StorageItem* item);
  QString getAbsoluteNamePath() const;
  std::string getAbsolutePath() const;

  virtual QSharedPointer<Clip> cliperize(Rational time_base) = 0;

  StorageItem* parent();

  std::string const& uuid() const;
  QString const& name() const;
  std::string const& type() const;
  virtual bool is_directory() const = 0;

};

}

#endif