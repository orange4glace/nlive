#ifndef _NLIVE_CLIP_H_
#define _NLIVE_CLIP_H_

#include <QObject>

namespace nlive {

class Clip : public QObject {
  Q_OBJECT

private:
  int id_;
  int start_time_;
  int end_time_;

signals:
  void onDidChangeTime(int old_start_time, int old_end_time);

public:
  Clip(int start_time, int end_time);

  void setTime(int start_time, int end_time);

  int start_time() const;
  int end_time() const;
  int id() const;

  bool operator<(const Clip& rhs) const;

};

}

#endif