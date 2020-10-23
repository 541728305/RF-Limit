#include "saveform.h"
#include "ui_saveform.h"

SaveForm::SaveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveForm)
{
    ui->setupUi(this);
}

SaveForm::~SaveForm()
{
    delete ui;
}

void SaveForm::on_saveLimit_clicked()
{
    emit savelimit();
}

void SaveForm::on_saveData_clicked()
{
    emit savedata();
}
