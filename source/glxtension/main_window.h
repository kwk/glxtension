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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QStringList>

class QTreeWidgetItem;
class QNetworkReply;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum kRegistrySection {
        kNone       = 0,
        kCOLLADA    = 1 << 0,
        kEGL        = 1 << 1,
        kOpenCL     = 1 << 2,
        kOpenGL     = 1 << 3,
        kOpenGL_ES  = 1 << 4,
        kOpenGL_SC  = 1 << 5,
        kOpenKODE   = 1 << 6,
        kOpenMAX_AL = 1 << 7,
        kOpenMAX_IL = 1 << 8,
        kOpenSL_ES  = 1 << 9,
        kOpenVG     = 1 << 10,
        kOpenWF     = 1 << 11,
        kWebGL      = 1 << 12,
        kUnknown    = 1 << 13,
        kAll        = 0xFFFF
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public slots:
    void extension_doubleClicked(QTreeWidgetItem * item, int column);

protected slots:
    void on_filterText_textChanged(const QString & text);
    void loadExtensionSpec(const QString & extension, kRegistrySection section=kOpenGL);
    // Slots for dealing with loading extension specifications from the web:
    void loadSpecStarted();
    void loadSpecProgress(int progress);
    void loadSpecFinished(bool ok);
    void gotNetworkReply(QNetworkReply * reply);

private:
    Ui::MainWindow *fUi;
    QMap<QString, QStringList> fExtMap;
    QStringList fExtensionList;
    /**
     * For each registry section we specify a URL string that can take six arguments:
     * %1 The organization (e.g. NV)
     * %2 The base string (vertex_half_float)
     * %3 Same as %1 but in lower case letters
     * %4 Same as %2 but in lower case letters
     * %5 Same as %1 but in UPPER case letters
     * %6 Same as %2 but in UPPER case letters
     */
    QMap<kRegistrySection, QString> fSpecBaseUrls;
    int fScannedRegSections;
    /**
     * The currently selected extension, e.g. GL_ARB_vertex_buffer
     */
    QString fCurExtension;
    /**
     * The URL that is currently being processed to find the extension online.
     */
    QString fCurUrl;
};

#endif // MAIN_WINDOW_H
