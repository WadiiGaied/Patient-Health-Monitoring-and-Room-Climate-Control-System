#ifndef ARDUINO_H
#define ARDUINO_H

#include<QByteArray>
#include<QSerialPort>

class Arduino
{
public:     //méthodes de la classe Arduino
    Arduino();
    ~Arduino();
    int connect_arduino(); // permet de connecter le PC à Arduino
    int close_arduino(); // permet de femer la connexion
    void write_to_arduino(QByteArray ); // envoyer des données vers arduino
    void write_to_arduino_test(int,int);
    int* read_from_arduino();  //recevoir des données de la carte Arduino
    QSerialPort* getserial();   // accesseur
    QString getarduino_port_name();
    int i = 0 ;
private:
    static const quint16 arduino_uno_vendor_id = 1659; //6790(arduino)/1659(ttl) ;
    static const quint16 arduino_uno_producy_id = 8963; //29987(arduino)8963(ttl)/;
    QByteArray data;  // contenant les données lues à partir d'Arduino
    QByteArray serialdata;
    QString arduino_port_name;
    bool arduino_is_available;


    QSerialPort * serial; //Cet objet rassemble des informations (vitesse, bits de données, etc.)
    //et des fonctions (envoi, lecture de réception,…) sur ce qu’est une voie série pour Arduino.
};
#endif // ARDUINO_H
