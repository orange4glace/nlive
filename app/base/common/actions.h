#ifndef NLIVE_ACTIONS_H_
#define NLIVE_ACTIONS_H_

#include <string>
#include "base/common/sig.h"

namespace nlive {

struct IActionRunParam {

};

class IAction {

public:
  virtual std::string id() const = 0;
  virtual std::wstring label() const = 0;
  virtual std::wstring tooltip() const = 0;
  virtual bool enabled() const = 0;
  virtual bool checked() const = 0;
  virtual void run(IActionRunParam* param) = 0;

};

class Action : public IAction, public Sig {

private:
  std::string id_;
  std::wstring label_;
  std::wstring tooltip_;
  bool enabled_;
  bool checked_;

public:
  Action(std::string id, std::wstring label = L"", bool enabled = true);

  virtual void run(IActionRunParam* param) override;

  std::string id() const override;
  void setLabel(std::wstring value);
  std::wstring label() const override;
  void setTooltip(std::wstring value);
  std::wstring tooltip() const override;
  void setEnabled(bool value);
  bool enabled() const override;
  void setChecked(bool value);
  bool checked() const override;

  sig2_t<void ()> onDidChange;

};

}

#endif