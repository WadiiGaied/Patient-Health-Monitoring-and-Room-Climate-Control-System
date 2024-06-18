#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <arduino.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int PAS = 2 ;
    int intensite = 0  ;
    int ret ;


private slots:
    void showtime();
    void test_port();

    void on_button_chambre1_clicked();

    void on_button_chambre2_clicked();

    void on_button_Accueil_clicked();

    void update_label();

    void on_button_cloud_clicked();

    void on_butto_ventilateur_clicked();

    void on_button_sortie_clicked();

    void on_buttonplus_clicked();

    void on_buttonplus_2_clicked();

private:
    Ui::MainWindow *ui;
    Arduino A ;



};




#endif // MAINWINDOW_H
