#ifndef LOADSETTING_H
#define LOADSETTING_H

#include <QWidget>
#include <QDebug>
namespace Ui {
class loadSetting;

}
class MainWindow;

class loadSetting : public QWidget
{
    Q_OBJECT

public:
    explicit loadSetting(QWidget *parent = 0);
    ~loadSetting();

public:
    void vuleToSetting(void);
    void closeEvent(QCloseEvent *);

private slots:
    void on_startComboBox_currentIndexChanged(int index);

    void on_okPushButton_clicked();



    void on_cancelPushButton_clicked();
public:
    int defauldataStart;
    int defaulupLimitX ;
    int defaulnumberX;
    int defaulphase;
    int defauldownLimitX;
    int defaulheadStart;
    int defaulheadStop;
signals:
    void ExitWin();
private:
    Ui::loadSetting *ui;
};

#endif // LOADSETTING_H
