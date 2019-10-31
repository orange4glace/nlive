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

private:
  std::vector<Div*> divs_;
  std::map<Div*, sig2_conn_t> div_conns_;

  inline void doLayout() {
    int container_width = width();
    std::vector<int> widths;
    widths.resize(divs_.size());
    for (auto& x : widths) x = 0;
    float flex_sum = 0;
    float flex_grow_sum = 0;
    float flex_shrink_sum = 0;
    int width_sum = 0;
    for (auto div : divs_) {
      flex_sum += div->flex();
      flex_grow_sum = div->flex_grow();
      flex_shrink_sum += div->flex_shrink();
    }
    if (flex_grow_sum == 0) flex_grow_sum = 1;
    if (flex_shrink_sum == 0) flex_shrink_sum = 1;
    for (int i = 0; i < divs_.size(); i ++) {
      auto div = divs_[i];
      int w = std::max(div->flex_basis(), (int)(div->flex() / flex_sum * container_width));
      widths[i] += w;
      width_sum += w;
    }
    if (width_sum > container_width) {
      int shrink_total = width_sum - container_width;
      for (int i = 0; i < divs_.size(); i ++) {
        auto div = divs_[i];
        int s = div->flex_shrink() / flex_shrink_sum * shrink_total;
        widths[i] -= s;
        width_sum -= s;
      }
      if (divs_.size() > 0) widths[divs_.size() - 1] -= (width_sum - container_width);
    }
    else if (width_sum < container_width) {
      int grow_total = container_width - width_sum;
      for (int i = 0; i < divs_.size(); i ++) {
        auto div = divs_[i];
        int s = div->flex_grow() / flex_grow_sum * grow_total;
        widths[i] += s;
        width_sum += s;
      }
      if (divs_.size() > 0) widths[divs_.size() - 1] += (container_width - width_sum);
    }
    int x = 0, y = 0;
    for (int i = 0; i < divs_.size(); i ++) {
      auto div = divs_[i];
      div->setGeometry(x, y, widths[i], height());
      x += widths[i];
    }
  }

protected:
  inline bool event(QEvent* e) override {
    switch (e->type()) {
    case QEvent::LayoutRequest:
    case QEvent::Resize:
      doLayout();
      return true;
    }
    return false;
  }

public:
  inline FlexLayout(QWidget* parent) :
    Div(parent) {

  }

  inline void addChild(Div* div) {
    div->setParent(this);
    divs_.push_back(div);
    updateGeometry();
  }

};

}

#endif