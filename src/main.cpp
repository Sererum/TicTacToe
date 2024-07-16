#include "menu_view/menuwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MenuWindow w;
  w.resize(600, 400);
  w.show();
  return app.exec();
}
