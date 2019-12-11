#include <QApplication>
#include <any>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
}
#include "base/common/memory.h"
#include "browser/main_window.h"
#include "browser/widgets_service.h"

int main(int argc, char *argv[])
{
    av_register_all();
    QApplication a(argc, argv);
    
    auto widgets_service = new sptr<nlive::WidgetsService>(
        new nlive::WidgetsService());
    auto app_window = new nlive::MainWindow(*widgets_service);

    app_window->show();

    return a.exec();
}
