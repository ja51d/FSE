#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <QUdpSocket>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct CanSatdata {

    int packetnumber=0;
    double cansatstatus=18.2;
    double errorcode=98.1;
    double sendtime=23.1;
    double basinc1=13.2;
    double basinc2=1.9;
    double height1=2.56;
    double height2= 23.1;
    double irtifafarki= 85.2;
    double landingspeed=34.4;
    double temperature=43.1;
    double voltage=78.3;
    double gps1latitude=34.9;
    double gps1longitude=234.2;
    double gps1altitude=34.2;
    double pitch=23.1;
    double roll=9.43;
    double yaw=56.4;
    int teamno=22490;




};


class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:


    void readData();
    void writeData();


    void timerFunc();


    void on_startb_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    CanSatdata datas;

    QTimer *timer;
    QUdpSocket *socket;
    QUdpSocket *socket2;

};
#endif // MAINWINDOW_H
