#ifndef LIMITSETTING_H
#define LIMITSETTING_H

#include <QWidget>

namespace Ui {
class limitSetting;
}

class limitSetting : public QWidget
{
    Q_OBJECT

public:
    explicit limitSetting(QWidget *parent = 0);
    ~limitSetting();

private:
    Ui::limitSetting *ui;

public:
    int defaultMethods;
    float defaultUpOffset;
    float defaultDownOffset;
    int defaultSigma;
    int defaultCreat;


private slots:
    void on_savePushButton_clicked();
    void on_exitPushButton_clicked();
};

#endif // LIMITSETTING_H
