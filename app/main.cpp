#include <QApplication>
#include <any>

#include "base/common/memory.h"
#include "browser/main_window.h"
#include "browser/widgets_service.h"
#include <qopengl.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    auto widgets_service = new sptr<nlive::WidgetsService>(
        new nlive::WidgetsService());
    auto app_window = new nlive::MainWindow(*widgets_service);

    app_window->show();

    return a.exec();
}
