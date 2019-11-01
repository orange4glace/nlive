#ifndef NLIVE_FLEX_BOX_H_
#define NLIVE_FLEX_BOX_H_

#include <QDebug>
#include <QWidget>
#include <QEvent>
#include <vector>
#include <map>
#include <algorithm>
#include "base/common/sig.h"
#include "base/layout/div.h"

namespace nlive {

class FlexLayout : public Div {

public:
  enum Direction {
    Column = 1,
    Row = 2
  };

private:
  FlexLayout::Direction flex_direction_;

  std::vector<Div*> divs_;
  std::map<Div*, sig2_conn_t> div_conns_;

  inline void doLayout() {
    int container_size;
    if (flex_direction_ == FlexLayout::Column) container_size = width();
    else container_size = height();
    std::vector<int> distrib;
    distrib.resize(divs_.size());
    for (auto& x : distrib) x = 0;
    float flex_sum = 0;
    float flex_grow_sum = 0;
    float flex_shrink_sum = 0;
    int content_size_sum = 0;
    for (auto div : divs_) {
      flex_sum += div->flex();
      flex_grow_sum = div->flex_grow();
      flex_shrink_sum += div->flex_shrink();
    }
    if (flex_grow_sum == 0) flex_grow_sum = 1;
    if (flex_shrink_sum == 0) flex_shrink_sum = 1;
    for (int i = 0; i < divs_.size(); i ++) {
      auto div = divs_[i];
      int w;
      if (div->flex_basis() != -1) {
        w = div->flex_basis();
      }
      else {
        w = (int)(div->flex() / flex_sum * container_size);
      }
      distrib[i] += w;
      content_size_sum += w;
    }
    if (content_size_sum > container_size) {
      int shrink_total = content_size_sum - container_size;
      for (int i = 0; i < divs_.size(); i ++) {
        auto div = divs_[i];
        int s = div->flex_shrink() / flex_shrink_sum * shrink_total;
        distrib[i] -= s;
        content_size_sum -= s;
      }
      if (divs_.size() > 0) distrib[divs_.size() - 1] -= (content_size_sum - container_size);
    }
    else if (content_size_sum < container_size) {
      int grow_total = container_size - content_size_sum;
      for (int i = 0; i < divs_.size(); i ++) {
        auto div = divs_[i];
        int s = div->flex_grow() / flex_grow_sum * grow_total;
        distrib[i] += s;
        content_size_sum += s;
      }
      if (divs_.size() > 0) distrib[divs_.size() - 1] += (container_size - content_size_sum);
    }
    int x = 0, y = 0;
    for (int i = 0; i < divs_.size(); i ++) {
      auto div = divs_[i];
      if (flex_direction_ == FlexLayout::Column) {
        div->setGeometry(x, y, distrib[i], height());
        x += distrib[i];
      }
      else {
        div->setGeometry(x, y, width(), distrib[i]);
        y += distrib[i];
      }
    }
  }

protected:
  inline bool event(QEvent* e) override {
    switch (e->type()) {
    case QEvent::LayoutRequest:
      return true;
    }
    return Div::event(e);
  }

  inline void contentRectUpdated() override {
    doLayout();
  }

public:
  inline FlexLayout(QWidget* parent, FlexLayout::Direction flex_direction = FlexLayout::Column) :
    Div(parent), flex_direction_(flex_direction) {

  }

  inline void addChild(Div* div) {
    div->setParent(this);
    divs_.push_back(div);
    updateGeometry();
  }

  inline void removeChild(Div* div) {
    divs_.erase(std::remove(divs_.begin(), divs_.end(), div), divs_.end());
    updateGeometry();
  }

  inline bool flex_direction() { return flex_direction_; }

};

}

#endif