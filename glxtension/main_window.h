#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void on_filterText_textChanged(const QString & text);

private:
    Ui::MainWindow *m_ui;
    QMap<QString, QStringList> m_extMap;
};

#endif // MAIN_WINDOW_H
