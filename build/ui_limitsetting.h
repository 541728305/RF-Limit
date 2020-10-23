/********************************************************************************
** Form generated from reading UI file 'limitsetting.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIMITSETTING_H
#define UI_LIMITSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_limitSetting
{
public:
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *savePushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *exitPushButton;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *sigmaRadioButton;
    QRadioButton *averageRadioButton;
    QRadioButton *maxRadioButton;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QLineEdit *upLineEdit;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *downLineEdit;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLineEdit *sigmaLineEdit;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QComboBox *comboBox;

    void setupUi(QWidget *limitSetting)
    {
        if (limitSetting->objectName().isEmpty())
            limitSetting->setObjectName(QString::fromUtf8("limitSetting"));
        limitSetting->resize(300, 363);
        gridLayout_4 = new QGridLayout(limitSetting);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox_3 = new QGroupBox(limitSetting);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        savePushButton = new QPushButton(groupBox_3);
        savePushButton->setObjectName(QString::fromUtf8("savePushButton"));

        horizontalLayout->addWidget(savePushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        exitPushButton = new QPushButton(groupBox_3);
        exitPushButton->setObjectName(QString::fromUtf8("exitPushButton"));

        horizontalLayout->addWidget(exitPushButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);


        verticalLayout_7->addWidget(groupBox_3);


        gridLayout_4->addLayout(verticalLayout_7, 2, 0, 1, 1);

        groupBox = new QGroupBox(limitSetting);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(10, -1, 10, -1);
        sigmaRadioButton = new QRadioButton(groupBox);
        sigmaRadioButton->setObjectName(QString::fromUtf8("sigmaRadioButton"));

        horizontalLayout_3->addWidget(sigmaRadioButton);

        averageRadioButton = new QRadioButton(groupBox);
        averageRadioButton->setObjectName(QString::fromUtf8("averageRadioButton"));

        horizontalLayout_3->addWidget(averageRadioButton);

        maxRadioButton = new QRadioButton(groupBox);
        maxRadioButton->setObjectName(QString::fromUtf8("maxRadioButton"));

        horizontalLayout_3->addWidget(maxRadioButton);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        gridLayout_2->setColumnStretch(0, 1);

        gridLayout_4->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(limitSetting);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(10, 10, 10, 10);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(10);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_3);

        upLineEdit = new QLineEdit(groupBox_2);
        upLineEdit->setObjectName(QString::fromUtf8("upLineEdit"));

        verticalLayout_3->addWidget(upLineEdit);


        verticalLayout_6->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        downLineEdit = new QLineEdit(groupBox_2);
        downLineEdit->setObjectName(QString::fromUtf8("downLineEdit"));

        verticalLayout->addWidget(downLineEdit);


        verticalLayout_6->addLayout(verticalLayout);


        horizontalLayout_2->addLayout(verticalLayout_6);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(10);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(label_2);

        sigmaLineEdit = new QLineEdit(groupBox_2);
        sigmaLineEdit->setObjectName(QString::fromUtf8("sigmaLineEdit"));

        verticalLayout_2->addWidget(sigmaLineEdit);


        verticalLayout_5->addLayout(verticalLayout_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(label_4);

        comboBox = new QComboBox(groupBox_2);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout_4->addWidget(comboBox);


        verticalLayout_5->addLayout(verticalLayout_4);


        horizontalLayout_2->addLayout(verticalLayout_5);

        horizontalLayout_2->setStretch(0, 3);
        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(2, 3);

        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        gridLayout->setColumnStretch(0, 1);

        gridLayout_4->addWidget(groupBox_2, 1, 0, 1, 1);


        retranslateUi(limitSetting);

        QMetaObject::connectSlotsByName(limitSetting);
    } // setupUi

    void retranslateUi(QWidget *limitSetting)
    {
        limitSetting->setWindowTitle(QCoreApplication::translate("limitSetting", "Form", nullptr));
        groupBox_3->setTitle(QString());
        savePushButton->setText(QCoreApplication::translate("limitSetting", "\347\241\256\345\256\232", nullptr));
        exitPushButton->setText(QCoreApplication::translate("limitSetting", "\345\217\226\346\266\210", nullptr));
        groupBox->setTitle(QCoreApplication::translate("limitSetting", "\347\256\227\346\263\225", nullptr));
        sigmaRadioButton->setText(QCoreApplication::translate("limitSetting", "\317\203", nullptr));
        averageRadioButton->setText(QCoreApplication::translate("limitSetting", "\345\271\263\345\235\207\345\200\274", nullptr));
        maxRadioButton->setText(QCoreApplication::translate("limitSetting", "\346\234\200\345\244\247\346\234\200\345\260\217\345\200\274", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("limitSetting", "\345\217\202\346\225\260", nullptr));
        label_3->setText(QCoreApplication::translate("limitSetting", "\344\270\212\351\231\220\345\201\217\347\247\273\351\207\217", nullptr));
        label->setText(QCoreApplication::translate("limitSetting", "\344\270\213\351\231\220\345\201\217\347\247\273\351\207\217", nullptr));
        label_2->setText(QCoreApplication::translate("limitSetting", "\317\203\345\200\215\346\225\260", nullptr));
        label_4->setText(QCoreApplication::translate("limitSetting", "\347\224\237\346\210\220\346\225\260\351\207\217", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("limitSetting", "\345\215\225\344\270\252\347\212\266\346\200\201", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("limitSetting", "\345\205\250\351\203\250\347\212\266\346\200\201", nullptr));

    } // retranslateUi

};

namespace Ui {
    class limitSetting: public Ui_limitSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIMITSETTING_H
