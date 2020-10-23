#include "limitsetting.h"
#include "ui_limitsetting.h"

int methods;
float upOffset;
float downOffset;
int nSigma;
int creat;
limitSetting::limitSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::limitSetting)
{
    ui->setupUi(this);
     defaultMethods = methods = 0;
     defaultUpOffset = upOffset = 0.25;
     defaultDownOffset = downOffset = 0.25;
     defaultSigma = nSigma = 3;
     defaultCreat = creat = 0;

     switch (defaultMethods) {
     case 0:
         ui->sigmaRadioButton->setChecked(true);
         ui->averageRadioButton->setChecked(false);
         ui->maxRadioButton->setChecked(false);
         break;
     case 1:
         ui->sigmaRadioButton->setChecked(false);
         ui->averageRadioButton->setChecked(true);
         ui->maxRadioButton->setChecked(false);
         break;
     case 2:
         ui->sigmaRadioButton->setChecked(false);
         ui->averageRadioButton->setChecked(false);
         ui->maxRadioButton->setChecked(true);
         break;
     default:
         break;
     }

     ui->upLineEdit->setText(QString::number(defaultUpOffset));
     ui->downLineEdit->setText(QString::number(defaultDownOffset));
     ui->sigmaLineEdit->setText(QString::number(defaultSigma));
    ui->comboBox->setCurrentIndex(creat);
}

limitSetting::~limitSetting()
{
    delete ui;
}



void limitSetting::on_savePushButton_clicked()
{
    if(ui->sigmaRadioButton->isChecked())
    {
        methods=0;
    }
    else if(ui->averageRadioButton->isChecked())
    {
        methods=1;
    }
    else if(ui->maxRadioButton->isChecked())
    {
        methods=2;
    }

    upOffset = ui->upLineEdit->text().toFloat();
    downOffset = ui->downLineEdit->text().toFloat();
    nSigma = ui->sigmaLineEdit->text().toInt();
    creat = ui->comboBox->currentIndex();

     defaultMethods=methods;
     defaultUpOffset=upOffset;
     defaultDownOffset=downOffset;
     defaultSigma=nSigma;
     defaultCreat=creat;
     this->close();
}

void limitSetting::on_exitPushButton_clicked()
{
    switch (defaultMethods) {
    case 0:
        ui->sigmaRadioButton->setChecked(true);
        ui->averageRadioButton->setChecked(false);
        ui->maxRadioButton->setChecked(false);
        break;
    case 1:
        ui->sigmaRadioButton->setChecked(false);
        ui->averageRadioButton->setChecked(true);
        ui->maxRadioButton->setChecked(false);
        break;
    case 2:
        ui->sigmaRadioButton->setChecked(false);
        ui->averageRadioButton->setChecked(false);
        ui->maxRadioButton->setChecked(true);
        break;
    default:
        break;
    }

    ui->upLineEdit->setText(QString::number(defaultUpOffset));
    ui->downLineEdit->setText(QString::number(defaultDownOffset));
    ui->sigmaLineEdit->setText(QString::number(defaultSigma));
   ui->comboBox->setCurrentIndex(creat);
    this->close();
}
