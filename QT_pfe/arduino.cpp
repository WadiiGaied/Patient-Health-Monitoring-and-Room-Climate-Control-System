#include "arduino.h"
#include<QDebug>
#include<QSerialPortInfo>
#include<QVector>


Arduino::Arduino():data(""), arduino_port_name(""),
    arduino_is_available(false),serial(new QSerialPort)
{}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
   return serial;
}

Arduino::~Arduino()
{
   close_arduino();
   delete serial;
}

int Arduino::close_arduino()
{
    if(serial->isOpen())
    {
        serial->close();
        return 0;
    }
    return 1;
}

/*QByteArray Arduino::read_from_arduino()
{   serialdata = "";
    data="";


    if(serial->isReadable())
    {
        data = serial->readAll(); // Lire toutes les données disponibles depuis le port série
        qDebug() << "data0:" << data;
         //i++
    }
    return data;
}*/
int* Arduino::read_from_arduino()
{
    serialdata = "";
    data = "";
    QList<int> values;
    int* array = new int[5];

    if (serial->isReadable())
    {
        data = serial->readAll(); // Lire toutes les données disponibles depuis le port série
        qDebug() << "data0:" << data;

        for (const QChar &ch : data) {
            values.append(ch.unicode()); // Ajoute la valeur Unicode de chaque caractère à la liste
        }

        for (int i = 0; i < 5; ++i) {
            if (i < values.size())
                array[i] = values[i];
            else
                array[i] = 0; // Remplit le tableau avec des zéros si les données sont insuffisantes
        }

        // Affiche chaque valeur dans le tableau
        for (int i = 0; i < 5; ++i) {
          qDebug() << "Value[" << i << "]:" << array[i];
        }
    }


return array ;
}
void Arduino::write_to_arduino( QByteArray d)
{
    if(serial->isWritable())
    {
        serial->write(d);  // envoyer des donnés vers Arduino
    }
    else
    {
        qDebug() << "Couldn't write to serial!";
    }
}

void Arduino::write_to_arduino_test(int value1 , int value2)
{
    if (serial->isWritable())
    {
        QByteArray byteArray;
        byteArray.append(static_cast<unsigned char>(value1));
        byteArray.append(static_cast<unsigned char>(value2));
        serial->write(byteArray);  // envoyer la valeur convertie en QByteArray
    }
    else
    {
        qDebug() << "Couldn't write to serial!";
    }
}


int Arduino::connect_arduino()
{   // recherche du port sur lequel la carte arduino identifée par  arduino_uno_vendor_id
    // est connectée
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts())
    {
        if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier())
        {
            qDebug() << "serial_port_info.vendorIdentifier() = " << serial_port_info.vendorIdentifier();
            qDebug() << "serial_port_info.productIdentifier() = " << serial_port_info.productIdentifier();

            if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
               serial_port_info.productIdentifier()== arduino_uno_producy_id)
            {
                arduino_is_available = true;
                arduino_port_name=serial_port_info.portName();
            }
        }
    }
    qDebug() << "arduino_port_name is :" << arduino_port_name;
    if(arduino_is_available)
    { // configuration de la communication ( débit...)
        serial->setPortName(arduino_port_name);
        if(serial->open(QSerialPort::ReadWrite))
        {
            serial->setBaudRate(QSerialPort::Baud115200); // débit : 115200 bits/s
            serial->setDataBits(QSerialPort::Data8); //Longueur des données : 8 bits,
            serial->setParity(QSerialPort::NoParity); //1 bit de parité optionnel
            serial->setStopBits(QSerialPort::OneStop); //Nombre de bits de stop : 1
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;
        }
        return 1;
    }
    return -1;
}
