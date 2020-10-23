#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QSplineSeries>
#include "limitsetting.h"
#include <vector>
#include "loadsetting.h"
#include "multicurvesplot.h"
#include "beziersetting.h"
#include "waitting.h"
#include "saveform.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE


QT_CHARTS_BEGIN_NAMESPACE
class QChart;
class QChartView;


QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


#define LimitSeries 0
#define DataSeries 1
#define AllSeries 2

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;


typedef struct _LimitCharData
{
    QVector<double> keyVec;//x
    QVector<QString> valVec;//y
}LimitCharData;//单个曲线的数据

typedef struct
{
    double X;
    double Y;
} PointF;


class BezierThread : public QObject
{
    Q_OBJECT

public:

    ~BezierThread()
    {
        deleteLater();
    }

    int m_bezier(QVector<QPointF> point,int div);
    void m_draw_bezier_curves(PointF *points, PointF *out_points, int div);
    PointF m_bezier_interpolation_func(double t, PointF *points);

public slots:
    void bezierCalculate(int start, int stop, int interval, int limitPoint);
signals:
    void doBezierCalculate(int start,int stop,int interval,int limitPoint);
    void calculateFish();
    void progressBarUpDate(int total,int progress);
};

class OpenDataThread;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:



    void func();
    void closeEvent( QCloseEvent * event );
    void copyToSource();
    void copyFromDefault();
    int openLimit();

    void createLimitLine();
    int openData();
    int createDataLine();
    void changeDataLine()const;
    int findUpDownLimit();
    void dataToList() const;
    void findMaxMin();
    int nullLineFilter(int x,int y,int nData);
    int newNullLineFilter(int x,int y,int nData);
    int filterData(int x, int y, int level);
    float creatFilterData();
    void filterDataLimit();
    void Variance();

    void creatLimit();
    void maxAndMin();
    void showSeries(int series,int rescale);
    void getStateNum();
    void sortingData();//整理数据，去除数据中的空行
    void RawDataAverageAndVariance();//原始数据的平均值和方差，创建原始数据筛选标准用
    void RawDataFilterCriteria();//创建原始数据筛选标准
    void slotGrabWidgetScreen();
    void goToAve();



public:
    int dataXMax=10;
    int limitXMax=10;
    int yMAX=2;

    SaveForm * mySave;
    Waitting * myWait;
    OpenDataThread *mOpenDataThread;
    QThread* myDataSplit;
    BezierThread *mBezierCalculate;
    QThread *myBezierThread;
    limitSetting *s;
    loadSetting *loadSet;
    BezierSetting *bezieS;
    DataTable m_dataTable;
    QList<QChartView *> m_charts;
    QChart *m_chart;
    QChartView *chartView;
    QLineSeries *upLimitSeries ;
    QLineSeries *downLimitSeries ;
    QPointF point ;
    QByteArray PATH;
    QString path;
    QString limitData[10][20];
    QString defaultData[10][20];
    QString data[10][20];
    QString tmp[10][20];
    QString lastFileName;
    float dataCreatAverange[20000];
    float dataCreatVariance[20000];
    float dataFilter[10][20000];
    float dataAverange[100000];
    float dataVariance[100000];
    float creatDataMax[20000];
    float creatDataMin[20000];

    int limitDataX;
    int dataLineNum;
    int curvesDataNum;



    int totalState = 0;
    int state = 0;
    int cmpCoordY;
    int cmpUpDataY;
    int cmpDownDataY;
    int coordX;
    int adjustCoordX;
    int upDownSate;
    float coordY;
    int seriesLength = 0;
    int offsetPonit;
    int CoodYMax;
    int CoodYMin;
    int CoodXMax;
    int CoodXMin;
    int CoodUpDownMax;
    int CoodUpDownMin;


    int filter=101;

private slots:

    void on_addLimitPushButton_clicked();

    void on_addDataPushButton_clicked();

    void on_savePushButton_clicked();

    void on_exitPushButton_clicked();

    void on_upPushButton_clicked();

    void on_downPushButton_clicked();

    void on_leftPushButton_clicked();

    void on_rightPushButton_clicked();

    void on_stateUpPushButton_clicked();

    void on_stateDownPushButton_clicked();

    void on_pointLineEdit_editingFinished();

    void on_defaultPushButton_clicked();

    void on_displayDataCheckBox_clicked();

    void version_slots();

    void whiteSkin_slots();

    void blueSkin_slots();

    void on_endLineEdit_editingFinished();

    void on_horizontalSlider_valueChanged(int value);

    void on_createSettingPushButton_clicked();

    void on_createLimitPushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void limitSetting_slots();

    void upDateCombo();

    void getCoord();

    void callout();

    void on_saveBPMPushButton_clicked();

    void clearAllData();

    void on_antialiasCheckBox_clicked();

    void on_bezierPushButton_clicked();

    void updateBezierToLimit(int bezierSave);

    void on_goToAvePushButton_clicked();

    void on_pushButton_clicked();

    void when_Bezier_Start();

    void when_Bezier_Close();

    void when_BezierSet_Close();

    void closeFile();

    int saveLimit();

    int saveData();

    void saveDataOk();

private:

    Ui::MainWindow *ui;
    MultiCurvesPlot *plot;

signals:
    void BezierOpen();
    void BezierClose(int bezierSave);
    void calculateBegin();
    void calculateOver();
    void doDataSplit(QStringList tempOption,int dataLineNum);
    void doDataJoin();


};

class OpenDataThread : public QObject
{
    Q_OBJECT

public:

    ~OpenDataThread()
    {
        deleteLater();
    }



public slots:
    void dataSplit(QStringList tempOption,int dataLineNum);
    void dataJoin();
signals:
    void dataSplitOk();
    void saveOK();
    void openFail();
    void waitProgressBarUpDate(int total,int progress);
};


#endif // MAINWINDOW_H
