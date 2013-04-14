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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void extension_doubleClicked(QTreeWidgetItem * item, int column);

protected slots:
    void on_filterText_textChanged(const QString & text);
    void loadExtensionSpec(const QString & extension);
    // Slots for dealing with loading extension specifications from the web:
    void loadSpecStarted();
    void loadSpecProgress(int progress);
    void loadSpecFinished(bool ok);

private:
    Ui::MainWindow *m_ui;
    QMap<QString, QStringList> m_extMap;
    QStringList m_extensionList;
};

#endif // MAIN_WINDOW_H
