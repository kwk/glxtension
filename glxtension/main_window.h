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
