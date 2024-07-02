#ifndef MACLASSE_H
#define MACLASSE_H

#include <QMainWindow>
#include <mainwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MaClasse; }
QT_END_NAMESPACE

class MaClasse : public QMainWindow
{
    Q_OBJECT

public:
    MaClasse(QWidget *parent = nullptr);
    ~MaClasse();
void test();
private slots:
    void on_button_login_clicked();


private:
    Ui::MaClasse *ui;
    MainWindow *mainwindow;

};
#endif // MACLASSE_H
