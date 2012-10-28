#include <QDebug>
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

    //
    // Collect extension info and put 'em into a map
    //
    QString exts = helperWidget->getString(GL_EXTENSIONS);
    QStringList extList = exts.split(" ", QString::SkipEmptyParts);

    QStringListIterator itList(extList);

    while (itList.hasNext())
    {
        QString ext = itList.next();
        QString corp = ext.split("_").at(1);

        m_extMap[corp].append(ext);
    }

    // display the extension tree by faking a signal
    on_filterText_textChanged("");
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::on_filterText_textChanged(const QString & text)
{
    QMapIterator<QString, QStringList> itMap(m_extMap);

    //
    // Build extension tree (eventually apply a filter)
    //
    while (itMap.hasNext()) {
        itMap.next();
        QString key = itMap.key();
        QStringList valueList = itMap.value();

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
