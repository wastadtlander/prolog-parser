#include "Gui.hpp"

Gui::Gui(std::string path) {
    mainLayout = new QVBoxLayout;

    createMenu();
    createPlainTextEdits();
    createBottomRow();

    mainLayout->setMenuBar(menu);
    mainLayout->addWidget(hLayoutWrapper);
    mainLayout->addWidget(hLayoutWrapper2);

    setLayout(mainLayout);

    if (path != "") {
        openFileOnStartup(path);
    }
}

Gui::~Gui() {
    delete mainLayout;
}

bool Gui::openFile(const std::string & filename) {
    std::ifstream file(filename);
    if (!file.good()) {
        return false;
    } else {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string input = buffer.str();
        f = QString::fromStdString(filename);
        kbInput = input;
        defaultFile = input;
        kb = vtpl::parseKnowledgeBase(input);
    }
    return true;
}

bool Gui::saveFile(const std::string & filename) {
    std::fstream file(filename, std::fstream::out);
    if (!file.good()) {
        return false;
    }
    file.clear();
    file << kbInput;
    return true;
}

bool Gui::openFileOnStartup(const std::string & filename) {
    std::ifstream file(filename);
    if (!file.good()) {
        std::cerr << "Error: File " << filename << " is invalid" << std::endl;
        return EXIT_FAILURE;
    } else {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string input = buffer.str();
        f = QString::fromStdString(filename);
        kbInput = input;
        defaultFile = input;
        kb = vtpl::parseKnowledgeBase(input);
    }
    return true;
}

bool Gui::saveFileQT() {
    std::fstream file(f.toStdString());
    if (!file.good()) {
        f = QFileDialog::getOpenFileName(this);
        file.open(f.toStdString());
    } else if (f.toStdString().compare(f.toStdString()) != 0) {
        switch(createMessageBox(2)) {
            case QMessageBox::Yes:
                f = QString::fromStdString(f.toStdString());
                break;
            case QMessageBox::Cancel:
                return false;
        }
    }
    file.clear();
    file << editor->toPlainText().toStdString();
    return true;
}

int Gui::createMessageBox(int version) {
    if (version == 1) {
        confirm.setText("Knowledge base cannot be saved to file.");
        confirm.setInformativeText("Action aborted.");
        confirm.setStandardButtons(QMessageBox::Cancel);
        return confirm.exec();
    } else if (version == 2) {
        confirm.setText("File already exists.");
        confirm.setInformativeText("Do you want to overwrite?");
        confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        return confirm.exec();
    }
    confirm.setText("This knowledge base has unsaved changes.");
    confirm.setInformativeText("Do you want to save your changes?");
    confirm.setStandardButtons(QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel);
    return confirm.exec();
}

void Gui::createMenu() {
    menu = new QMenuBar;
    menu->setObjectName("menu");

    file = new QMenu(tr("&File"), this);

    newProgram = new QAction(tr("&New"), this);
    connect(newProgram, &QAction::triggered, this, &Gui::newProgramAction);

    open = new QAction(tr("&Open"), this);
    connect(open, &QAction::triggered, this, &Gui::openAction);

    save = new QAction(tr("&Save"), this);
    connect(save, &QAction::triggered, this, &Gui::saveAction);

    saveAs = new QAction(tr("&Save As"), this);
    connect(saveAs, &QAction::triggered, this, &Gui::saveAsAction);

    quit = new QAction(tr("&Quit"), this);
    connect(quit, &QAction::triggered, this, &Gui::quitAction);

    file->addAction(newProgram);
    file->addAction(open);
    file->addAction(save);
    file->addAction(saveAs);
    file->addAction(quit);

    menu->addMenu(file);
}

void Gui::newProgramAction() {
    if (!saved) {
        switch (Gui::createMessageBox()) {
            case QMessageBox::Discard:
                kb = std::tuple<vtpl::ParseError, vtpl::KnowledgeBase>();
                saved = true;
                shouldTrace = false;
                kbInput.clear();
                defaultFile.clear();
                f.clear();
                editor->clear();
                results->clear();
                query->clear();
                trace->setChecked(false);
                break;
            case QMessageBox::Save:
                f = QFileDialog::getOpenFileName(this);
                temp = f.toStdString();
                if (!saveFileQT()) {
                    createMessageBox(1);
                } else {
                    kb = std::tuple<vtpl::ParseError, vtpl::KnowledgeBase>();
                    saved = true;
                    shouldTrace = false;
                    kbInput.clear();
                    defaultFile.clear();
                    f.clear();
                    editor->clear();
                    results->clear();
                    query->clear();
                    trace->setChecked(false);
                }
                break;
            case QMessageBox::Cancel:
                break;
        }
    } else {
        kb = std::tuple<vtpl::ParseError, vtpl::KnowledgeBase>();
        saved = true;
        shouldTrace = false;
        kbInput.clear();
        defaultFile.clear();
        f.clear();
        editor->clear();
        results->clear();
        query->clear();
        trace->setChecked(false);
    }
}

void Gui::openAction() {
    if (!saved) {
        switch (Gui::createMessageBox()) {
            case QMessageBox::Discard:
                f = QFileDialog::getOpenFileName(this);
                temp = f.toStdString();
                openFile(temp);
                if (std::get<0>(kb).isSet()) {
                    results->setPlainText(QString::fromStdString("Error: " + std::get<0>(kb).message()));
                }
                editor->setPlainText(QString::fromStdString(kbInput));
                results->setPlainText(QString::fromStdString("Parsing Successful"));
                break;
            case QMessageBox::Save:
                temp = f.toStdString();
                if (!saveFileQT()) {
                    createMessageBox(1);
                } else {
                    f = QFileDialog::getOpenFileName(this);
                    temp = f.toStdString();
                    openFile(temp);
                    if (std::get<0>(kb).isSet()) {
                        results->setPlainText(QString::fromStdString("Error: " + std::get<0>(kb).message()));
                    }
                    editor->setPlainText(QString::fromStdString(kbInput));
                    results->setPlainText(QString::fromStdString("Parsing Successful"));
                }
                break;
            case QMessageBox::Cancel:
                break;
        }
    } else {
        f = QFileDialog::getOpenFileName(this);
        temp = f.toStdString();
        openFile(temp);
        if (std::get<0>(kb).isSet()) {
            results->setPlainText(QString::fromStdString("Error: " + std::get<0>(kb).message()));
        }
        editor->setPlainText(QString::fromStdString(kbInput));
        results->setPlainText(QString::fromStdString("Parsing Successful"));
    }
}

void Gui::saveAction() {
    temp = f.toStdString();
    saveFileQT();
}

void Gui::saveAsAction() {
    temp = QFileDialog::getOpenFileName(this).toStdString();
    saveFileQT();
}

void Gui::quitAction() {
    if (!saved) {
        switch (Gui::createMessageBox()) {
            case QMessageBox::Discard:
                QWidget::close();
                break;
            case QMessageBox::Save:
                temp = f.toStdString();
                if (!saveFileQT()) {
                    createMessageBox(1);
                } else {
                    QWidget::close();
                }
                break;
            case QMessageBox::Cancel:
                break;
        }
    } else {
        QWidget::close();
    }
}

void Gui::createPlainTextEdits() {
    hLayoutWrapper = new QGroupBox;

    QHBoxLayout *hLayout = new QHBoxLayout;

    editor = new QPlainTextEdit;
    editor->setObjectName("editor");

    connect(editor, &QPlainTextEdit::textChanged, this, &Gui::isChanged);

    results = new QPlainTextEdit;
    results->setObjectName("results");
    results->setReadOnly(true);

    hLayout->addWidget(editor);
    hLayout->addWidget(results);

    hLayoutWrapper->setLayout(hLayout);
}

void Gui::createBottomRow() {
    hLayoutWrapper2 = new QGroupBox;

    QHBoxLayout *hLayout = new QHBoxLayout;

    queryLabel = new QLabel("Query: ", this);
    query = new QLineEdit(this);
    query->setObjectName("query");

    trace = new QCheckBox("&Trace", this);
    trace->setObjectName("trace");

    connect(trace, &QCheckBox::stateChanged, this, &Gui::setShouldTrace);

    hLayout->addWidget(queryLabel);
    hLayout->addWidget(query);
    hLayout->addWidget(trace);

    hLayoutWrapper2->setLayout(hLayout);
}

void Gui::setShouldTrace() {
    shouldTrace = trace->isChecked();
    std::get<1>(kb).setTrace(shouldTrace);
}

bool Gui::parseKB() {
    kbInput = editor->toPlainText().toStdString();
    kb = vtpl::parseKnowledgeBase(kbInput);
    if (std::get<0>(kb).isSet()) {
        results->setPlainText(QString::fromStdString("Error: " + std::get<0>(kb).message()));
        return EXIT_FAILURE;
    }
    results->setPlainText(QString::fromStdString("Parsing Successful"));
    return true;
}

bool Gui::queryKB() {
    if (!saved) {
        parseKB();
        setShouldTrace();
    }
    std::string input = query->text().toStdString();
    std::pair<vtpl::ParseError, vtpl::ExpressionTreeNode> query = vtpl::parseQuery(input);
    if (query.first.isSet()) {
        results->setPlainText(QString::fromStdString("Parse Error: " + query.first.message()));
    } else {
        std::string toWrite;
        std::list<vtpl::Substitution> sub = std::get<1>(kb).ask(query.second);
        qDebug("%i", shouldTrace);
        if (sub.empty()) {
            results->setPlainText(QString::fromStdString("false."));
        } else {
            toWrite += std::get<1>(kb).getLog();
            for (vtpl::Substitution s : sub) {
                std::string output = vtpl::apply(query.second, s).toString();
                toWrite += output + '\n';
            }
            results->setPlainText(QString::fromStdString(toWrite));
        }  
    }
    return true;
}

void Gui::isChanged() {
    if (defaultFile.compare(editor->toPlainText().toStdString()) != 0) {
        saved = false;
    } else {
        saved = true;
    }
    kbInput = editor->toPlainText().toStdString();
}

void Gui::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::Print)) {
        parseKB();
    } else if (event->matches(QKeySequence::InsertParagraphSeparator)) {
        queryKB();
    }
}