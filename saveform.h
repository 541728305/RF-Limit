#ifndef SAVEFORM_H
#define SAVEFORM_H

#include <QWidget>

namespace Ui {
class SaveForm;
}

class SaveForm : public QWidget
{
    Q_OBJECT

public:
    explicit SaveForm(QWidget *parent = nullptr);
    ~SaveForm();

private slots:
    void on_saveLimit_clicked();

    void on_saveData_clicked();

private:
    Ui::SaveForm *ui;

signals:
    void savelimit();
    void savedata();
};

#endif // SAVEFORM_H
