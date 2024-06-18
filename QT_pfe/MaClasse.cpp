#include "MaClasse.h"
#include "ui_MaClasse.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
MaClasse::MaClasse(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MaClasse)
{
    ui->setupUi(this);
}

MaClasse::~MaClasse()
{
    delete ui;
}

void MaClasse::test()
{
    qDebug() << "wadii" ;
}
void MaClasse::on_button_login_clicked()
{
    QString username = ui->text_username->text();
    QString password = ui->text_password->text();
    if(username=="csf" && password=="c++qt" )
    {
        hide();
        mainwindow = new MainWindow(this);
        mainwindow->show();
    }
    else
    {
        QMessageBox::warning(this,"Login","UserName and Password is Not  correct");

    }
}

