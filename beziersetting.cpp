#include "beziersetting.h"
#include "ui_beziersetting.h"
#include <QDebug>
#include <QMessageBox>
extern int getBezierPoint;
extern int bezierDoNumber;
BezierSetting::BezierSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BezierSetting)
{
    ui->setupUi(this);
    ui->lineEdit->setText(QString::number(getBezierPoint));
//    progressBar = new QProgressBar(this);
    ui->progressBar->setOrientation(Qt::Horizontal);
//    ui->progressBar->setTextVisible(false);
//    ui->gridLayout_2->addWidget(progressBar);
    ui->progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    ui->comboBox->setCurrentIndex(bezierDoNumber);

}

BezierSetting::~BezierSetting()
{
    delete ui;
}

void BezierSetting::on_pushButton_clicked()
{

    getBezierPoint=ui->lineEdit->text().toInt();
    if(getBezierPoint==0)
    {
        getBezierPoint=1;
        QMessageBox::information(this,tr("警告"),tr("最小值为1"),tr("确定"));
        ui->lineEdit->setText(QString::number(getBezierPoint));
        return ;
    }
    if(getBezierPoint>25)
    {
        getBezierPoint=25;
        QMessageBox::information(this,tr("警告"),tr("最小值为1"),tr("确定"));
        ui->lineEdit->setText(QString::number(getBezierPoint));
        return ;
    }
    bezierDoNumber=ui->comboBox->currentIndex();
    emit buzierSettingStart();
}

void BezierSetting::progressUpdate(int total,int progress)
{
    qDebug()<<"更新进度条"<<total<<progress;
    ui->progressBar->setRange(0,total);
    ui->progressBar->setValue(progress);
}
void BezierSetting::calculateBeginText()
{

    ui->pushButton->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->pushButton->setText("正在计算");
}
void BezierSetting::calculateOverText()
{
    ui->pushButton->setText("开始平滑");
    ui->pushButton->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->comboBox->setEnabled(true);

}
#include <QCloseEvent>
void BezierSetting::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "提示","你确定退出吗？",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    if (resBtn != QMessageBox::Ok) {
        event->ignore();
    } else {
        emit buzierSettingClose();
        event->accept();
    }
}
