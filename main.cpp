#include <QApplication>
#include "MainWindow.h"
#include "Config.h"

int main(int argc, char **argv)
{
    QApplication *app = new QApplication(argc, argv);
    MainWindow *mw = new MainWindow();
    mw->show();
    app->exec();

	Config* cfg = Config::Current();
	cfg->save();

    return true;
}