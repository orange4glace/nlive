#include "base/ui/svg_sprite.h"

#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QSvgRenderer>
#include <QXmlStreamReader>
#include <memory>
#include <map>
#include <string>
#include "ext/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

namespace nlive {

namespace {

struct XMLCache {
  std::shared_ptr<XMLDocument> document;
  int ref_count;
};

class _XMLCacheStorage {

private:
  std::map<QString, XMLCache> refs_;

public:
  _XMLCacheStorage::_XMLCacheStorage() {}

  std::shared_ptr<XMLDocument> load(QString path) {
    if (refs_.count(path)) {
      auto& ref = refs_[path];
      ref.ref_count ++;
      std::shared_ptr<XMLDocument> copy = std::make_shared<XMLDocument>();
      ref.document->DeepCopy(copy.get());
      return copy;
    }
    QFile file(path);
    Q_ASSERT(file.open(QIODevice::ReadOnly));
    std::shared_ptr<XMLDocument> doc = std::make_shared<XMLDocument>();
    doc->Parse(file.readAll().toStdString().c_str());
    XMLCache ref;
    ref.document = doc;
    ref.ref_count = 1;
    refs_.insert(std::make_pair(path, ref));
    std::shared_ptr<XMLDocument> copy = std::make_shared<XMLDocument>();
    ref.document->DeepCopy(copy.get());
    return copy;
  }

  void unload(QString path) {
    Q_ASSERT(refs_.count(path));
    auto& ref = refs_[path];
    if (ref.ref_count == 1) refs_.erase(path);
    else ref.ref_count --;
  }

};

_XMLCacheStorage XMLCacheStorage;

}

SvgSprite::SvgSprite(QString path, int width, int height) :
  path_(path), width_(width), height_(height), pixmap_(nullptr) {

  pixmap_ = new QPixmap(width, height);
  doCreateSprite();

}

SvgSprite::~SvgSprite() {
  delete pixmap_;
}

void SvgSprite::doCreateSprite() {
  auto xml = XMLCacheStorage.load(path_);
  XMLPrinter printer;
  xml->Print(&printer);
  auto cstr = printer.CStr();

  QXmlStreamReader qreader(cstr);
  QSvgRenderer qrenderer(&qreader);
  QPainter p(pixmap_);
  p.setRenderHint(QPainter::RenderHint::Antialiasing);
  qrenderer.render(&p);
}


}