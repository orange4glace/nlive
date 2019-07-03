#include <QApplication>

#include "browser/main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto main_window = new nlive::MainWindow();

    main_window->show();

    return a.exec();
}
