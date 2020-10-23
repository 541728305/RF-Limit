#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QGraphicsView>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "limitsetting.h"
#include <stdlib.h>
#include <numeric>
#include <cmath>
#include "loadsetting.h"
#include <QPixmap>
#include <QPointF>
int dataStart;
int upLimitX;
int numberX;
int downLimitX;
int phase;
int PHASE;
int comboFlag;
int limitFlag=1;
int dataFlag=1;
int creatFlag=1;
int limitDataWidth = 0;
int DataWidth = 0;
int RawDataWidth=0;
int graphSelect = 0;
double graphCoordX=0;
double graphCoordY=0;
int row=0;
int  limitRow=0;
int showHead=0;
int headStart=0;
int headStop=3;
int pointCount = 0;
QVector<QPointF> bezierPoint;
QVector<QPointF> controlPoint;
#define CURVE_CNT 5000 //最多显示几条曲线
#define POINTSTART (state*(PHASE*ui->pointLineEdit->text().toInt())+dataStart);
#define POINTSTOP (state*(PHASE*ui->pointLineEdit->text().toInt())+dataStart+ui->pointLineEdit->text().toInt());
#define POINTNUM ui->pointLineEdit->text().toInt();
QVector<CurveData> limitCurvesData(50);//所有曲线的数据,allCurvesData[n]中又含有两个QVector，也即第n条曲线的x、y值
QVector<CurveData> backupCurvesData(50);//备份曲线数据,backupCurvesData[n]中又含有两个QVector，也即第n条曲线的x、y值
QVector<CurveData> bezierCurvesData(50);//贝塞尔曲线的数据,allCurvesData[n]中又含有两个QVector，也即第n条曲线的x、y值
double filterCurvesData[50][100000];//筛选标准
QVector<CurveData> allCurvesData(5000);//所有曲线的数据,allCurvesData[n]中又含有两个QVector，也即第n条曲线的x、y值
QVector<CurveData> allRawCurvesData(5000);//所有曲线的数据,allCurvesData[n]中又含有两个QVector，也即第n条曲线的x、y值
QVector<CurveData> backupRawCurvesData(5000);//所有曲线的数据,allCurvesData[n]中又含有两个QVector，也即第n条曲线的x、y值
QVector<LimitCharData>limitChar(50);
QVector<LimitCharData>headerChar(5000);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    mySave = new SaveForm;
    myWait = new Waitting;
    mOpenDataThread = new OpenDataThread;
    myDataSplit = new QThread;
    plot = new MultiCurvesPlot(CURVE_CNT, this);
    s = new limitSetting;
    loadSet = new loadSetting;
    bezieS = new BezierSetting;
    mBezierCalculate = new BezierThread;
    myBezierThread = new QThread();

    mBezierCalculate->moveToThread(myBezierThread);
    myBezierThread->start();

    mOpenDataThread->moveToThread(myDataSplit);
    myDataSplit->start();
    ui->setupUi(this);
    limitFlag = 1;
    dataFlag = 1;
    creatFlag = 1;

    dataStart=6;
    upLimitX=2;
    downLimitX=upLimitX+1;
    numberX=5;
    phase=0;
    PHASE=2;
    comboFlag=0;
    connect(ui->actionopen_limit_file,SIGNAL(triggered()),this,SLOT(on_addLimitPushButton_clicked()));
    connect(ui->actionOpen_Data_File,SIGNAL(triggered()),this,SLOT(on_addDataPushButton_clicked()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(on_savePushButton_clicked()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(on_exitPushButton_clicked()));
    connect(ui->version,SIGNAL(triggered()),this,SLOT(version_slots()));
    connect(ui->limitCreatAction,SIGNAL(triggered()),this,SLOT(on_createSettingPushButton_clicked()));
    connect(ui->limitSettingAction,SIGNAL(triggered()),this,SLOT(limitSetting_slots()));
    connect(ui->actionWhite,SIGNAL(triggered()),this,SLOT(whiteSkin_slots()));
    connect(ui->actionBlue,SIGNAL(triggered()),this,SLOT(blueSkin_slots()));
    connect(plot,SIGNAL(coordChange()),this,SLOT(getCoord()));
    connect(plot,SIGNAL(clearFlag()),this,SLOT(clearAllData()));

    //贝塞尔平滑
    connect(this,SIGNAL(BezierClose(int)), this, SLOT( updateBezierToLimit(int)));

    connect(bezieS,SIGNAL(buzierSettingStart()), this, SLOT( when_Bezier_Start()));
    connect(bezieS,SIGNAL(buzierSettingClose()), this, SLOT( when_BezierSet_Close()));

    connect(mBezierCalculate,SIGNAL(doBezierCalculate(int ,int ,int ,int )), mBezierCalculate, SLOT(bezierCalculate(int ,int ,int ,int )));
    connect(mBezierCalculate,SIGNAL(calculateFish()), this, SLOT(when_Bezier_Close()));

    connect(mBezierCalculate,SIGNAL(progressBarUpDate(int,int)), bezieS, SLOT(progressUpdate(int, int)));

    connect(mBezierCalculate,SIGNAL(calculateFish()), bezieS, SLOT(calculateOverText()));
    connect(this,SIGNAL(calculateBegin()), bezieS, SLOT( calculateBeginText()));
    //打开数据
    connect(this,SIGNAL(doDataSplit(QStringList,int )), mOpenDataThread, SLOT( dataSplit(QStringList,int)));
    connect(mOpenDataThread,SIGNAL(dataSplitOk()), this, SLOT(closeFile()));

    connect(mOpenDataThread,SIGNAL(waitProgressBarUpDate(int,int)), myWait, SLOT(progressUpdate(int, int)));
    //Save
    connect(mySave,SIGNAL(savelimit()), this, SLOT(saveLimit()));
    connect(mySave,SIGNAL(savedata()), this, SLOT(saveData()));
    connect(this,SIGNAL(doDataJoin()), mOpenDataThread, SLOT( dataJoin()));
    connect(mOpenDataThread,SIGNAL(saveOK()), this, SLOT(saveDataOk()));

    ui->comboBox->setCurrentIndex(dataStart);
    ui->displayDataCheckBox->setChecked(true);
    ui->limitGridLayout->addWidget(plot);
    plot->legend->setVisible(false);
    this->setMouseTracking(true);
    ui->bezierPushButton->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::ButtonText,QColor(18,124,188));
    ui->bezierPushButton->setPalette(palette);
    if(ui->antialiasCheckBox->isChecked()==true)
    {
        showHead=1;
    }
    else
    {
        showHead=0;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getCoord()
{
    qDebug()<<"coord change";
    ui->startLineEdit->setText(QString::asprintf("%.0f",graphCoordX));
    ui->endLineEdit->setText(QString::asprintf("%.0f",graphCoordX));
    if(graphSelect==1)
    {
        ui->infoLabel->setText("选取上限");
        ui->AdjustLabel->setText("上限");
    }

    if(graphSelect==2)
    {
        ui->infoLabel->setText("选取下限");
        ui->AdjustLabel->setText("下限");
    }

}
void MainWindow::showSeries(int series,int rescale)
{
    int start = POINTSTART;
    int stop = POINTSTOP;
    QList<uint16_t> list;

    qDebug()<<"series"<<series<<__LINE__;
    switch(series)
    {
    case 0:
        if(limitDataWidth)
        {
            qDebug()<<"调用showLimit"<<__LINE__;

            plot->showLimitSeries(start,stop,dataStart,upLimitX,downLimitX);
            qDebug()<<"upLimitX"<<upLimitX;

            if(upLimitX-1>0 && creatFlag==1)
            {


                if(headStop>limitChar[upLimitX-1].valVec.data()->size())
                {
                    headStop = limitChar[upLimitX-1].valVec.data()->size();
                }
                if(headStart>limitChar[upLimitX-1].valVec.data()->size())
                {
                    headStart = 0;
                }

                 int end=headStop;
                 if(headStop>3)
                 {

                     while(headerChar[upLimitX-1].valVec.at(start).at(end)!=' ')
                         end++;
                 }
                plot->setNameText( limitChar[upLimitX-1].valVec.at(start).mid(headStart,end),showHead);
            }
        }
        break;
    case 1:
        if(RawDataWidth)
        {
            filterData(numberX,RawDataWidth,filter);
            if(ui->displayDataCheckBox->isChecked())
            plot->showDataSeries(start,stop,dataStart,numberX,DataWidth);
            qDebug()<<"numberX"<<numberX;
            qDebug()<<"upLimitX"<<upLimitX;
           if((upLimitX-1)>0)
           {
               if(headStop>headerChar[upLimitX-1].valVec.data()->size())
               {
                   headStop = headerChar[upLimitX-1].valVec.data()->size();
               }
               if(headStart>headerChar[upLimitX-1].valVec.data()->size())
               {
                   headStart = 0;
               }
               int end=headStop;
               if(headStop>3)
               {

                   while(headerChar[upLimitX-1].valVec.at(start).at(end)!=' ')
                       end++;
               }

               plot->setNameText( headerChar[upLimitX-1].valVec.at(start).mid(headStart,end),showHead);
           }
//            {
//                plot->setNameText(headerChar[downLimitX-2].valVec.at(start).mid(0,3),1);
////                qDebug()<<headerChar[upLimitX-1].valVec.at(start);
//            }

        }
        break;
    default:
        break;
    }
    if(rescale)
    {
        plot->xAxis->rescale();
        plot->yAxis->rescale();
        plot->yAxis->setRange(plot->yAxis->range().lower-qAbs(plot->yAxis->range().lower*0.2),plot->yAxis->range().upper+qAbs(plot->yAxis->range().upper*0.25));
    }
    QString stateLabel;
    stateLabel=stateLabel.asprintf("%d/%d",state+1,totalState+1);
    ui->stateLabel->setText(stateLabel);


}

void MainWindow::copyToSource()
{
    int j = 0;
    float temp;
    for(int i=upLimitX;i<=downLimitX;i++)
    {
        j = state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
         while(QString::compare(limitData[i][j], "\0", Qt::CaseInsensitive)!=0)
         {
            temp = limitData[i][j].toFloat();
           defaultData[i][j] =  QString::number(temp);
           //qDebug()<<defaultData[i][j];
            j++;
         }
    }

}

void MainWindow::copyFromDefault()
{
    int j = 0;
    if(limitDataWidth)
    {
        for(int i=upLimitX;i<=downLimitX;i++)
        {
            for(j = state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; j<state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart + ui->pointLineEdit->text().toInt();j++)
            {
                limitCurvesData[i].valVec.replace(j,backupCurvesData[i].valVec.at(j));
            }
        }
    }
    if(RawDataWidth)
    {
        qDebug()<<"数据还原";
        for (int i=numberX; i < RawDataWidth; i++)
        {
            qDebug()<<allRawCurvesData[i].valVec.size()<<backupRawCurvesData[i].valVec.size();
            for(j = state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; j<state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart + ui->pointLineEdit->text().toInt();j++)
            {
                allRawCurvesData[i].valVec.replace(j,backupRawCurvesData[i].valVec.at(j));
            }
        }
    }
}

void MainWindow::getStateNum()
{
    if(limitFlag==0)
//    {
//        qDebug()<<(limitCurvesData[upLimitX].valVec.size()-dataStart)/PHASE/ui->pointLineEdit->text().toInt();
        totalState=(limitRow-dataStart)/PHASE/ui->pointLineEdit->text().toInt()-1;
//    }
    if(dataFlag==0)
    {
//        qDebug()<<"allRawCurvesDataAT"<<allRawCurvesData[numberX].valVec.size();
//        qDebug()<<" allCurvesData[numberX].size"<< allCurvesData[numberX].valVec.size();
//        qDebug()<<"getStateNum"<<(allCurvesData[numberX].valVec.size()-dataStart)/PHASE/ui->pointLineEdit->text().toInt();
        totalState=(row-dataStart)/PHASE/ui->pointLineEdit->text().toInt()-1;
    }
    if(totalState<0)
    {
       totalState = 0;
    }
    if(state>totalState)
    {
        state=0;
    }
}
/*********************************************************************************************
    * File Name     :openLimit
    * Description   :打开规格
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
int MainWindow::openLimit()
{

    QFileDialog *FileDialog = new QFileDialog(this);
    QString fileName = FileDialog->getOpenFileName(this,tr("Open File"),qApp->applicationDirPath (),tr("Excel(*.csv)"));
    if(fileName == "")
        return 1;
    if(QString::compare(lastFileName, fileName, Qt::CaseInsensitive)!=0)
    {
        state = 0;
    }
    lastFileName = fileName;
    QDir dir = QDir::currentPath();
    QFile file(dir.filePath(fileName));

    dataStart=ui->comboBox->currentIndex();

    for(int i=0; i<limitXMax; i++)
    {
        for(int j=0; j<yMAX; j++)
        {
            limitData[i][j].clear();
            defaultData[i][j].clear();
            tmp[i][j].clear();
        }
    }

    if(file.open(QIODevice::ReadOnly))
    {
//         qDebug()<<"OPEN FILE ok";
         ui->infoLabel->setText("获取规格成功！");
    }
    QTextStream * read = new QTextStream(&file);//文本流
    QString temp = read->readAll();

    QStringList tempOption = temp.split("\r\n",QString::SkipEmptyParts);//以"\r\n"区分行
    QString  tempData;
    limitDataWidth = 0;
    limitDataWidth = numberX;
    for(int i = 0;i<limitCurvesData.size();i++)
    {
        limitCurvesData[i].keyVec.clear();
        limitCurvesData[i].valVec.clear();


    }
    for(int i = 0;i<limitChar.size();i++)
    {
        limitChar[i].valVec.clear();
    }
    limitDataX = tempOption.count() ;
    qDebug()<<"tempOption.count"<<tempOption.count() ;
    if(tempOption.count()>limitCurvesData.size())
    {
       limitDataX=limitCurvesData.size();
    }
    qDebug()<<"行数"<<limitDataX;

    for(int i = 0 ; i < limitDataX ; i++)
    {
        QStringList tempbar = tempOption.at(i).split(",");//一行中的单元格以，区分
        limitRow=tempbar.count();
       for(int j = 0 ; j < tempbar.count() ; j++)
       {
          QStringList temp = tempbar.at(j).split(",");
          tempData = temp.join(",");
          limitCurvesData[i].keyVec.append(j);
          limitCurvesData[i].valVec.append(tempData.toDouble());
          limitChar[i].valVec.append(tempData);
       }
        qDebug()<<"i"<<i ;
    }

    file.close();
    qDebug()<<"备份" ;
    qDebug()<<"数据宽度" <<limitRow;
    qDebug()<<"容器宽度" <<limitCurvesData[10].valVec.size();
    backupCurvesData = limitCurvesData;
    totalState=dataStart;

//    offsetPonit = state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
//    copyToSource();
    return 0;
}



/*********************************************************************************************
    * File Name     :saveLimit
    * Description   :保存规格
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
int MainWindow::saveLimit()
{
    mySave->close();
    backupCurvesData = limitCurvesData ;
    QFileDialog *FileDialog = new QFileDialog(this);
    QString fileName = FileDialog->getSaveFileName(this,tr("Open File"),qApp->applicationDirPath (),tr("Excel(*.csv)"));
    if(fileName == "")
    {
//        QMessageBox::information(this,tr("保存"),tr("保存失败！请检查文件名！"),tr("确定"));
        return 1;
    }
    QDir dir = QDir::currentPath();

    QFile file(dir.filePath(fileName));

    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"OPEN FILE ok";

    }
    else
    {
        QMessageBox::information(this,tr("保存"),tr("保存失败！请检查文件是否被占用！"),tr("确定"));
    }
    QTextStream write(&file);//文本流
    qDebug()<<"limit size"<<limitCurvesData[upLimitX].valVec.size();
    if(creatFlag==1)
    {
        for(int i=upLimitX;i<=downLimitX;i++)
        {
            int stop = limitCurvesData[i].valVec.size();
            if(stop>limitChar[i].valVec.size())
            {
                stop = limitChar[i].valVec.size();
            }
            for(int j=dataStart;j< limitCurvesData[i].valVec.size();j++)
            {
                limitChar[i].valVec.replace(j,QString::number(limitCurvesData[i].valVec.at(j)));
            }
        }
        qDebug()<<"copy ok!";
        for( int i=0;i<=downLimitX;i++)
        {
            int j = 0;
            limitChar[i].valVec.resize(totalState*(PHASE*ui->pointLineEdit->text().toInt())+ PHASE*ui->pointLineEdit->text().toInt()+dataStart);
            while(j<limitChar[i].valVec.size())
            {
                write<<limitChar[i].valVec[j]<<",";
                j++;
            }
            write<<'\n';
            //         qDebug()<<"j="<<j;
        }
    }else if(creatFlag==0)
    {
        qDebug()<<"创建规格保存";
        for( int i=upLimitX;i<=downLimitX;i++)
        {
            int j = dataStart;

            while(j<totalState*(PHASE*ui->pointLineEdit->text().toInt())+PHASE*ui->pointLineEdit->text().toInt()+ dataStart)
            {
                write<<limitCurvesData[i].valVec[j]<<",";
                j++;
            }
            write<<'\n';
        }
    }
    file.close();
    ui->infoLabel->setText("保存成功！");

    QMessageBox::information(this,tr("保存"),tr("保存成功！"),tr("确定"));
    return 0;
}



/*********************************************************************************************
    * File Name     :createLimitLine
    * Description   :创建规格曲线
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::createLimitLine()
{
   QString stateLabel;
   stateLabel=stateLabel.asprintf("%d/%d",state+1,totalState+1);
   ui->stateLabel->setText(stateLabel);

   upLimitSeries->clear();
   downLimitSeries->clear();
   int same=0;
   int numberY  =  state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;;
   int numberX = 0;


       for(int j=numberY; j<numberY+ui->pointLineEdit->text().toInt(); j++)
       {
         if((int)(limitData[upLimitX][j].toFloat()*1000)==(int)(limitData[downLimitX][j].toFloat()*1000))
         {

                   same++;
           if(same>ui->pointLineEdit->text().toInt()*0.5)
           {
               return;
           }
         }
       }



   while(numberY < state*(PHASE*ui->pointLineEdit->text().toInt())+dataStart+ui->pointLineEdit->text().toInt())
   {

     // qDebug()<<data[i][j];
      upLimitSeries->append(numberX, limitData[upLimitX][numberY].toFloat());
      numberY++;
      numberX++;
   }
   numberY =state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
   numberX = 0;
   while(numberY < state*(PHASE*ui->pointLineEdit->text().toInt())+dataStart+ui->pointLineEdit->text().toInt())
   {

     // qDebug()<<data[i][j];
      downLimitSeries->append(numberX, limitData[downLimitX][numberY].toFloat());
      numberY++;
      numberX++;
   }
   findMaxMin();
   m_chart->createDefaultAxes();
   m_chart->axes(Qt::Horizontal).first()->setRange(0,  ui->pointLineEdit->text());
   m_chart->axes(Qt::Vertical).first()->setRange(limitData[downLimitX][CoodXMin].toFloat()-limitData[upLimitX][CoodXMax].toFloat()*0.2,  limitData[upLimitX][CoodXMax].toFloat()+limitData[upLimitX][CoodXMax].toFloat()*0.2);

   QValueAxis *axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first());
   Q_ASSERT(axisY);
   axisY->setLabelFormat("%.1f");
   axisY->setTickCount(10);
   QValueAxis *axisX = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Horizontal).first());
   Q_ASSERT(axisX);
   axisX->setLabelFormat("%.0f");

}

QFile file;
/*********************************************************************************************
    * File Name     :openData
    * Description   :打开参照数据
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
int MainWindow::openData()
{
    QFileDialog *FileDialog = new QFileDialog(this);
    QString fileName = FileDialog->getOpenFileName(this,tr("Open File"),qApp->applicationDirPath (),tr("Excel(*.csv)"));

    if(fileName == "")
        return 1;
    lastFileName = fileName;
    QDir dir = QDir::currentPath();
    file.setFileName(dir.filePath(fileName));
    dataStart= ui->comboBox->currentIndex();


    if(file.open(QIODevice::ReadOnly))
    {
//         qDebug()<<"OPEN FILE ok";
        ui->infoLabel->setText("获取数据成功！");

    }
    QTextStream * read = new QTextStream(&file);//文本流
    QString temp = read->readAll();
    QString tempData;
    QStringList tempOption = temp.split("\r\n",QString::SkipEmptyParts);//以"\r\n"区分行

    dataLineNum = tempOption.count() ;

    for(int i = 0;i<allCurvesData.size();i++)
    {
        allCurvesData[i].keyVec.clear();
        allCurvesData[i].valVec.clear();

    }
    for(int i = 0;i<allRawCurvesData.size();i++)
    {
        allRawCurvesData[i].keyVec.clear();
        allRawCurvesData[i].valVec.clear();
    }
    for(int i = 0;i<headerChar.size();i++)
    {
        headerChar[i].valVec.clear();
    }
    DataWidth = 0;
    RawDataWidth=0;
    DataWidth = dataLineNum;
    RawDataWidth=dataLineNum;

    qDebug()<<"数据行数"<<dataLineNum;
    if(allRawCurvesData.size()<dataLineNum)
    {
       dataLineNum=allRawCurvesData.size();
    }

    myWait->setWindowTitle("加载数据");
    myWait->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Widget);
    myWait->setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
    myWait->show();
    emit doDataSplit(tempOption,dataLineNum);
//    for(int i = 0 ; i < dataLineNum ; i++)
//    {
//        QStringList tempbar = tempOption.at(i).split(",");//一行中的单元格以，区分
//        if(i==0)
//        row=tempbar.count();
//       for(int j = 0 ; j < row; j++)
//       {
//          QStringList temp = tempbar.at(j).split(",");
//          tempData = temp.join(",");
//          allRawCurvesData[i].keyVec.append(j);
//          allRawCurvesData[i].valVec.append(tempData.toDouble());
//          backupRawCurvesData[i].keyVec.append(j);
//          backupRawCurvesData[i].valVec.append(tempData.toDouble());
//          headerChar[i].valVec.append(tempData);
//       }

//    }
//    file.close();
    qDebug()<<"数据加载完成";
    totalState=dataStart;


//    RawDataFilterCriteria();
    return 0;
}
void MainWindow::closeFile()
{
     file.close();
     dataFlag = 0;
     creatFlag=1;
    //ui->infoLabel->setText("绘制数据曲线...");

     sortingData();
     getStateNum();
     RawDataAverageAndVariance();
     RawDataFilterCriteria();
     myWait->close();
     showSeries(DataSeries,1);

    if(limitFlag ==0 )
     showSeries(LimitSeries,1);
}
void OpenDataThread::dataSplit(QStringList tempOption,int dataLineNum)
{
    qDebug()<<"dataSplit";
    QString tempData;
    for(int i = 0 ; i < dataLineNum ; i++)
    {
        QStringList tempbar = tempOption.at(i).split(",");//一行中的单元格以，区分
        if(i==0)
            row=tempbar.count();
        for(int j = 0 ; j < row; j++)
        {
            QStringList temp = tempbar.at(j).split(",");
            tempData = temp.join(",");
            allRawCurvesData[i].keyVec.append(j);
            allRawCurvesData[i].valVec.append(tempData.toDouble());
            backupRawCurvesData[i].keyVec.append(j);
            backupRawCurvesData[i].valVec.append(tempData.toDouble());
            headerChar[i].valVec.append(tempData);

        }
        emit waitProgressBarUpDate(dataLineNum,i);
    }
    emit dataSplitOk();
}
void MainWindow:: sortingData()
{
    int zero=0;
    int point = POINTNUM;
    int count = numberX;
    for(int i=numberX;i<DataWidth;i++)
    {
        for(int j=dataStart;j<allRawCurvesData[i].valVec.size();j++)
        {
            if(allRawCurvesData[i].valVec.at(j)==0)
            {
                zero++;
            }
            if(zero>=point)
            {
               break;
            }
        }
        if(zero>=point)
        {
            zero=0;
            continue;
        }
        allCurvesData[count]=allRawCurvesData[i];
        count++;
    }
    allRawCurvesData=allCurvesData;
    RawDataWidth= DataWidth=count--;
    qDebug()<<"RawDataWidth"<<RawDataWidth;
    qDebug()<<"DataWidth"<<DataWidth;
}

void MainWindow::RawDataAverageAndVariance()
{
    int y;
    int zeroAverage =0;
    float yAve=0;
    int start;
    int stop;
    float sigma = 0;
    {
        start = 0;
        stop = totalState+1;
    }
    qDebug()<<"totalState"<<totalState;
    for(int i=0;i<100000;i++)
    {
        dataVariance[i]=0;
        dataAverange[i]=0;
    }
    qDebug()<<"allCurvesDataSize"<<allCurvesData.size();
    qDebug()<<"DataWidth"<<DataWidth<<__LINE__;
    limitCurvesData[upLimitX].valVec.resize(totalState*(PHASE*ui->pointLineEdit->text().toInt())+ PHASE*ui->pointLineEdit->text().toInt()+dataStart);
    limitCurvesData[downLimitX].valVec.resize(totalState*(PHASE*ui->pointLineEdit->text().toInt())+ PHASE*ui->pointLineEdit->text().toInt()+dataStart);
    for(int k =start;k<stop;k++)
    {
        y=k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

        for (int j =0; j <ui->pointLineEdit->text().toInt();j++)
        {

            for (int i=numberX; i < DataWidth; i++)
            {
                yAve+=allCurvesData[i].valVec.at(y+j);
            }
            (dataAverange[y+j] = yAve/(DataWidth-numberX) );

            for (int i=numberX; i < DataWidth; i++)
            {
              (sigma+= pow((allCurvesData[i].valVec.at(y+j)-dataAverange[y+j]), 2)) ;
            }
            (dataVariance[y+j] = (sigma/(DataWidth-numberX)));
             zeroAverage=0;
             yAve=0;
             sigma = 0;
        }
    }

    qDebug()<<"求完平均值";
}

void MainWindow::RawDataFilterCriteria()
{
    int start;
    int stop;
    start = 0;
    stop = totalState+1;
    int numberY=0;
    for(int i = 0;i<50;i++)
    {
        for(int j=0;j<100000;j++)
        {
            filterCurvesData[i][j]=0;
        }
    }
    for(int k=start;k<stop;k++)
    {
         numberY  =  k*(PHASE*ui->pointLineEdit->text().toInt()) + dataStart;

        for (int j =0; j <ui->pointLineEdit->text().toInt();j++)
        {
//            filterCurvesData[upLimitX][numberY+j]=(dataAverange[numberY+j]+6*dataVariance[numberY+j]+0.1*dataAverange[numberY+j]);
//            filterCurvesData[downLimitX][numberY+j]=(dataAverange[numberY+j]-6*dataVariance[numberY+j]-0.1*dataAverange[numberY+j]);

            filterCurvesData[upLimitX][numberY+j]=(dataAverange[numberY+j]+qAbs(0.1*dataAverange[numberY+j]));
            filterCurvesData[downLimitX][numberY+j]=(dataAverange[numberY+j]-qAbs(0.1*dataAverange[numberY+j]));
        }
    }
//    qDebug()<<"filterCurvesData"<<filterCurvesData[upLimitX];

}





void MainWindow:: findMaxMin()
{
    int temp=limitData[upLimitX][state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart].toFloat()*1000;
    CoodYMax=temp;
    CoodYMin=temp;
    CoodXMax=state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
    CoodXMin=state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

        for(int j=0;j<ui->pointLineEdit->text().toInt();j++)
        {
            temp=limitData[upLimitX][state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart+j].toFloat()*1000;
            if(CoodYMax<temp)
            {
                CoodYMax=temp;
                CoodXMax=state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart+j;

            }
            temp=limitData[downLimitX][state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart+j].toFloat()*1000;
            if(CoodYMin>temp)
            {
                CoodYMin=temp;
                CoodXMin=state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart+j;
            }
        }

}
/*********************************************************************************************
    * File Name     :createDataLine
    * Description   :创建数据曲线
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
int MainWindow::createDataLine()
{
    DataTable dataTable;
    QString stateLabel;
    stateLabel=stateLabel.asprintf("%d/%d",state+1,totalState+1);
    ui->stateLabel->setText(stateLabel);
    int numberY  =  state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;


    for (int i=numberX; i < dataLineNum; i++)
    {
        DataList dataList;

        if(filterData(i,numberY,filter)==0 || nullLineFilter(i,numberY,1)==0)
       {
           continue;
       }
        for (int j =0; j <ui->pointLineEdit->text().toInt();j++) {

            QPointF value((j), data[i][numberY].toFloat());
            numberY++;
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);

        }
        dataTable << dataList;
        numberY  =  state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
    }

        if(m_chart->series().length())
        {
            int i = m_chart->series().length()-1;
            for( ; i>=2;i--)
            {

                m_chart->removeSeries(m_chart->series().at(i));
            }
        }
        if(ui->displayDataCheckBox->isChecked()==false)
        {
            return 1;
        }
        QString name("Series ");
        int nameIndex = 0;

        for (const DataList &list : dataTable) {
            QLineSeries *series = new QLineSeries(m_chart);

            for (const Data &data : list)
            {
               series->append(data.first);
            }
            series->setName(name + QString::number(nameIndex));
            nameIndex++;
            m_chart->addSeries(series);
        }
        m_chart->axes(Qt::Vertical).first()->setRange(0,1);
        m_chart->createDefaultAxes();
        m_chart->axes(Qt::Horizontal).first()->setRange(0,  ui->pointLineEdit->text());
        if(limitFlag==0)
        m_chart->axes(Qt::Vertical).first()->setRange(limitData[downLimitX][CoodXMin].toFloat()-(limitData[upLimitX][CoodXMax].toFloat()*0.2),  limitData[upLimitX][CoodXMax].toFloat()+limitData[upLimitX][CoodXMax].toFloat()*0.2);
        QValueAxis *axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first());
        Q_ASSERT(axisY);
        axisY->setLabelFormat("%.1f");
        axisY->setTickCount(10);
        QValueAxis *axisX = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Horizontal).first());
        Q_ASSERT(axisX);
        axisX->setLabelFormat("%.0f");

        return 0;

}

int MainWindow::nullLineFilter(int x, int y, int nData)
{
    int zero =0;

    int stop=y;
    for(;y<stop + ui->pointLineEdit->text().toInt();y++)
    {
        if(nData==1)
        {
            if(data[x][y].toFloat()*1000==0)
            {
                zero++;

            }
        }else
        {
            if(limitData[x][y].toFloat()*1000==0)
            {
                zero++;

            }
        }
            if(zero>ui->pointLineEdit->text().toInt()/2)
            {
                return 0;
            }



    }
    return 1;

}



int MainWindow::newNullLineFilter(int x, int y, int nData)
{
    int zero =0;

    int stop=y;
    for(;y<stop + ui->pointLineEdit->text().toInt();y++)
    {
        if(nData==1)
        {
            if(dataFilter[x][y]*1000==0)
            {
                zero++;

            }

        }
            if(zero>ui->pointLineEdit->text().toInt()/2)
            {
                return 0;
            }



    }
    return 1;

}

/*********************************************************************************************
    * File Name     :closeEvent
    * Description   :退出事件函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::closeEvent( QCloseEvent * event )
{
    switch( QMessageBox::information( this, tr("提示"), tr("未保存的数据将会丢失，是否退出?"), tr("滚"), tr("再想想"), 0, 1 ) )
       {
         case 0:
              event->accept();
              break;
         case 1:
         default:
             event->ignore();
             break;
       }
}


/*********************************************************************************************
    * File Name     :on_addLimitPushButton_clicked
    * Description   :添加Limit按键槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_addLimitPushButton_clicked()
{


    if(openLimit()==0)
    {
        limitFlag = 0;
       // ui->infoLabel->setText("绘制规格曲线...");
//         showSeries(LimitSeries,1);
        getStateNum();
        showSeries(LimitSeries,1);

        //ui->infoLabel->setText("绘制规格曲线完成");
    }


}



/*********************************************************************************************
    * File Name     :on_addDataPushButton_clicked
    * Description   :添加数据按键槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_addDataPushButton_clicked()
{
    if(openData()==0)
    {


    }


}
void MainWindow::callout()
{
    qDebug()<<"callout线程号"<<QThread::currentThreadId();
}


/*********************************************************************************************
    * File Name     :on_savePushButton_clicked
    * Description   :保存按键槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_savePushButton_clicked()
{


    mySave->setWindowTitle("保存数据");
    mySave->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Widget);
    mySave->setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
    mySave->show();

//    QMessageBox box;
//    int save= QMessageBox::information( this, tr("保存选项"), tr("请选择要保存的数据\n\n注意事项：\n\n\t保存数据时，规格不会随同保存\n\t请依次保存！\n\n"), tr("\n\t保存规格\t\t \n"), tr("\n\t保存数据\t\t \n"),tr("\n\t不保存\t\t \n"));
//    qDebug()<<"save"<<save;
//    if(save==0)
//    {
//        saveLimit();
//    }
//    else if(save==1)
//    {
//        saveData();
//    }
}


/*********************************************************************************************
    * File Name     :on_upPushButton_clicked
    * Description   :Limit上调整按键槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_upPushButton_clicked()
{
    float adjustStep;
    int adjustStart;
    int adjustEnd;
    if(limitFlag)
    {
        return;
    }
    adjustStep = ui->adjustStepLineEdit->text().toFloat();
    adjustStart = ui->startLineEdit->text().toInt() ;
    adjustEnd = ui->endLineEdit->text().toInt() ;

    switch (graphSelect) {
    case 1:
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
            limitCurvesData[upLimitX].valVec.replace(i,limitCurvesData[upLimitX].valVec.at(i)+adjustStep);
        }
//        qDebug()<<"limitData"<<temp;
        break;
    case 2:
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {


            limitCurvesData[downLimitX].valVec.replace(i,limitCurvesData[downLimitX].valVec.at(i)+adjustStep);
        }
//        qDebug()<<"limitData"<<temp;
        break;

    default:
        break;
    }
    showSeries(LimitSeries,0);
}


/*********************************************************************************************
    * File Name     :on_downPushButton_clicked
    * Description   :Limit下调整按键槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_downPushButton_clicked()
{
    float adjustStep;
    int adjustStart;
    int adjustEnd;
    if(limitFlag)
    {
        return;
    }
    adjustStep = ui->adjustStepLineEdit->text().toFloat();
    adjustStart = ui->startLineEdit->text().toInt();
    adjustEnd = ui->endLineEdit->text().toInt();
    switch (graphSelect) {
    case 1:
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {          
            limitCurvesData[upLimitX].valVec.replace(i,limitCurvesData[upLimitX].valVec.at(i)-adjustStep);
        }
//        qDebug()<<"limitData"<<temp;

        break;
    case 2:
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
            limitCurvesData[downLimitX].valVec.replace(i,limitCurvesData[downLimitX].valVec.at(i)-adjustStep);
        }
//        qDebug()<<"limitData"<<temp;

        break;

    default:
        break;
    }
    showSeries(LimitSeries,0);
}


/*********************************************************************************************
    * File Name     :on_leftPushButton_clicked
    * Description   :Limit左调整按键槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_leftPushButton_clicked()
{
    int adjustStart;
    int adjustEnd;
    float temp [20000]; ;
    if(limitFlag)
    {
        return;
    }
    adjustStart = ui->startLineEdit->text().toInt();
    adjustEnd = ui->endLineEdit->text().toInt();
    switch (graphSelect) {
    case 1:
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
                temp[i]= limitCurvesData[upLimitX].valVec.at(i);
        }
        adjustStart--;
        adjustEnd--;
        ui->startLineEdit->setText(QString::number(adjustStart));
        ui->endLineEdit->setText(QString::number(adjustEnd));
        if(adjustStart<0)
        {
            adjustStart=0;
            ui->startLineEdit->setText(QString::number(adjustStart));
        }

        if(adjustEnd<0)
        {
            adjustEnd=0;
            ui->endLineEdit->setText(QString::number(adjustEnd));
        }
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
                limitCurvesData[upLimitX].valVec.replace(i,temp[i+1]);
        }
            break;
    case 2:
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
               temp[i]= limitCurvesData[downLimitX].valVec.at(i);
        }
        adjustStart--;
        adjustEnd--;
        ui->startLineEdit->setText(QString::number(adjustStart));
        ui->endLineEdit->setText(QString::number(adjustEnd));
        if(adjustStart<0)
        {
            adjustStart=0;
            ui->startLineEdit->setText(QString::number(adjustStart));
        }

        if(adjustEnd<0)
        {
            adjustEnd=0;
            ui->endLineEdit->setText(QString::number(adjustEnd));
        }
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
               limitCurvesData[downLimitX].valVec.replace(i,temp[i+1]);
        }
            break;

    default:
        break;
    }
    showSeries(LimitSeries,0);
}



/*********************************************************************************************
    * File Name     :on_rightPushButton_clicked
    * Description   :Limit右调整按键槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_rightPushButton_clicked()
{
    int adjustStart;
    int adjustEnd ;
    float temp [20000];
    if(limitFlag)
    {
        return;
    }
    adjustStart = ui->startLineEdit->text().toInt();
    adjustEnd = ui->endLineEdit->text().toInt();
    switch (graphSelect) {
    case 1:
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
                temp[i]= limitCurvesData[upLimitX].valVec.at(i);
        }
        adjustStart++;
        adjustEnd++;
        ui->startLineEdit->setText(QString::number(adjustStart));
        ui->endLineEdit->setText(QString::number(adjustEnd));


        if(adjustStart>ui->pointLineEdit->text().toInt())
        {
            adjustStart=ui->pointLineEdit->text().toInt();
            ui->startLineEdit->setText(QString::number(adjustStart));
        }
        if(adjustEnd>ui->pointLineEdit->text().toInt())
        {
            adjustEnd=ui->pointLineEdit->text().toInt();
            ui->endLineEdit->setText(QString::number(adjustEnd));
        }

        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
                limitCurvesData[upLimitX].valVec.replace(i,temp[i-1]);
        }
            break;
    case 2:
        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
               temp[i]= limitCurvesData[downLimitX].valVec.at(i);
        }
        adjustStart++;
        adjustEnd++;
        ui->startLineEdit->setText(QString::number(adjustStart));
        ui->endLineEdit->setText(QString::number(adjustEnd));
        if(adjustStart>ui->pointLineEdit->text().toInt())
        {
            adjustStart=ui->pointLineEdit->text().toInt();
            ui->startLineEdit->setText(QString::number(adjustStart));
        }
        if(adjustEnd>ui->pointLineEdit->text().toInt())
        {
            adjustEnd=ui->pointLineEdit->text().toInt();
            ui->endLineEdit->setText(QString::number(adjustEnd));
        }

        for(int i=adjustStart+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=adjustEnd+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
        {
                limitCurvesData[downLimitX].valVec.replace(i,temp[i-1]);
        }
            break;

    default:
        break;
    }
    showSeries(LimitSeries,0);
}

/*********************************************************************************************
    * File Name     :on_stateDownPushButton_clicked
    * Description   :状态下切换按钮槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_stateDownPushButton_clicked()
{
    if(totalState>0)
    {
        state++;
        if(state>totalState)
            state=0;
        if(ui->displayDataCheckBox->isChecked()==true)
        {
            if(dataFlag==0)
                showSeries(DataSeries,1);
        }
             if(limitFlag==0 || creatFlag==0)
             {
                 showSeries(LimitSeries,1);
             }
    }
}


/*********************************************************************************************
    * File Name     :on_stateUpPushButton_clicked
    * Description   :状态上切换按钮槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_stateUpPushButton_clicked()
{
if(totalState>0)
{
    state--;
    if(state<0)
        state=totalState;
    if(ui->displayDataCheckBox->isChecked()==true)
    {
        if(dataFlag==0)
            showSeries(DataSeries,1);
    }
        if(limitFlag==0 || creatFlag==0)
            showSeries(LimitSeries,1);
    }
}


/*********************************************************************************************
    * File Name     :on_pointLineEdit_editingFinished
    * Description   :单个状态点数编辑框槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_pointLineEdit_editingFinished()
{

    if(dataFlag==0)
    {
        if(limitFlag)
        {

        }
        if(ui->pointLineEdit->text().toInt()>row-dataStart)
        {
            ui->pointLineEdit->setText(QString::number(row-dataStart));
        }
        getStateNum();
        RawDataAverageAndVariance();
        RawDataFilterCriteria();
        showSeries(DataSeries,1);
    }

    if(limitFlag==0 && creatFlag==1)
    {
        if(ui->pointLineEdit->text().toInt()>limitCurvesData[upLimitX].valVec.size()-dataStart)
        {
            ui->pointLineEdit->setText(QString::number(limitCurvesData[upLimitX].valVec.size()-dataStart));
        }

//        totalState=dataStart;
        getStateNum();
        showSeries(LimitSeries,1);
    }

}


/*********************************************************************************************
    * File Name     :on_exitPushButton_clicked
    * Description   :退出按钮槽函数
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
void MainWindow::on_exitPushButton_clicked()
{
    if (!(QMessageBox::information(this,tr("提示"),tr("未保存的数据将会丢失，是否退出?"),tr("滚吧"),tr("再想想"))))
          {
                QApplication* app = nullptr;
                app->exit(0);
          }
}


void MainWindow::version_slots()
{

    QMessageBox::information(this,tr("版本"),tr( "作者：亮仔庚\n"
                                                   "仅供交流学习使用！\n"
                                               "----------------------\n"
                                               "版本：V1.0_20200714\n "
                                              "新增了规格生成器以及过滤器\n"
                                               "----------------------\n"
                                               "----------------------\n"
                                               "版本：V1.1_20200715\n "
                                              "提高了数据显示上限，现在最高可以显示\n1w行，扩展了数据起始列A-Z\n"
                                               "补充：不好意思，32位版本最高只能显示1000个数据\n"
                                               "再补充：32位版本加载超过650个数据就有崩溃迹象，唉可怜的32位\n"
                                               "----------------------\n"






                                              ),tr("我知道了"));
}

void MainWindow::on_defaultPushButton_clicked()
{
    if(limitFlag==0)
    {
        copyFromDefault();
        showSeries(LimitSeries,1);

    }
    if(dataFlag==0)
    {
        copyFromDefault();
        showSeries(DataSeries,1);

    }

}

void MainWindow::on_displayDataCheckBox_clicked()
{
    if(dataFlag)
    {
        return;
    }

    if(ui->displayDataCheckBox->isChecked()==true)
    {
        showSeries(DataSeries,1);
    }else
    {
       plot->removeDataSeries();
    }

}

void MainWindow::on_endLineEdit_editingFinished()
{

    if(ui->startLineEdit->text().toInt()>ui->endLineEdit->text().toInt())
    {
        ui->endLineEdit->setText(ui->startLineEdit->text());
        QMessageBox::information(this,tr("智商题"),tr("        起点大于终点?        "),tr("  下次不再犯此错了  "));

    }

}

void MainWindow::whiteSkin_slots()
{
    if(ui->actionWhite->isChecked()==false)
    {
        QPalette pal = qApp->palette();
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
        window()->setPalette(pal);
        ui->actionWhite->setChecked(true);
        ui->actionBlue->setChecked(false);
    }
//    QFileDialog *FileDialog = new QFileDialog(this);
//    QString fileName = FileDialog->getOpenFileName(this,tr("Open File"),qApp->applicationDirPath (),tr("Excel(*.qss)"));
//    if(fileName == "")
//        return ;
//    QFile file(fileName);
//    qDebug()<<file.open(QFile::ReadOnly);
//    if (file.isOpen())
//    {
//        QString styleSheet = this->styleSheet();
//        styleSheet += QLatin1String(file.readAll());
//        this->setStyleSheet(styleSheet);
//    }

}
void MainWindow::blueSkin_slots()
{
    //if(ui->actionBlue->isChecked()==false)
    {
        QPalette pal = qApp->palette();
        pal.setColor(QPalette::Window, QRgb(0x018bba));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
        window()->setPalette(pal);
        ui->actionBlue->setChecked(true);
        ui->actionWhite->setChecked(false);


    }

}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    switch (value) {
    case 0:
        ui->horizontalSliderLabel->setText("数据过滤：不过滤");
        filter=101;
        break;
    case 1:
        ui->horizontalSliderLabel->setText("数据过滤：弱过滤");
        filter=25;
        break;
    case 2:
        ui->horizontalSliderLabel->setText("数据过滤：中过滤");
        filter=10;
        break;
    case 3:
        ui->horizontalSliderLabel->setText("数据过滤：强过滤");
        filter=5;
        break;
    case 4:
         ui->horizontalSliderLabel->setText("数据过滤：仅Pass");
         filter=1;
        break;
    default:
        break;
    }
    showSeries(LimitSeries,1);
    showSeries(DataSeries,1);
    showSeries(LimitSeries,1);

}

void MainWindow::on_createSettingPushButton_clicked()
{
    if(ui->actionBlue->isChecked()==true)
    {
        QPalette pal = s->palette();
        pal.setColor(QPalette::Window, QRgb(0x018bba));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
        s->setPalette(pal);
    }
    else
    {
        QPalette pal = s->palette();
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
        s->setPalette(pal);
    }
    s->setWindowTitle("生成设置");
    s->setWindowFlags(s->windowFlags() |Qt::Dialog);
    s->setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
    s->show();
}

extern int methods;
extern float upOffset;
extern float downOffset;
extern int nSigma;
extern int creat;
void MainWindow::on_createLimitPushButton_clicked()
{
    if(dataFlag==0)
    {
        creatLimit();
//         showSeries(LimitSeries,1);
    }
}


void MainWindow::filterDataLimit()
{
    int y;
    int zero =0;
    float yAve=0;
    int start;
    int stop;
    start = 0;
    stop = totalState;

    for(int k =start;k<=stop;k++)
    {
        y=k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;


        for (int j =0; j <ui->pointLineEdit->text().toInt();j++)
        {
            zero=0;
            yAve=0;
            int c=0;
            for (int i=numberX; i < dataLineNum; i++)
            {
                if(((int)(allRawCurvesData[i].valVec.at(y+j)*1000))==0)

                {
                    zero++;
                    continue;
                }

                    yAve+=allRawCurvesData[i].valVec.at(j+y);
                    allCurvesData[c].valVec.append(allRawCurvesData[i].valVec.at(j+y));
                    c++;
            }
             (dataCreatAverange[y+j] = yAve/(c));
            curvesDataNum = c;

        }
    }

}





void MainWindow::Variance()
{
    int y;
    int zero =0;

    float sigma=0;

    int start;
    int stop;
    {
        start = 0;
        stop = totalState+1;
    }
    for(int k =start;k<stop;k++)
    {
        y=k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

        for (int j =0; j <ui->pointLineEdit->text().toInt();j++)
        {
            zero=0;

            sigma = 0;

            for (int i=0; i < dataLineNum; i++)
            {

                 if(((int)(allRawCurvesData[i].valVec.at(y+j)*1000)) == 0)
                {
                    zero++;
                    continue;
                }
                (sigma+= pow((allRawCurvesData[i].valVec.at(y+j)-dataCreatAverange[y+j]), 2)) ;
            }
             (dataCreatVariance[y+j] = sqrt(sigma/(dataLineNum-zero-numberX)));

        }
    }
}

void MainWindow::maxAndMin()
{
    int y;
    int zeroAverage =0;
    int start;
    int stop;

    {
        start = 0;
        stop = totalState+1;
    }
    for(int k =start;k<stop;k++)
    {
        y=k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

        for (int j =0; j <ui->pointLineEdit->text().toInt();j++)
        {
            zeroAverage=0;
            creatDataMax[y+j]=allCurvesData[numberX].valVec.at(y+j);
            creatDataMin[y+j]=allCurvesData[numberX].valVec.at(y+j);

            for (int i=numberX; i < dataLineNum; i++)
            {

                if(((int)(allCurvesData[numberX].valVec.at(y+j)*1000)) == 0)
                {
                    zeroAverage++;
                    continue;
                }
                if(((int)(allCurvesData[numberX].valVec.at(y+j)*1000)) > ((int)(creatDataMax[y+j]*1000)))
                {
                    creatDataMax[y+j]=allCurvesData[numberX].valVec.at(y+j);
                }
                else if(((int)(allCurvesData[numberX].valVec.at(y+j)*1000)) < ((int)(creatDataMin[y+j]*1000)))
                {
                    creatDataMin[y+j]=allCurvesData[numberX].valVec.at(y+j);
                }

            }

        }
    }
}


int MainWindow::filterData(int x,int y, int level)
{
    int start;
    int stop;
    float zero =0;
    int tempx=x;
    int offset;
    QVector<CurveData> tempCurvesData(50000);//所有曲线的数据,allCurvesData[n]中又含有两个QVector，也即第n条曲线的x、y值

    qDebug()<<"过滤";
    start = 0;
    stop = totalState+1;

    for(int k =start;k<stop;k++)
    {

        offset=k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
        for (int i=x; i < y; i++)
        {
            zero=0;
            for(int j =0; j <ui->pointLineEdit->text().toInt();j++)
            {
                if(limitFlag)
                {

                    if(((int)(filterCurvesData[upLimitX][offset+j]*1000))<((int)((allRawCurvesData[i].valVec.at(j+offset))*1000)) || ((int)( filterCurvesData[downLimitX][offset+j]*1000))>((int)(allRawCurvesData[i].valVec.at(j+offset)*1000)))
                    {
                        zero++;                      
                        if((int)((zero)/ui->pointLineEdit->text().toFloat()*100)>=level)
                            break;
                    }
                }
                else
                {
                    if(((int)(limitCurvesData[upLimitX].valVec.at(j+offset)*1000))<((int)(allRawCurvesData[i].valVec.at(j+offset)*1000)) || ((int)(limitCurvesData[downLimitX].valVec.at(j+offset)*1000))>((int)(allRawCurvesData[i].valVec.at(j+offset)*1000)))
                    {
                        zero++;                       
                        if((int)((zero)/ui->pointLineEdit->text().toFloat()*100)>=level)
                            break;
                    }
                }


            }
            if((int)((zero)/ui->pointLineEdit->text().toFloat()*100)>=level)
            {
//                qDebug()<<"剔除"<<(int)((zero)/ui->pointLineEdit->text().toFloat()*100)<<"level"<<level;


                continue;
            }
             tempCurvesData[tempx]=allRawCurvesData[i];

            tempx++;
            DataWidth=tempx;
        }
        tempx=x;
    }
    qDebug()<<"过滤完";
    allCurvesData=tempCurvesData;
    if((int)((zero)/ui->pointLineEdit->text().toFloat()*100)>=level)
    {
        return 1;
    }
    return 0;
}



float  MainWindow::creatFilterData()
{
    int start;
    int stop;

    {
        start = 0;
        stop = totalState+1;
    }

    for(int k=start;k<stop;k++)
    {
        int numberY  =  k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
        for (int i=numberX; i < curvesDataNum; i++)
        {
            if(filterData(i,numberY,filter)==0 || nullLineFilter(i,numberY,1)==0)
           {
               continue;
           }
            for(int j=0;j<ui->pointLineEdit->text().toInt();j++)
            {
               (dataFilter[i][numberY+j] = data[i][numberY+j].toFloat()) ;

            }
            }

    }

    return 0;
}

void MainWindow::creatLimit()
{

    int numberY;

    for(int i=0; i<dataLineNum; i++)
    {
        for(int j=0; j<yMAX; j++)
        {
            if(i==0)
            {
                dataVariance[j]=0;
                dataAverange[j]=0;
                creatDataMax[j]=0;
                creatDataMin[j]=0;
            }
            if(i<limitXMax)
            {
                if(creatFlag==1)
                {
                    limitData[i][j].clear();
                    defaultData[i][j].clear();
                    creatFlag=0;
                }

                tmp[i][j].clear();
            }

            dataFilter[i][j]=0;
        }
    }




    int start;
    int stop;

    if(creat==0)
    {
        start = state;
        stop = state+1;
    }else
    {
        start = 0;
        stop = totalState+1;
    }



    RawDataAverageAndVariance();
//    limitCurvesData[upLimitX].valVec.clear();
//    limitCurvesData[downLimitX].valVec.clear();
    int size=0;
    for(int k=start;k<stop;k++)
    {
        numberY  =  k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

        for (int j =0; j <ui->pointLineEdit->text().toInt();j++)
            size++;
    }
    qDebug()<<"size"<<size;
    limitCurvesData[upLimitX].valVec.resize(totalState*(PHASE*ui->pointLineEdit->text().toInt())+PHASE*ui->pointLineEdit->text().toInt()+ dataStart);
    limitCurvesData[downLimitX].valVec.resize(totalState*(PHASE*ui->pointLineEdit->text().toInt())+PHASE*ui->pointLineEdit->text().toInt()+ dataStart);
    backupCurvesData[upLimitX].valVec.resize(totalState*(PHASE*ui->pointLineEdit->text().toInt())+PHASE*ui->pointLineEdit->text().toInt()+ dataStart);
    backupCurvesData[downLimitX].valVec.resize(totalState*(PHASE*ui->pointLineEdit->text().toInt())+PHASE*ui->pointLineEdit->text().toInt()+ dataStart);
    switch (methods) {
    case 0:
//        creatFilterData();

        for(int k=start;k<stop;k++)
        {
            numberY  =  k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

            for (int j =0; j <ui->pointLineEdit->text().toInt();j++) {
                limitCurvesData[upLimitX].valVec.replace(numberY+j,dataAverange[numberY+j]+nSigma*dataVariance[numberY+j]+upOffset);
                limitCurvesData[downLimitX].valVec.replace(numberY+j,dataAverange[numberY+j]-nSigma*dataVariance[numberY+j]-upOffset);
                backupCurvesData[upLimitX].valVec.replace(numberY+j,dataAverange[numberY+j]+nSigma*dataVariance[numberY+j]+upOffset);
                backupCurvesData[downLimitX].valVec.replace(numberY+j,dataAverange[numberY+j]-nSigma*dataVariance[numberY+j]-upOffset);
                qDebug()<<"j"<<numberY+j;
            }
        }

        break;
    case 1:


        for(int k=start;k<stop;k++)
        {
            numberY  =  k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

            for (int j =0; j <ui->pointLineEdit->text().toInt();j++) {

                limitCurvesData[upLimitX].valVec.replace(numberY+j,dataAverange[numberY+j]+upOffset);
                limitCurvesData[downLimitX].valVec.replace(numberY+j,dataAverange[numberY+j]-upOffset);


            }
        }
        break;
    case 2:

        maxAndMin();
        for(int k=start;k<stop;k++)
        {
            numberY  =  k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

            for (int j =0; j <ui->pointLineEdit->text().toInt();j++) {

                limitCurvesData[upLimitX].valVec.replace(numberY+j,dataAverange[numberY+j]+nSigma*dataVariance[numberY+j]+upOffset);
                limitCurvesData[downLimitX].valVec.replace(numberY+j,dataAverange[numberY+j]-nSigma*dataVariance[numberY+j]-upOffset);

            }
        }
        break;
    default:
        break;
    }
    if(phase==0)
    {
        for(int k=start;k<stop;k++)
        {
            numberY  =  k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart + ui->pointLineEdit->text().toInt();

            for (int j =0; j <ui->pointLineEdit->text().toInt();j++) {

                limitCurvesData[upLimitX].valVec.replace(numberY+j, 360) ;
                limitCurvesData[downLimitX].valVec.replace(numberY+j, 360) ;
//                limitData[0][numberY+j] = data[0][numberY+j];
//                limitData[1][numberY+j]=data[1][numberY+j];
            }
        }
    }

//    for(int j=0;j<dataStart;j++)
//    {
//        for(int i=0;i<=downLimitX;i++)
//            limitCurvesData[i].valVec.replace(j = data[i][j];

//    }
    limitFlag=0;
    creatFlag=0;
    limitDataWidth=100;
    qDebug()<<"刷新显示";
    showSeries(LimitSeries,1);

//    plot->replot();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{


    dataStart=index;


    loadSet->vuleToSetting();
    totalState=dataStart;
    while(QString::compare(data[numberX][totalState], "\0", Qt::CaseInsensitive)!=0)
    {
        totalState++;
    }
    totalState = totalState/(ui->pointLineEdit->text().toInt()*2)-1;
    if(totalState<0)
    {
        totalState = 0;
    }
    if(state>totalState)
    {
        state=0;
    }
    if(comboFlag==0)
    {
        if(dataFlag==0)
        {
            showSeries(DataSeries,1);
        }
        if(limitFlag==0)
        {
            showSeries(LimitSeries,1);
        }
    }

}


void MainWindow::limitSetting_slots()
{
    if(ui->actionBlue->isChecked()==true)
    {
        QPalette pal = loadSet->palette();
        pal.setColor(QPalette::Window, QRgb(0x018bba));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
        loadSet->setPalette(pal);
    }
    else
    {
        QPalette pal = loadSet->palette();
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
        loadSet->setPalette(pal);
    }
    loadSet->setWindowTitle("加载设置");
    loadSet->setWindowFlags(loadSet->windowFlags() |Qt::Dialog);
    loadSet->setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
    connect(loadSet,SIGNAL(ExitWin()),this,SLOT(upDateCombo()));
    loadSet->show();
}

void MainWindow::upDateCombo()
{
    ui->comboBox->setCurrentIndex(dataStart);
    if(ui->displayDataCheckBox->isChecked()==true)
    if(dataFlag==0)
    {
        showSeries(DataSeries,1);
    }
    if(limitFlag==0)
    {
        showSeries(LimitSeries,1);
    }

}

void MainWindow::on_saveBPMPushButton_clicked()
{

    int newState=state;
    QPixmap sum;
    QVector<QPixmap>p(100);
    QString dir;
    QString thisTime;
    QString name;
    QDir photo ;
    QVector<QString>filePathName(100);

    p.clear();



    if(totalState>=0)
    {
        ui->saveBPMPushButton->setText("正在截图");
        ui->saveBPMPushButton->setEnabled(false);
        for(state=0;state<=totalState;state++)
        {

            if(ui->displayDataCheckBox->isChecked()==true)
            {
                if(dataFlag==0)
                    showSeries(DataSeries,1);
            }
            if(limitFlag==0 || creatFlag==0)
            {
                showSeries(LimitSeries,1);
            }
            QTime dieTime = QTime::currentTime().addMSecs(400);
                while( QTime::currentTime() < dieTime )
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

                p[state]=plot->grab();

                filePathName[state] = "RF Capture State";

                filePathName[state] += QString::number(state);

                filePathName[state] += ".png";

        }


        state=newState;
        if(ui->displayDataCheckBox->isChecked()==true)
        {
            if(dataFlag==0)
                showSeries(DataSeries,1);
        }
        if(limitFlag==0 || creatFlag==0)
        {
            showSeries(LimitSeries,1);
        }

         dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        qApp->applicationDirPath (),
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
         thisTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
         name = "RF截图 ";
        //创建photo文件夹
        photo.mkdir(dir+"/"+name+thisTime);

        for(int i=0;i<=totalState;i++)
        {
            if(!p[i].save(dir+"/"+name+thisTime+"/"+filePathName[i],"png"))

            {
                qDebug()<<"save widget screen failed"<<endl;
            }
        }
//        int w=0;
//        int h=0;
//        if(totalState>3)
//        {
//            w=3*p[0].width()+30;
//            h=qCeil(totalState/3)*p[0].height();
//        }
//        QImage imageWithOverlay = QImage(w,h,QImage::Format_ARGB32_Premultiplied);
//        QPainter painter(&imageWithOverlay);

//        painter.setCompositionMode(QPainter::CompositionMode_Source);
//        painter.fillRect(imageWithOverlay.rect(), Qt::transparent);
//        int x=0;
//        int y=0;
//        for(int i=0;i<=totalState;i++)
//        {

//            painter.drawImage(p[0].width()*x, p[0].height()*y, p[i].toImage());
//            x++;
//            if(x>3)
//            {
//                y++;
//                x=0;
//            }
//        }
//        //        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//        //        painter.drawImage(0, 0, overlayImage);

//        painter.end();

//        if(!imageWithOverlay.save(dir+"/"+name+thisTime+"/"+"sum","png"))

//        {
//            qDebug()<<"save imageWithOverlay failed"<<endl;
//        }


        ui->saveBPMPushButton->setText("一键截图");
        ui->saveBPMPushButton->setEnabled(true);
    }
}

void MainWindow::slotGrabWidgetScreen()

{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    qApp->applicationDirPath (),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

//    QRect rect =


//    QPixmap p = this->grab(rect);
    QPixmap p= plot->grab();

    QString filePathName = "widget";

    filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");

    filePathName += ".png";

    if(!p.save(dir+"/"+filePathName,"png"))

    {

        qDebug()<<"save widget screen failed"<<endl;

    }

}
void MainWindow::clearAllData()
{
    limitDataWidth = 0;
    DataWidth = 0;
    RawDataWidth=0;
    for(int i=0;i<limitCurvesData.size();i++)
    {
        limitCurvesData[i].valVec.clear();
    }
    for(int i=0;i<allRawCurvesData.size();i++)
    {
        allRawCurvesData[i].valVec.clear();
    }
    for(int i=0;i<allCurvesData.size();i++)
    {
        allCurvesData[i].valVec.clear();
    }

}

void MainWindow::on_antialiasCheckBox_clicked()
{
    if(ui->antialiasCheckBox->isChecked()==true)
    {
        showHead=1;
        if(ui->displayDataCheckBox->isChecked()==true)
        {
            if(dataFlag==0)
                showSeries(DataSeries,1);
        }
            if(limitFlag==0 || creatFlag==0)
                showSeries(LimitSeries,1);


    }
    else
    {
        showHead=0;
        if(ui->displayDataCheckBox->isChecked()==true)
        {
            if(dataFlag==0)
                showSeries(DataSeries,1);
        }
            if(limitFlag==0 || creatFlag==0)
                showSeries(LimitSeries,1);
    }
}
#define bezierOpen 1
#define bezierClose 0
int bezierState = bezierClose;
int bezierSave = 1;
void MainWindow::on_bezierPushButton_clicked()
{
    if(bezierState==bezierClose)
    {
        if(limitFlag==0 || creatFlag==0)
        {
            QPalette palette;
            palette.setColor(QPalette::ButtonText,QColor(234,45,49));
            ui->bezierPushButton->setPalette(palette);
            ui->bezierPushButton->setText("应用贝塞尔曲线绘制");
            bezierState = bezierOpen;
            emit BezierOpen();
        }
        else
        {
            QMessageBox::information(this,tr("警告"),tr("要想使用贝塞尔曲线绘制，请先加载Limit！"),tr("确定"));
        }
    }
    else if(bezierState==bezierOpen)
    {
        bezierSave=(QMessageBox::information(this,tr("保存贝塞尔曲线"),tr("将绘制的贝塞尔曲线更新到Limit?"),tr("更新"),tr("不更新")));
        if(graphSelect!=1 && graphSelect!=2 && bezierSave==0)
        {
            QMessageBox::information(this,tr("警告"),tr("未选择任何Limit！\n""请选定所要更新的Limit！"),tr("确定"));
        }
        else
        {
            QPalette palette;
            palette.setColor(QPalette::ButtonText,QColor(18,124,188));
            ui->bezierPushButton->setPalette(palette);
            ui->bezierPushButton->setText("开启贝塞尔曲线绘制");
            bezierState = bezierClose;
            emit BezierClose(bezierSave);
        }
    }

}
void MainWindow::updateBezierToLimit(int bezierSave)
{
    if(bezierSave==0)
    {

        float adjustStep;
        int adjustStart;
        int adjustEnd;
        if(limitFlag || bezierCurvesData[0].keyVec.size()==0)
        {
            return;
        }
        adjustStep = ui->adjustStepLineEdit->text().toFloat();
        adjustStart = ui->startLineEdit->text().toInt();
        adjustEnd = ui->endLineEdit->text().toInt();
        int div=0;
        qDebug()<<"size"<<bezierCurvesData[0].keyVec.size();
        qDebug()<<"div"<<(div=bezierCurvesData[0].keyVec.last()-bezierCurvesData[0].keyVec.first());
        int begin=bezierCurvesData[0].keyVec.first();
        int end=bezierCurvesData[0].keyVec.last();
        qDebug()<<"begin"<<begin<<" end"<<end;
        qDebug()<<"start"<<begin+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart<<"  end"<<div+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
        if(end>ui->pointLineEdit->text().toInt())
        {
            end=ui->pointLineEdit->text().toInt();
        }
        QMap<int, int> idx2Key;//记录或查询：从曲线索引(key)->graph指针(val)。读之前必须查询key的存在性contains(key)?
        QMap<int, int> Key2Idx;//记录或查询：从graph指针(key)->曲线索引(val)。读之前必须查询key的存在性contains(key)?
        int KEY[1000];
        int Idx[1000];
        int p=0;

        for(int i=0;i<bezierCurvesData[0].valVec.size();i++)
        {

            idx2Key[i]=(int)(bezierCurvesData[0].keyVec.at(i));
            Key2Idx[(int)(bezierCurvesData[0].keyVec.at(i))]=i;

            if(i>0)
            {
                if(((int)(bezierCurvesData[0].keyVec.at(i)))== KEY[p-1])
                {
                    qDebug()<<"满足";
                    continue;
                }
            }
            KEY[p]=(int)(bezierCurvesData[0].keyVec.at(i));
            Idx[p]=i;
            qDebug()<<"KEY[p]"<<KEY[p]<<"(int)(bezierCurvesData[0].keyVec.at(i))"<<(int)(bezierCurvesData[0].keyVec.at(i));
            p++;
        }
        qDebug()<<"总修改点数"<<p;
        p=0;
        switch (graphSelect) {
        case 1:
            for(int i=begin+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=end+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
            {

                limitCurvesData[upLimitX].valVec.replace(i,bezierCurvesData[0].valVec.at(Idx[p]));
                p++;
            }
            break;
        case 2:
            for(int i=begin+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i<=end+state*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart; i++)
            {

                limitCurvesData[downLimitX].valVec.replace(i,bezierCurvesData[0].valVec.at(Idx[p]));
                p++;
            }
            break;
        default:
            break;
        }
        showSeries(LimitSeries,1);
    }
    bezierCurvesData[0].keyVec.clear();
    bezierCurvesData[0].valVec.clear();

}


void MainWindow::goToAve()
{
    int y;
    int start;
    int stop;
    float coefficient,scatter,mean;
    start = 0;
    stop = totalState+1;

//    qDebug()<<"totalState"<<totalState;
//    qDebug()<<"allCurvesDataSize"<<allCurvesData.size();
    qDebug()<<"DataWidth"<<DataWidth<<__LINE__;

    for(int k =start;k<stop;k++)
    {
        y=k*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;

        for (int j =0; j <ui->pointLineEdit->text().toInt();j++)
        {

            for (int i=numberX; i < RawDataWidth; i++)
            {
//                for(int run=0;run<10;run++)
                {
                    scatter=allRawCurvesData[i].valVec.at(y+j);
                    mean=dataAverange[y+j];
//                    qDebug()<<"散点收拢前"<<scatter;
//                    qDebug()<<"平均值"<<mean;
                    coefficient=(((mean-scatter)/(abs(mean)+abs(mean-scatter)))*abs(scatter-mean));
                    allRawCurvesData[i].valVec.replace(y+j,scatter+coefficient);
//                    qDebug()<<"散点收拢后"<<allCurvesData[i].valVec.at(y+j)<<endl;
                }
            }

        }
    }
    qDebug()<<"向均值收拢";
}

void MainWindow::on_goToAvePushButton_clicked()
{
    if(dataFlag==0)
    {
        goToAve();
        RawDataAverageAndVariance();
        RawDataFilterCriteria();
        showSeries(DataSeries,0);
    }
    else
    {
        QMessageBox::information(this,tr("警告"),tr("请先加载数据！"),tr("确定"));
    }
}
/*********************************************************************************************
    * File Name     :saveData
    * Description   :保存规格
    * Input         :无
    * return        :无
    * Author        :亮
    * Date          :2020/6/30
*********************************************************************************************/
int MainWindow::saveData()
{
    mySave->close();
    backupRawCurvesData = allRawCurvesData ;
    QFileDialog *FileDialog = new QFileDialog(this);
    QString fileName = FileDialog->getSaveFileName(this,tr("Open File"),qApp->applicationDirPath (),tr("Excel(*.csv)"));
    if(fileName == "")
    {
//        QMessageBox::information(this,tr("保存"),tr("保存失败！请检查文件名！"),tr("确定"));
        return 1;
    }
    QDir dir = QDir::currentPath();

//    QFile file(dir.filePath(fileName));
    file.setFileName(dir.filePath(fileName));
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"OPEN FILE ok";
    }
    else
    {
        QMessageBox::information(this,tr("保存"),tr("保存失败！请检查文件是否被占用！"),tr("确定"));
    }

    qDebug()<<" allRawCurvesData[i].valVec.size()"<<allRawCurvesData[10].valVec.size();
    qDebug()<<"长度"<<totalState*(PHASE*ui->pointLineEdit->text().toInt())+ dataStart;
    myWait->setWindowTitle("保存数据");
    myWait->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Widget);
    myWait->setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
    myWait->show();

    emit doDataJoin();
//    ui->infoLabel->setText("保存成功！");
//    myWait->close();
//   QMessageBox::information(this,tr("保存"),tr("保存成功！"),tr("确定"));
    return 0;
}

void MainWindow::saveDataOk()
{
    qDebug()<<"关闭保存";
    qDebug()<<myWait->close();
     ui->infoLabel->setText("保存成功！");
     QMessageBox::information(this,tr("保存"),tr("保存成功！"),tr("确定"));
}
void OpenDataThread:: dataJoin()
{
    emit waitProgressBarUpDate(RawDataWidth,0);
    if(dataFlag==0)
    {
        int porss=0;
         QTextStream write(&file);//文本流
        for(int i=0;i<RawDataWidth;i++)
        {
            porss++;
            emit waitProgressBarUpDate(2*RawDataWidth,porss);
            for(int j=dataStart;j< allRawCurvesData[i].valVec.size();j++)
            {

                headerChar[i].valVec.replace(j,QString::number(allRawCurvesData[i].valVec.at(j)));
            }
        }
        qDebug()<<"copy ok!";
        for( int i=0;i<RawDataWidth;i++)
        {
            porss++;
            emit waitProgressBarUpDate(2*RawDataWidth,porss);
            int j = 0;
//            headerChar[i].valVec.resize(totalState*(PHASE*ui->pointLineEdit->text().toInt())+PHASE*ui->pointLineEdit->text().toInt()+ dataStart);
            while(j<headerChar[i].valVec.size())
            {

                write<<headerChar[i].valVec[j]<<",";
                j++;
            }
//             qDebug()<<"header size"<<headerChar[i].valVec.size();
            write<<'\n';

        }
    }
     file.close();
     emit saveOK();
}

PointF tmpPoints[2500];
PointF BezierThread::m_bezier_interpolation_func(double t, PointF *points)
{

    for (int i = 1; i < pointCount; ++i)
    {

        for (int j = 0; j < pointCount - i; ++j)
        {

            if (i == 1)
            {
                tmpPoints[j].X = (double)(points[j].X * (1 - t) + points[j + 1].X * t);
                tmpPoints[j].Y = (double)(points[j].Y * (1 - t) + points[j + 1].Y * t);
                continue;
            }
            tmpPoints[j].X = (double)(tmpPoints[j].X * (1 - t) + tmpPoints[j + 1].X * t);
            tmpPoints[j].Y = (double)(tmpPoints[j].Y * (1 - t) + tmpPoints[j + 1].Y * t);
        }

    }
    return tmpPoints[0];
}
#define N 1000
void BezierThread::m_draw_bezier_curves(PointF *points, PointF *out_points, int div)
{
    double step = 1.0 /div;
    double t = 0;
    for (int i = 0; i < div; i++)
    {
        PointF temp_point = m_bezier_interpolation_func(t, points); //二阶贝塞尔曲线
        t += step;
        out_points[i] = temp_point;
    }

}

int BezierThread::m_bezier(QVector<QPointF> point,int div)
{
    QPointF bpoint;
    PointF *in = (PointF *)malloc(pointCount * sizeof(PointF));
    for (int i = 0; i < pointCount; i++)
    {
        in[i].X = point[i].x();
        in[i].Y = point[i].y();
//        qDebug()<<"y"<<point[i].y();
    }

    PointF out[N];
    m_draw_bezier_curves(in, out, div);

    for (int j = 0; j < N; j++)
    {
        bpoint.setX(out[j].X);
        bpoint.setY(out[j].Y);
        bezierPoint.push_back(bpoint);
    }


    free(in);
    return 0;
}
QMutex mutex;
int getBezierPoint = 1;
int bezierDoNumber=0;
void MainWindow::on_pushButton_clicked()
{
    if(dataFlag==0)
    {
        bezieS->setWindowTitle("贝塞尔曲线平滑");
        bezieS->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Widget);
        bezieS->setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
        bezieS->show();
    }
    else
    {
        QMessageBox::information(this,tr("警告"),tr("请先加载数据！"),tr("确定"));
    }

}
int BezierSetClose = 0;
void MainWindow::when_Bezier_Start()
{
    qDebug()<<"when_Bezier_Start";
    int start;
    int stop;
    int limitPoint=ui->pointLineEdit->text().toInt();
    int interval=((limitPoint*getBezierPoint*0.01));
    interval=getBezierPoint;
    qDebug()<<"interval"<<interval;

    if(bezierDoNumber==0)
    {
        start=state;
        stop=state+1;
    }
    else
    {
        start = 0;
        stop = totalState+1;
    }

    BezierSetClose=0;
    emit calculateBegin();
    emit mBezierCalculate->doBezierCalculate(start,stop,interval,limitPoint);

}

void MainWindow::when_BezierSet_Close()
{
    mutex.lock();
    qDebug()<<"BezierSetClose"<<BezierSetClose;
    BezierSetClose=1;
    mutex.unlock();
}
void MainWindow::when_Bezier_Close()
{
    qDebug()<<"when_Bezier_Close";
    RawDataAverageAndVariance();
    RawDataFilterCriteria();
    showSeries(DataSeries,0);
}
void BezierThread::bezierCalculate(int start,int stop,int interval,int limitPoint)
{
    qDebug()<<"bezierCalculate";
    int y;
    pointCount=0;
    int progress=0;
    for(int k =start;k<stop;k++)
    {
        y=k*(PHASE*limitPoint)+ dataStart;

        for (int i=numberX; i < RawDataWidth; i++)
        {
            mutex.lock();
            if(BezierSetClose==1)
            {


                BezierSetClose=0;
                mutex.unlock();
                emit progressBarUpDate((stop-start)*(RawDataWidth-numberX),0);
                emit calculateFish();
                return;
            }
             mutex.unlock();
            progress++;

            bezierPoint.clear();
            controlPoint.clear();
            pointCount=0;

            for (int j =0; j <limitPoint;j++)
            {


                if(((j%interval)==0)||(j==0)||(j==limitPoint-1))
                {

                    QPointF Pt;
                    Pt.setX(pointCount);
                    Pt.setY(allRawCurvesData[i].valVec.at(y+j));
                    controlPoint.push_back(Pt);
                    pointCount++;

                }

            }
            while(m_bezier(controlPoint,limitPoint)!=0);
            for (int j =0; j <limitPoint;j++)
            {
                allRawCurvesData[i].valVec.replace(y+j,bezierPoint[j].ry());
            }
            bezierPoint.clear();
            controlPoint.clear();
            pointCount=0;
            emit progressBarUpDate((stop-start)*(RawDataWidth-numberX),progress);
        }
    }
    mutex.lock();
    BezierSetClose=0;
    mutex.unlock();
    emit calculateFish();
}
