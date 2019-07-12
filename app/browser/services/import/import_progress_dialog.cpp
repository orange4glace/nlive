#include "browser/services/import/import_progress_dialog.h"

#include "platform/resource/resource_service.h"
#include "model/storage/storage_directory.h"
#include "model/storage/resource_storage_item.h"

#include <QUrl>
#include <QList>
#include <QPainter>
#include <QDebug>

namespace nlive {

ImportProgressDialog::ImportProgressDialog(int total) :
  total_(total), progress_(0) {

}

void ImportProgressDialog::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.drawText(QPoint(0, 20), QString::number(progress_));
}

int ImportProgressDialog::total() const {
  return total_;
}

int ImportProgressDialog::progress() const {
  return progress_;
}

void ImportProgressDialog::setProgress(int value) {
  progress_ = value;
}

}