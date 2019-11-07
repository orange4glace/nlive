#ifndef NLIVE_STORAGE_ITEM_FACTORY_H_
#define NLIVE_STORAGE_ITEM_FACTORY_H_

#include <QObject>

#include "model/storage/storage_item.h"

namespace nlive {

template <class T>
class StorageItemFactory {

public:
  virtual T* create

};

}

#endif