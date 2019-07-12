#ifndef _IMPORT_PROGRESS_DIALOG_H_
#define _IMPORT_PROGRESS_DIALOG_H_

#include <QDialog>

namespace nlive {

class ImportProgressDialog : public QDialog {
  
private:
  int total_;
  int progress_;

protected:
  void paintEvent(QPaintEvent* event) override;

public:
  ImportProgressDialog(int total);

  int total() const;
  int progress() const;
  void setProgress(int value);
  
};

}

#endif