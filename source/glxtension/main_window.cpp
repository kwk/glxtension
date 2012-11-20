#include <QDebug>
#include <QCompleter>
#include "main_window.h"
#include "ui_main_window.h"
#include "gl_helper_widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    GLHelperWidget * helperWidget = new GLHelperWidget(m_ui->glWidget);

    m_ui->vendorValue->setText(helperWidget->getString(GL_VENDOR));
    m_ui->rendererValue->setText(helperWidget->getString(GL_RENDERER));
    m_ui->versionValue->setText(helperWidget->getString(GL_VERSION));

    // Collect extension info and put them into a map with key: corp and value: extension

    QString exts = helperWidget->getString(GL_EXTENSIONS);
    QStringList extList = exts.split(" ", QString::SkipEmptyParts);
    QStringListIterator itList(extList);
    while (itList.hasNext()) {
        QString ext = itList.next();
        QString corp = ext.split("_").at(1);
        m_extMap[corp].append(ext);
        //m_extensionList.append(ext.replace(0, corp.length()+4, ""));
        m_extensionList.append(ext);
    }
    m_extensionList.sort();

    // display the extension tree by faking a filter signal
    on_filterText_textChanged("");

    // What to do when an extension was double clicked.
    connect(m_ui->extensionsTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(extension_doubleClicked(QTreeWidgetItem*,int)));

    // When a specification page starts loading, is loading, and has been loaded...
    connect(m_ui->extensionSpecView, SIGNAL(loadStarted()), this, SLOT(loadSpecStarted()));
    connect(m_ui->extensionSpecView, SIGNAL(loadProgress(int)), this, SLOT(loadSpecProgress(int)));
    connect(m_ui->extensionSpecView, SIGNAL(loadFinished(bool)), this, SLOT(loadSpecFinished(bool)));

    // Add extensions to combobox
    m_ui->extensionComboBox->addItems(m_extensionList);
    connect(m_ui->extensionComboBox, SIGNAL(activated(QString)), this, SLOT(loadExtensionSpec(QString)));

    // Autocompleter for filtering extensions
    QCompleter *completer = new QCompleter(m_extensionList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_ui->extensionComboBox->setCompleter(completer);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::extension_doubleClicked(QTreeWidgetItem * item, int column)
{
    qDebug() << "Text: " << item->text(column) << " Column: " << column;
    QString extension = item->text(column);
    if (!extension.startsWith("GL_")) {
        return;
    }
    loadExtensionSpec(extension);
}

void MainWindow::on_filterText_textChanged(const QString & text)
{
    QMapIterator<QString, QStringList> it(m_extMap);

    //
    // Build extension tree (eventually apply a filter)
    //
    while (it.hasNext()) {
        it.next();
        QString key = it.key();
        QStringList valueList = it.value();
        if (text!="") {
            valueList = valueList.filter(text, Qt::CaseInsensitive);
        }
        if (!valueList.empty()) {
            QTreeWidgetItem * corpItem = new QTreeWidgetItem(m_ui->extensionsTree);
            corpItem->setText(0, key + " ("+QString::number(valueList.count())+")");
            QStringListIterator it(valueList);
            while (it.hasNext()) {
                QTreeWidgetItem * child = new QTreeWidgetItem(corpItem);
                child->setText(0, it.next());
            }
        }
    }
    if(text!="") {
        m_ui->extensionsTree->expandAll();
    }
}

void MainWindow::loadExtensionSpec(const QString & extension)
{
    // Parse extension name 
    QRegExp regex = QRegExp("^([A-Z]+)_([A-Z_]*[A-Z])_([\\w_]+)$");

    if (regex.indexIn(extension) == -1) {
        m_ui->extensionSpecView->setHtml("Invalid extension name", QUrl(""));
        return;
    }

    QString corp = regex.cap(2);
    QString base = regex.cap(3);
    QString url = "http://www.opengl.org/registry/specs/"+corp+"/"+base+".txt";
    qDebug() << "Spec URL: " << url;

    // Load specification site
    m_ui->extensionSpecView->load(url);
    // Find and select the item in the extension combo box
    int idx = m_ui->extensionComboBox->findText(extension);
    if (idx != -1) {
        m_ui->extensionComboBox->setCurrentIndex(idx);
    }
}

void MainWindow::loadSpecStarted()
{
    QString url = m_ui->extensionSpecView->url().toString();
    m_ui->statusBar->showMessage(tr("Start loading: ") + url);
}

void MainWindow::loadSpecProgress(int progress)
{
    QString url = m_ui->extensionSpecView->url().toString();
    m_ui->statusBar->showMessage(tr("Loading: ") + url + " " + QString::number(progress) + "%");
}

void MainWindow::loadSpecFinished(bool ok)
{
    QString url = m_ui->extensionSpecView->url().toString();

    if (!ok) {
        m_ui->statusBar->showMessage(tr("Failed Loading: ") + url);
        return;
    }
    m_ui->statusBar->showMessage(tr("Loaded: ") + url);

    m_ui->extensionTabs->setCurrentWidget(m_ui->extensionSpecTab);
}

