#ifndef _NLIVE_STORAGE_ITEM_H_
#define _NLIVE_STORAGE_ITEM_H_

#include <QObject>
#include <QString>

namespace nlive {

class StorageItem : public QObject {
  Q_OBJECT

private:
  QString uuid_;
  QString type_;
  QString name_;

  StorageItem* parent_;

protected:
  StorageItem(QString type, QString name, StorageItem* parent = nullptr, QString uuid = QString());
  
public:
  void setParent(StorageItem* item);
  QString getAbsolutePath() const;

  StorageItem* parent();

  QString const& uuid() const;
  QString const& name() const;
  QString const& type() const;
  virtual bool is_directory() const = 0;

};

}

#endif