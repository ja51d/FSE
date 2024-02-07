#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QUdpSocket>
#include <QFile>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QFileDialog>





void MainWindow::writeData(){



    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    out<< datas.packetnumber<<datas.cansatstatus<<datas.errorcode<<datas.sendtime<<datas.basinc1<<datas.basinc2
        <<datas.height1<<datas.height2<<datas.irtifafarki<<datas.landingspeed<<datas.temperature<<datas.voltage
        <<datas.gps1latitude<<datas.gps1longitude << datas.gps1altitude <<datas.pitch<<datas.roll<<datas.yaw<<datas.teamno;

    QHostAddress receiverAddress("127.0.0.1");

    socket2->writeDatagram(data,data.size(),receiverAddress, 5006);
}



void MainWindow::readData(){
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size());




        QDataStream stream(datagram);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream >> numba;

        qDebug()<<numba;
    }
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)


{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL (timeout()),this,SLOT(timerFunc()));


    mCamera = new QCamera(this);
        mCameraImageCapture = new QCameraImageCapture(mCamera, this);
    mCameraViewfinder = new QCameraViewfinder(this);

    mLayout = new QVBoxLayout;
    mOptionsMenu = new QMenu("Options", this);
    mTurnonAction = new QAction("Turn on", this);
     mTurnoffAction = new QAction("Turn off", this);
      mCaptureAction = new QAction("Capture", this);

     mOptionsMenu->addActions({ mTurnoffAction, mTurnonAction, mCaptureAction });
      ui->optionsPushButton->setMenu(mOptionsMenu);
     mCamera->setViewfinder(mCameraViewfinder);
      mLayout->addWidget(mCameraViewfinder);
     mLayout->setMargin(0);
     ui->scrollArea->setLayout(mLayout);

    connect(mTurnonAction, &QAction::triggered, [&]() {
         mCamera->start();
     });

     connect(mTurnoffAction, &QAction::triggered, [&]() {
         mCamera->stop();
     });

    connect(mCaptureAction, &QAction::triggered, [&]() {
         auto filename = QFileDialog::getSaveFileName(this, "Capture", "/",
                                      "Image (*.jpg;*.jpeg)");
         if (filename.isEmpty()) {
             return;
         }

         mCameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);

         QImageEncoderSettings imageEncoderSettings;
         imageEncoderSettings.setCodec("image/jpeg");
          imageEncoderSettings.setResolution(1600, 1200);
         mCameraImageCapture->setEncodingSettings(imageEncoderSettings);
          mCamera->setCaptureMode(QCamera::CaptureStillImage);
         mCamera->start();
          mCamera->searchAndLock();
         mCameraImageCapture->capture(filename);
          mCamera->unlock();

    });










    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::Any, 5005);

    socket2 = new QUdpSocket(this);

    socket2->bind(QHostAddress::Any, 5006);



}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::timerFunc(){




    readData();
    ui->listWidget->addItem(QString::number(numba));

    writeData();

    datas.packetnumber+=1;
    datas.basinc1+=1;
    datas.basinc2+=1;
    datas.height1+=1;
    datas.height2+=1;
    datas.irtifafarki+=2.2;
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


