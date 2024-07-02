#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MaClasse.h"
#include <QDateTime>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <string>
#include <QUrl>
#include <QDesktopServices>

MaClasse *mac ;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mac->test();

   // int ret = A.connect_arduino();
    //qDebug() << "ret = " << ret ;
    test_port();
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));


    QTimer *timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(showtime()));
    timer->start();
    QDateTime date=QDateTime::currentDateTime();
    QString datetimetext=date.toString("dddd dd MMMM yyyy ");
    ui->date->setText(datetimetext);


}

MainWindow::~MainWindow()
{
    A.close_arduino();
    delete ui;
}

void MainWindow::showtime()
{
    QTime time=QTime::currentTime();
    QString  time_text=time.toString("hh : mm : ss");
    ui->clock->setText(time_text);
}

void MainWindow::on_button_chambre1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_button_chambre2_clicked()
{
     ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_button_Accueil_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_button_cloud_clicked()
{
    QString link="https://thingspeak.com/channels/2563565/private_show";
    QDesktopServices::openUrl(QUrl(link));
}
void MainWindow::test_port()
{
    ret = A.connect_arduino();
    qDebug() << "ret = " << ret;
    if (ret == 0) {
        QPixmap image("C:/Users/WADII/Desktop/IMAGE_PROJET_QT/QT_CAN/logo_connect.png");
        if (!image.isNull()) {
            int w = ui->image_connixon->width();
            int h = ui->image_connixon->height();
            ui->image_connixon->setPixmap(image.scaled(w, h, Qt::KeepAspectRatio));
            ui->text_connixson->setText("Carte est Connectée");
                ui->text_connixson->setStyleSheet("QLabel { color: green; font: 900 10pt 'Segoe UI'; }");
        } else {
            qDebug() << "Failed to load image: C:/Users/WADII/Desktop/csf/Projet_1/logo_connect.png";
        }
    } else {
        QPixmap image("C:/Users/WADII/Desktop/IMAGE_PROJET_QT/QT_CAN/logo_dconnect .png");
        if (!image.isNull()) {
            int w = ui->image_connixon->width();
            int h = ui->image_connixon->height();
            ui->image_connixon->setPixmap(image.scaled(w, h, Qt::KeepAspectRatio));
            ui->text_connixson->setText("Carte Non Connectée");
                ui->text_connixson->setStyleSheet("QLabel { color: red; font: 900 10pt 'Segoe UI'; }");
        } else {
            qDebug() << "Failed to load image: C:/Users/WADII/Desktop/csf/Projet_1/logo_dconnect.png";
        }
    }
}


void MainWindow::update_label()
{

    int* data  = A.read_from_arduino();
    qDebug() << "data[0]"<<data[0];
    qDebug() << "data[1]"<<data[1];
    qDebug() << "data[2]"<<data[2];
    qDebug() << "data[3]"<<data[3];
    qDebug() << "data[3]"<<data[4];



    /*if (data == "A")
    {
        ui->etat_led->setText("ON")  ;
    }
    else if (data == "B")
    {
        ui->etat_led->setText("OFF")  ;
    }*/
    /*if ( A.i == 1){
        temp_hum = data ;
    }
    if (A.i == 2){
        temp = data ;


    }
    if (A.i == 3){
         hum = data ;
         A.i = 0 ;

    }*/
    /*if (A.i == 1 ){
        temp_hum = data ;
        if (temp_hum.isEmpty() || temp_hum.toInt()==0) {
            // Si temp est vide ou égal à zéro, afficher la valeur actuelle de ui->lcd_temp_chambre
            ui->lcd_temp_humaine->display(ui->lcd_temp_humaine->intValue());
        }
        else{
            ui->lcd_temp_humaine->display(temp_hum.toInt());
        }

    }*/


   /* if (A.i == 1){
        temp = data ;
        if (temp.isEmpty() || temp.toInt() == 0) {
            // Si temp est vide ou égal à zéro, afficher la valeur actuelle de ui->lcd_temp_chambre
            ui->lcd_temp_chambre->display(ui->lcd_temp_chambre->intValue());
        }
        else{
            ui->lcd_temp_chambre->display(temp.toInt());
        }
    }
   else if (A.i == 2){
        hum = data ;
        if (hum.isEmpty() || hum.toInt() == 0) {
            // Si temp est vide ou égal à zéro, afficher la valeur actuelle de ui->lcd_temp_chambre
            ui->lcd_humudite->display(ui->lcd_humudite->intValue());
        }
        else{
            ui->lcd_humudite->display(hum.toInt());
        }

    }
   else  if (A.i == 3 ){
        temp_hum = data ;
        if (temp_hum.isEmpty() || temp_hum.toInt() == 0) {
            // Si hum est vide ou égal à zéro, afficher la valeur actuelle de ui->lcd_humidite
            ui->lcd_temp_humaine->display(ui->lcd_temp_humaine->intValue());
        }
        else{
            ui->lcd_temp_humaine->display(temp_hum.toInt());
        }


    }
    if (A.i == 4){
        button = data ;
        if (button.isEmpty() || button.toInt() == 0 ) {
            // Si temp est vide ou égal à zéro, afficher la valeur actuelle de ui->lcd_temp_chambre
            ui->lcd_button->display(ui->lcd_button->intValue());
        }
        else{
            ui->lcd_button->display(button.toInt());
        }
         A.i = 0 ;
    }*/
   /* ui->lcd_button->display(data[0].isEmpty() ? ui->lcd_button->intValue() : data[0].toInt());
    ui->lcd_temp_humaine->display(data[1].isEmpty() ? ui->lcd_temp_humaine->intValue() : data[1].toInt());
    ui->lcd_temp_chambre->display(data[2].isEmpty() ? ui->lcd_temp_chambre->intValue() : data[2].toInt());
    ui->lcd_humudite->display(data[3].isEmpty() ? ui->lcd_humudite->intValue() : data[3].toInt());*/

    ui->lcd_button->display(data[0]);
    ui->lcd_temp_humaine->display(data[1]);
    ui->lcd_pulsation->display(data[2]);
    ui->lcd_temp_chambre->display(data[3]);
    ui->lcd_humudite->display(data[4]);




}







void MainWindow::on_butto_ventilateur_clicked()
{
    if (ui->radioButton_on->isCheckable())
    {
       // A.write_to_arduino("A");
        A.write_to_arduino_test(1,intensite);


    }
    if(ui->radioButton_off->isChecked())
    {
         A.write_to_arduino_test(0,intensite);
    }
}


void MainWindow::on_button_sortie_clicked()
{
    hide();
    mac = new MaClasse (this);
    mac->show();
}


void MainWindow::on_buttonplus_clicked()
{
    intensite += PAS;
    ui->lcd_ventilateur->display(intensite);
    if (intensite > 100){
        intensite = 100;
     ui->lcd_ventilateur->display(intensite);
    }
}


void MainWindow::on_buttonplus_2_clicked()
{
    intensite -= PAS;
    ui->lcd_ventilateur->display(intensite);
    if (intensite < 0){
     intensite = 0;
     ui->lcd_ventilateur->display(intensite);
    }
}

