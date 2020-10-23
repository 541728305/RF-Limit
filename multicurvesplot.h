/*
QCustomplot的绘图机制：每条曲线都要绘制在一个单独的graph中

*/



#ifndef MULTICURVESPLOT_H
#define MULTICURVESPLOT_H

#include <QObject>
#include "qcustomplot.h"
#include <QMouseEvent>
#include <QMap>
#include "qDebug.h"

typedef struct _CurveData
{
    QVector<double> keyVec;//x
    QVector<double> valVec;//y
}CurveData;//单个曲线的数据




class my_Thread : public QObject
{
    Q_OBJECT
public:

public slots:
    void show(int count);

signals:
   void showDataSeriesSignal(int count);
   void finish();
};




/*
注意：调用任何函数前，必须先已经通过init函数初始化过支持的曲线数目,送入数据源的指针
为什么不直接在构造中
*/
class MultiCurvesPlot : public QCustomPlot
{
    Q_OBJECT
public:
    /*传入的_allCurvesData必须是有size的*/
    MultiCurvesPlot(int _curvesCnt, QWidget *parent = 0);
    ~MultiCurvesPlot(){qDebug() << "delete MultiCurvesPlot";}

    void setCurvesName(QVector<QString> _nameVec);
    void addData(int curveIdx, double x, double y);
    void setAutoScroll(bool enable){autoScroll = enable;}
    void setDiffSolveEnable(bool enable);
    void setColors(QVector<QColor> _colors);
    void setMainPlot(QCustomPlot *_mainPlot);
    void setTracerEnable(bool enable);//是否使能游标
    void setScatterPointEnable(bool enable);//是否使能散点
    const QVector<CurveData> *getAllData(void) const;
    QString getCurveName(int idx);


    void showDataSeries(int start, int stop, int startOffset, int seriesStart, int seriesStop);
    void removeDataSeries();
    void removeLimitSeries();
    QThread *m_thread;
    void setNameText(QString text,int show);
    void updateBezierControl();
public slots:
    void setCurveName(int idx, const QString newName);
    void showCurves(QList<uint16_t> _idxList, int start, int stop, int startOffset, int isLimit);
    void storeAxisScope(bool x, bool y);//保存轴的范围
    void resumeAxisScope(bool x, bool y);//恢复轴的范围

    void calloutSeries1(QCPDataSelection select);
    void calloutSeries2(QCPDataSelection select);

    void showLimitSeries(int start, int stop, int startOffset,int upLimit, int downLimit);

signals:
    void noniusValueChanged(int _curveIdx, QString val);//游标值变化了

    void coordChange();
    void clearFlag();



protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

//    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
//    virtual void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void clearAllData();
    void when_selectionChangedByUser();
    //void when_itemDoubleClick (QCPAbstractItem *item, QMouseEvent *event){qDebug() << item;}
    void when_legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);
    void finish();
    void bezierFunctionOpen();
    void bezierFunctionClose(int bezierSave );

private:
    //QCPGraph* getGraph(int curveIdx);

    MultiCurvesPlot();

    int curvesCnt;//支持的曲线数目    

    QVector<QColor> getColor;//记录所有曲线的颜色
    QVector<QString> getName;  //记录所有曲线的名称（图例）
    QList<uint32_t> idxList;//当前正在显示的曲线的编号
    bool autoScroll;

    QCPItemText *textLabel;//单击时提示信息框
    QCPItemText *nameLabel;
    QCPItemLine *arrow;//提示信息的箭头

    QAction *actClearDatas;//右键菜单:清空历史数据
    QAction *actshowAllGraph;//右键菜单:全显

    /*游标相关组件*/
    QCPItemTracer *tracer;//游标
    QCPGraph *traceGraph;//游标要吸附哪个graph
    QCPItemText *tracerXText;//用于实时显示游标X值
    QCPItemText *tracerYText;//用于实时显示游标Y值
    QCPItemLine *tracerArrow;
    bool tracerEnable;//是否使能游标

    /*两点求差组件*/
    bool diffSolveEnable;//是否使能两点求差
    QCPItemText *diffText;//单击时提示信息框
    QPointF *lastPoint;
    void solveDifference(QPointF newPoint);

    /*记忆轴的范围组件*/
    QCPRange Xscope;//记录X轴的范围
    QCPRange Yscope;//记录Y轴的范围



    my_Thread *threadFunc;


};

#endif // MULTICURVESPLOT_H
