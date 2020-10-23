#ifndef WAITTING_H
#define WAITTING_H

#include <QWidget>

namespace Ui {
class Waitting;
}

class Waitting : public QWidget
{
    Q_OBJECT

public:
    explicit Waitting(QWidget *parent = nullptr);
    ~Waitting();
private slots:
    void progressUpdate(int total,int progress);
private:
    Ui::Waitting *ui;
signals:

};

#endif // WAITTING_H
