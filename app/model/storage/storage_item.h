#ifndef NLIVE_STORAGE_ITEM_H_
#define NLIVE_STORAGE_ITEM_H_

#include <QObject>
#include "base/common/memory.h"
#include <QString>
#include <boost/serialization/serialization.hpp>
#include "base/common/sig.h"
#include "model/sequence/sequence.h"
#include "model/common/rational.h"

namespace nlive {

// We have a circular dependency here. (Project <----> StorageItem)
class Project;
class Clip;

class StorageItem : public Sig {

private:
  // friend class boost::serialization::access;
  // template <class Archive>
  // void serialize(Archive& ar, const unsigned int version) {
  //   ar << uuid_ << type_ << name_.toStdString();
  // }

  std::string uuid_;
  std::string type_;
  QString name_;

  sptr<Project> project_;
  StorageItem* parent_;

protected:
  StorageItem(
    sptr<Project> project,
    std::string type, QString name,
    sptr<StorageItem> parent = nullptr,
    std::string uuid = std::string());
  
public:
  void setParent(StorageItem* item);
  QString getAbsoluteNamePath() const;
  std::string getAbsolutePath() const;

  virtual sptr<Clip> cliperize(sptr<Sequence> sequence) = 0;

  StorageItem* parent();
  inline sptr<Project> project() { return project_; }

  std::string const& uuid() const;
  QString const& name() const;
  std::string const& type() const;
  virtual bool is_directory() const = 0;

};

}

#endif