//== gui_tests.cpp - Qt GUI Tests  ===========================================//
// 
//
//============================================================================//

#include <QDebug>
#include <QTest>

#include "test_config.hpp"
#include "Gui.hpp"

class GUITests : public QObject {
  Q_OBJECT

private slots:

  void testEditorAndResults();
  void testQuery();
  void testTrace();
  void testSaveFile();
  
private:
  Gui gui;

};

void GUITests::testEditorAndResults() {
  auto editor = gui.findChild<QPlainTextEdit *>("editor");

  QVERIFY(editor->objectName() == "editor");

  auto results = gui.findChild<QPlainTextEdit *>("results");

  QVERIFY(results->objectName() == "results");

  std::string input = "friends(X,Y) :- likes(X,Z), likes(Y,Z).\n"
                      "likes(bill,movies).\n"
                      "likes(sally,movies).\n"
                      "likes(bob,pizza).";
  editor->setPlainText(QString::fromStdString(input));

  QTest::keyClick(editor, Qt::Key_P, Qt::ControlModifier);

  std::string output = results->toPlainText().toStdString();
  QVERIFY(output == "Parsing Successful");
}

void GUITests::testQuery() {
  auto editor = gui.findChild<QPlainTextEdit *>("editor");

  QVERIFY(editor->objectName() == "editor");

  auto results = gui.findChild<QPlainTextEdit *>("results");

  QVERIFY(results->objectName() == "results");

  std::string input = "friends(X,Y) :- likes(X,Z), likes(Y,Z).\n"
                      "likes(bill,movies).\n"
                      "likes(sally,movies).\n"
                      "likes(bob,pizza).";
  editor->setPlainText(QString::fromStdString(input));

  QTest::keyClick(editor, Qt::Key_P, Qt::ControlModifier);

  auto query = gui.findChild<QLineEdit *>("query");

  QVERIFY(query->objectName() == "query");

  std::string q = "friends(X,Y).";
  query->setText(QString::fromStdString(q));

  QTest::keyClick(query, Qt::Key_Enter);

  std::string output = results->toPlainText().toStdString();
  QVERIFY(output == "(friends(bill,bill))\n"
                    "(friends(sally,bill))\n" 
                    "(friends(bill,sally))\n"
                    "(friends(sally,sally))\n"
                    "(friends(bob,bob))\n");
}

void GUITests::testTrace() {
  auto checkbox = gui.findChild<QCheckBox *>("trace");

  QVERIFY(checkbox->objectName() == "trace");

  QTest::mouseClick(checkbox, Qt::LeftButton, Qt::NoModifier);

  QVERIFY(checkbox->isChecked());
}

void GUITests::testSaveFile() {
  auto editor = gui.findChild<QPlainTextEdit *>("editor");

  QVERIFY(editor->objectName() == "editor");

  std::string input = "friends(X,Y) :- likes(X,Z), likes(Y,Z).\n"
                      "likes(bill,movies).\n"
                      "likes(sally,movies).\n"
                      "likes(bob,pizza).";

  editor->setPlainText(QString::fromStdString(input));

  QVERIFY(gui.saveFile("G:\\My Drive\\Fall 2022\\Applied Software Design\\Code\\ece3574-fall22-vtpl-wastadtlander\\tests\\dog.pro"));
}

QTEST_MAIN(GUITests)
#include "GUITests.moc"

