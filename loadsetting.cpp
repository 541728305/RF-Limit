#include "loadsetting.h"
#include "ui_loadsetting.h"
#include "mainwindow.h"
#include <QDebug>




extern int dataStart;
extern int upLimitX ;
extern int downLimitX;
extern int numberX;
extern int phase;
extern int PHASE;
extern int comboFlag;
extern int headStart;
extern int headStop;
loadSetting::loadSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loadSetting)
{
    ui->setupUi(this);
    dataStart=6;
    upLimitX=2;
    downLimitX=upLimitX+1;
    numberX=5;
    phase=0;
    PHASE=2;
    comboFlag=0;
    defauldataStart=dataStart;
    defaulupLimitX=upLimitX;
    defauldownLimitX = downLimitX;
    defaulnumberX=numberX;
    defaulphase = phase;
    defaulheadStart=headStart;
    defaulheadStop=headStop;
    ui->limitStartLineEdit->setText(QString::number(defaulupLimitX+1));
    ui->dataStartLineEdit->setText(QString::number(defaulnumberX+1));
    ui->startComboBox->setCurrentIndex(defauldataStart);
    ui->comboBox->setCurrentIndex(defaulphase);

    ui->headStartLineEdit->setText(QString::number(defaulheadStart));
    ui->headStopLineEdit->setText(QString::number(defaulheadStop));


}

loadSetting::~loadSetting()
{
    delete ui;
}

void loadSetting::on_startComboBox_currentIndexChanged(int index)
{

    dataStart=index;
}


void loadSetting::vuleToSetting(void)
{
    ui->startComboBox->setCurrentIndex(dataStart);
}


void loadSetting::closeEvent(QCloseEvent *)
 {
     emit ExitWin();
 }
void loadSetting::on_okPushButton_clicked()
{


    dataStart=ui->startComboBox->currentIndex();
    upLimitX=ui->limitStartLineEdit->text().toInt()-1;
    downLimitX=upLimitX+1;
    numberX = ui->dataStartLineEdit->text().toInt()-1;
    phase=ui->comboBox->currentIndex();
    headStart=ui->headStartLineEdit->text().toInt();
    headStop=ui->headStopLineEdit->text().toInt();
    defauldataStart=dataStart;
    defaulupLimitX=upLimitX;
    defauldownLimitX = downLimitX;
    defaulnumberX=numberX;
    defaulphase = phase;
    defaulheadStart=headStart;
    defaulheadStop=headStop;
    if(phase==0)
    {
        PHASE=2;
    }
    else
    {
        PHASE=1;
    }

    comboFlag=1;
    this->close();
}

void loadSetting::on_cancelPushButton_clicked()
{
    ui->limitStartLineEdit->setText(QString::number(defaulupLimitX+1));
    ui->dataStartLineEdit->setText(QString::number(defaulnumberX+1));
    ui->startComboBox->setCurrentIndex(defauldataStart);
    ui->comboBox->setCurrentIndex(defaulphase);
    this->close();
}
