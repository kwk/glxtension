/*
glxtension - Show OpenGL information
Copyright (C) 2013 Konrad Kleine

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QDebug>
#include <QCompleter>
#include <QtNetwork/QNetworkReply>
#include "main_window.h"
#include "ui_main_window.h"
#include "gl_helper_widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    fUi(new Ui::MainWindow),
    fScannedRegSections(kNone),
    fCurExtension(),
    fCurUrl()
{
    fUi->setupUi(this);
    GLHelperWidget * helperWidget = new GLHelperWidget(fUi->glWidget);

    fUi->vendorValue->setText(helperWidget->getString(GL_VENDOR));
    fUi->rendererValue->setText(helperWidget->getString(GL_RENDERER));
    fUi->versionValue->setText(helperWidget->getString(GL_VERSION));
    fUi->glslVersionValue->setText(helperWidget->getString(GL_SHADING_LANGUAGE_VERSION));

    // The Registry is broken down into separate sections for each API.
    // We try to hide these sections from the user as best as possible.
    // (see http://www.khronos.org/registry/).
    fSpecBaseUrls[kOpenGL] = "http://www.opengl.org/registry/specs/%1/%2.txt";
    fSpecBaseUrls[kOpenGL_ES] = "http://www.khronos.org/registry/gles/extensions/%1/%1_%2.txt";
    fSpecBaseUrls[kOpenGL_SC] = "http://www.khronos.org/registry/glsc/extensions/%1/%1_%2.txt";
    fSpecBaseUrls[kUnknown] = "http://www.opengl.org/registry/specs/%1/%3_%2.txt";

    // Collect extension info and put them into a map with key: corp and value: extension

    QString exts = helperWidget->getString(GL_EXTENSIONS);
    QStringList extList = exts.split(" ", QString::SkipEmptyParts);
    QStringListIterator itList(extList);
    while (itList.hasNext()) {
        QString ext = itList.next();
        QString corp = ext.split("_").at(1);
        fExtMap[corp].append(ext);
        //m_extensionList.append(ext.replace(0, corp.length()+4, ""));
        fExtensionList.append(ext);
    }
    fExtensionList.sort();

    // display the extension tree by faking a filter signal
    on_filterText_textChanged("");

    // What to do when an extension was double clicked.
    connect(fUi->extensionsTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(extension_doubleClicked(QTreeWidgetItem*,int)));

    // When a specification page starts loading, is loading, and has been loaded...
    connect(fUi->extensionSpecView, SIGNAL(loadStarted()), this, SLOT(loadSpecStarted()));
    connect(fUi->extensionSpecView, SIGNAL(loadProgress(int)), this, SLOT(loadSpecProgress(int)));
    connect(fUi->extensionSpecView, SIGNAL(loadFinished(bool)), this, SLOT(loadSpecFinished(bool)));

    // Monitor the underlying web page of the QWebView object to detect
    // if an extension could be correctly retrieved. We need to check for
    // anything other than 200 (e.g. 404 not found).
    connect(fUi->extensionSpecView->page()->networkAccessManager(),
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(gotNetworkReply(QNetworkReply*)));

    // Add extensions to combobox
    fUi->extensionComboBox->addItems(fExtensionList);
    connect(fUi->extensionComboBox, SIGNAL(activated(QString)), this, SLOT(loadExtensionSpec(QString)));

    // Autocompleter for filtering extensions
    QCompleter *completer = new QCompleter(fExtensionList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    fUi->extensionComboBox->setCompleter(completer);
}

MainWindow::~MainWindow()
{
    delete fUi;
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
    QMapIterator<QString, QStringList> it(fExtMap);

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
            QTreeWidgetItem * corpItem = new QTreeWidgetItem(fUi->extensionsTree);
            corpItem->setText(0, key + " ("+QString::number(valueList.count())+")");
            QStringListIterator it(valueList);
            while (it.hasNext()) {
                QTreeWidgetItem * child = new QTreeWidgetItem(corpItem);
                child->setText(0, it.next());
            }
        }
    }
    if(text!="") {
        fUi->extensionsTree->expandAll();
    }
}

void MainWindow::loadExtensionSpec(const QString & extension, kRegistrySection section)
{
    // If we haven't seen the extension coming in before, we reset the lists
    // of registry sections we potentially have to visit.
    if (fCurExtension != extension) {
        fScannedRegSections = kNone;
    }
    fCurExtension = extension;
    fScannedRegSections |= section;

    QStringList splitList = extension.split("_");
    if (splitList.size() < 2) {
        fUi->extensionSpecView->setHtml(tr("Please, specify the extension name in full format (e.g. \"GL_ARB_extension_name\")."), QUrl(""));
    }
    QString corp = splitList.at(1);
    QString base = extension;
    base = base.replace(0, corp.length()+4, ""); // "GL_" (3 chars) ... "_" (1 char)
    fCurUrl = fSpecBaseUrls[section].arg(corp, base, corp.toLower(), base.toLower(), corp.toUpper(), base.toUpper());
    qDebug() << "Spec URL: " << fCurUrl;
    fUi->extensionSpecView->load(fCurUrl);
    // Find and select the item in the extension combo box
    int idx = fUi->extensionComboBox->findText(extension);
    if (idx != -1) {
        fUi->extensionComboBox->setCurrentIndex(idx);
    }
}

void MainWindow::loadSpecStarted()
{
    QString url = fUi->extensionSpecView->url().toString();
    fUi->statusBar->showMessage(tr("Start loading: %1").arg(url));
}

void MainWindow::loadSpecProgress(int progress)
{
    QString url = fUi->extensionSpecView->url().toString();
    fUi->statusBar->showMessage(tr("Loading: %1 %2%").arg(url, QString::number(progress)));
}

void MainWindow::loadSpecFinished(bool ok)
{
    QString url = fUi->extensionSpecView->url().toString();

    if (!ok) {
        fUi->statusBar->showMessage(tr("Failed Loading: %1").arg(url));
        return;
    }
    fUi->statusBar->showMessage(tr("Loaded: %1").arg(url));

    fUi->extensionTabs->setCurrentWidget(fUi->extensionSpecTab);
}

void MainWindow::gotNetworkReply(QNetworkReply *reply)
{
    QString url = reply->url().toString();

    // Skip HTTP status analysis for things than the current URL (e.g. images, css, js)
    if (url != fCurUrl) {
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString reason(reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray());
        qDebug() << "Failed to query URL " << url << " (HTTP status code: " << statusCode << ", Reason: " << reason << ")";

        // Test which remaining registry section we haven't checked yet
        if ((fScannedRegSections & kOpenGL) == 0) {
            loadExtensionSpec(fCurExtension, kOpenGL);
        } else if ((fScannedRegSections & kOpenGL_ES) == 0) {
            loadExtensionSpec(fCurExtension, kOpenGL_ES);
        } else if ((fScannedRegSections & kOpenGL_SC) == 0) {
            loadExtensionSpec(fCurExtension, kOpenGL_SC);
        } else if ((fScannedRegSections & kUnknown) == 0) {
            loadExtensionSpec(fCurExtension, kUnknown);
        } else {
            fUi->statusBar->showMessage(tr("Unable to find extension \"%1\" in any registry section.").arg(fCurExtension));
        }
    } else {
        qDebug() << "Fetched URL " << url;
        // If everything worked fine and we got the extension spec loaded,
        // we reset the application's memory on where to look for the extension.
        fScannedRegSections = kNone;
    }
}
