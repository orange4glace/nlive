#include <QApplication>

#include <QSharedPointer>
#include "browser/main_window.h"
#include "browser/widgets_service.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto widgets_service = new QSharedPointer<nlive::WidgetsService>(
        new nlive::WidgetsService());
    auto app_window = new nlive::MainWindow(*widgets_service);

    app_window->show();

    return a.exec();
}
