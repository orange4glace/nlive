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

SvgSprite::SvgSprite(QString path) :
  path_(path), pixmap_(nullptr) {
  width_ = height_ = 0;
}

SvgSprite::SvgSprite(QString path, int width, int height, std::string color) :
  path_(path), width_(width), height_(height), color_(color), pixmap_(nullptr) {

  pixmap_ = new QPixmap(width, height);
  doCreateSprite();

}

SvgSprite::~SvgSprite() {
  delete pixmap_;
}

void SvgSprite::resize(int width, int height) {
  if (width_ == width && height_ == height) return;
  width_ = width;
  height_ = height;
  doCreateSprite();
}

void SvgSprite::setColor(std::string color) {
  color_ = color;
  doCreateSprite();
}

void SvgSprite::setColor(QColor color) {
  setColor(color.name(QColor::HexRgb).toStdString());
}

namespace {

void iter(XMLNode* el, const char* color) {
  for (XMLNode* n = el->FirstChild(); n; n = n->NextSibling()) {
    auto e = n->ToElement();
    if (strcmp(e->Name(), "path") == 0 || strcmp(e->Name(), "polygon"))
      e->SetAttribute("fill", color);
    iter(n, color);
  }
}

}

void SvgSprite::doCreateSprite() {
  if (width_ <= 0 || height_ <= 0) return;
  if (pixmap_) delete pixmap_;
  pixmap_ = new QPixmap(width_, height_);
  pixmap_->fill(Qt::transparent);

  auto xml = XMLCacheStorage.load(path_);
  iter(xml->RootElement(), color_.c_str());
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