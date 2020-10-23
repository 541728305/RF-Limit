#include "waitting.h"
#include "ui_waitting.h"

Waitting::Waitting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Waitting)
{
    ui->setupUi(this);
}

Waitting::~Waitting()
{
    delete ui;
}
void Waitting::progressUpdate(int total,int progress)
{
    ui->progressBar->setRange(0,total);
    ui->progressBar->setValue(progress);
}
