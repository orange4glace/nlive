#include <QApplication>

#include "browser/main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto app_window = new nlive::MainWindow();

    app_window->show();

    return a.exec();
}
