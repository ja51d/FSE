#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QUdpSocket>
#include <QFile>


void MainWindow::readData(){

    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size());



    }


}


void MainWindow::writeData(){



    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    out<< datas.packetnumber<<datas.cansatstatus<<datas.errorcode<<datas.sendtime<<datas.basinc1<<datas.basinc2
       <<datas.height1<<datas.height2<<datas.irtifafarki<<datas.landingspeed<<datas.temperature<<datas.voltage
       <<datas.gps1latitude<<datas.gps1longitude << datas.gps1altitude <<datas.pitch<<datas.roll<<datas.yaw<<datas.teamno;

    QHostAddress receiverAddress("127.0.0.1");

    socket2->writeDatagram(data,data.size(),receiverAddress, 5005);
}





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL (timeout()),this,SLOT(timerFunc()));


    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::Any, 5005);

    socket2 = new QUdpSocket(this);

    socket2->bind(QHostAddress::Any, 5005);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::timerFunc(){



    writeData();

    datas.packetnumber+=1;
    datas.basinc1+=1;
    datas.basinc2+=1;
    datas.height1+=1;
    datas.height2+=1;
    datas.irtifafarki+=2.1;
    datas.landingspeed-= 0.5;
    datas.temperature+= 0.3;
    datas.gps1altitude+=1;
    datas.gps1latitude+=1;
    datas.gps1longitude+=1;
    datas.pitch+=1;
    datas.roll+=1;
    datas.yaw+=1;


}

void MainWindow::on_startb_toggled(bool checked)
{




            if(checked==true){

                ui->startb->setText("Stop");

                timer->start(1000);




            }

            else{


                ui->startb->setText("Start");

                timer->stop();

            }
}
