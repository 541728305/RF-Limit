#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QDebug>
class my_Thread:public QObject
{
    Q_OBJECT
public:
    my_Thread();
    ~ my_Thread();


public slots:
    void callout()
    {
        qDebug()<<"thead"<<QThread::currentThreadId();
    }



};

#endif // MY_THREAD_H
