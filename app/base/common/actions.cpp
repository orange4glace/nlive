#include "base/common/actions.h"

namespace nlive {

Action::Action(std::string id, std::wstring label, bool enabled) :
  id_(id), label_(label), enabled_(enabled) {
  tooltip_ = L"";
  icon_ = L"";
  checked_ = false;
}

void Action::run(IActionRunParam* param) {}

std::string Action::id() const {
  return id_;
}

void Action::setLabel(std::wstring value) {
  label_ = value;
  onDidChange();
}

std::wstring Action::label() const {
  return label_;
}

void Action::setTooltip(std::wstring value) {
  tooltip_ = value;
  onDidChange();
}

std::wstring Action::tooltip() const {
  return tooltip_;
}

void Action::setIcon(std::wstring value) {
  icon_ = value;
  onDidChange();
}

std::wstring Action::icon() const {
  return icon_;
}

void Action::setEnabled(bool value) {
  enabled_ = value;
  onDidChange();
}

bool Action::enabled() const {
  return enabled_;
}

void Action::setChecked(bool value) {
  checked_ = value;
  onDidChange();
}

bool Action::checked() const {
  return checked_;
}

}