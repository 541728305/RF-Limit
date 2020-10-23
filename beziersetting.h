#ifndef BEZIERSETTING_H
#define BEZIERSETTING_H

#include <QWidget>
#include <QProgressBar>
namespace Ui {
class BezierSetting;
}

class BezierSetting : public QWidget
{
    Q_OBJECT

public:
    explicit BezierSetting(QWidget *parent = nullptr);
    ~BezierSetting();
//    QProgressBar *progressBar;
    void closeEvent (QCloseEvent *event);

private slots:
    void on_pushButton_clicked();
    void progressUpdate(int total,int progress);
    void calculateBeginText();
    void calculateOverText();


signals:
    void buzierSettingClose();
    void buzierSettingStart();

private:
    Ui::BezierSetting *ui;
};

#endif // BEZIERSETTING_H
