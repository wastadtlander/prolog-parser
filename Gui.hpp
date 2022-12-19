#ifndef GUI_HPP
#define GUI_HPP

#include <QWidget>

#include <QCheckBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QTextStream>

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "KnowledgeBase.hpp"
#include "KnowledgeBaseUtility.hpp"
#include "Parser.hpp"

class Gui : public QWidget {
    Q_OBJECT
public:
    Gui(std::string path = "");

    ~Gui();

    Gui & operator=(Gui gui) = delete;

    Gui & operator=(Gui && gui) = delete;

    bool openFile(const std::string & filename);

    bool saveFile(const std::string & filename);
private:
    bool openFileOnStartup(const std::string & filename);

    bool saveFileQT();

    QVBoxLayout *mainLayout;

    int createMessageBox(int version = 0);

    QMessageBox confirm;

    void createMenu();

    QMenuBar *menu;

    QMenu *file;

    void newProgramAction();
    void openAction();
    void saveAction();
    void saveAsAction();
    void quitAction();

    QAction *newProgram;
    QAction *open;
    QAction *save;
    QAction *saveAs;
    QAction *quit;

    QFileDialog dialog;
    QString f;
    std::string defaultFile;
    std::string kbInput;

    void createPlainTextEdits();

    QGroupBox *hLayoutWrapper;
    
    QPlainTextEdit *editor;
    QPlainTextEdit *results;

    void createBottomRow();

    QGroupBox *hLayoutWrapper2;

    QLabel *queryLabel;
    QLineEdit *query;

    void setShouldTrace();

    QCheckBox *trace;

    bool parseKB();

    bool queryKB();

    std::tuple<vtpl::ParseError, vtpl::KnowledgeBase> kb;

    void isChanged();

    bool saved = true;
    bool shouldTrace = false;

    void keyPressEvent(QKeyEvent *event);

    std::string temp;
};

#endif