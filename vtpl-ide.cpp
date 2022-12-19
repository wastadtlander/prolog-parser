//== vtpl-ide.cpp - main driver for vtpl IDE GUI ==============================//
// 
//
//============================================================================//

#include <QApplication>

#include "Gui.hpp"

int main(int argc, char *argv[]) {
  QApplication ide(argc, argv);

  std::string path = "";
  if (argc == 2) {
    path = argv[1];
  }

  Gui gui(path);
  gui.show();

  return ide.exec();
}
