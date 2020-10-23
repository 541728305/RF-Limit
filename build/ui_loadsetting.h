/********************************************************************************
** Form generated from reading UI file 'loadsetting.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADSETTING_H
#define UI_LOADSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loadSetting
{
public:
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QLineEdit *limitStartLineEdit;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QComboBox *startComboBox;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *dataStartLineEdit;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_5;
    QComboBox *comboBox;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *okPushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelPushButton;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *loadSetting)
    {
        if (loadSetting->objectName().isEmpty())
            loadSetting->setObjectName(QString::fromUtf8("loadSetting"));
        loadSetting->resize(288, 239);
        gridLayout_3 = new QGridLayout(loadSetting);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(loadSetting);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_3);

        limitStartLineEdit = new QLineEdit(groupBox);
        limitStartLineEdit->setObjectName(QString::fromUtf8("limitStartLineEdit"));
        sizePolicy.setHeightForWidth(limitStartLineEdit->sizePolicy().hasHeightForWidth());
        limitStartLineEdit->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(limitStartLineEdit);


        horizontalLayout_2->addLayout(verticalLayout_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(label_4);

        startComboBox = new QComboBox(groupBox);
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->addItem(QString());
        startComboBox->setObjectName(QString::fromUtf8("startComboBox"));

        verticalLayout_4->addWidget(startComboBox);


        horizontalLayout_2->addLayout(verticalLayout_4);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(2, 1);

        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        dataStartLineEdit = new QLineEdit(groupBox);
        dataStartLineEdit->setObjectName(QString::fromUtf8("dataStartLineEdit"));
        sizePolicy.setHeightForWidth(dataStartLineEdit->sizePolicy().hasHeightForWidth());
        dataStartLineEdit->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(dataStartLineEdit);


        horizontalLayout_3->addLayout(verticalLayout);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        verticalLayout_5->addWidget(label_5);

        comboBox = new QComboBox(groupBox);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout_5->addWidget(comboBox);


        horizontalLayout_3->addLayout(verticalLayout_5);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(2, 1);

        gridLayout->addLayout(horizontalLayout_3, 1, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(loadSetting);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        okPushButton = new QPushButton(groupBox_2);
        okPushButton->setObjectName(QString::fromUtf8("okPushButton"));

        horizontalLayout->addWidget(okPushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelPushButton = new QPushButton(groupBox_2);
        cancelPushButton->setObjectName(QString::fromUtf8("cancelPushButton"));

        horizontalLayout->addWidget(cancelPushButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);

        verticalLayout_2->setStretch(0, 2);
        verticalLayout_2->setStretch(1, 1);

        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(loadSetting);

        QMetaObject::connectSlotsByName(loadSetting);
    } // setupUi

    void retranslateUi(QWidget *loadSetting)
    {
        loadSetting->setWindowTitle(QCoreApplication::translate("loadSetting", "Form", nullptr));
        groupBox->setTitle(QString());
        label_3->setText(QCoreApplication::translate("loadSetting", "\350\247\204\346\240\274\345\274\200\345\247\213\350\241\214", nullptr));
        limitStartLineEdit->setText(QString());
        label_4->setText(QCoreApplication::translate("loadSetting", "\350\247\204\346\240\274/\346\225\260\346\215\256\345\274\200\345\247\213\345\210\227", nullptr));
        startComboBox->setItemText(0, QCoreApplication::translate("loadSetting", "A", nullptr));
        startComboBox->setItemText(1, QCoreApplication::translate("loadSetting", "B", nullptr));
        startComboBox->setItemText(2, QCoreApplication::translate("loadSetting", "C", nullptr));
        startComboBox->setItemText(3, QCoreApplication::translate("loadSetting", "D", nullptr));
        startComboBox->setItemText(4, QCoreApplication::translate("loadSetting", "E", nullptr));
        startComboBox->setItemText(5, QCoreApplication::translate("loadSetting", "F", nullptr));
        startComboBox->setItemText(6, QCoreApplication::translate("loadSetting", "G", nullptr));
        startComboBox->setItemText(7, QCoreApplication::translate("loadSetting", "H", nullptr));
        startComboBox->setItemText(8, QCoreApplication::translate("loadSetting", "I", nullptr));
        startComboBox->setItemText(9, QCoreApplication::translate("loadSetting", "J", nullptr));
        startComboBox->setItemText(10, QCoreApplication::translate("loadSetting", "K", nullptr));
        startComboBox->setItemText(11, QCoreApplication::translate("loadSetting", "L", nullptr));
        startComboBox->setItemText(12, QCoreApplication::translate("loadSetting", "M", nullptr));
        startComboBox->setItemText(13, QCoreApplication::translate("loadSetting", "N", nullptr));
        startComboBox->setItemText(14, QCoreApplication::translate("loadSetting", "O", nullptr));
        startComboBox->setItemText(15, QCoreApplication::translate("loadSetting", "P", nullptr));
        startComboBox->setItemText(16, QCoreApplication::translate("loadSetting", "Q", nullptr));
        startComboBox->setItemText(17, QCoreApplication::translate("loadSetting", "R", nullptr));
        startComboBox->setItemText(18, QCoreApplication::translate("loadSetting", "S", nullptr));
        startComboBox->setItemText(19, QCoreApplication::translate("loadSetting", "T", nullptr));
        startComboBox->setItemText(20, QCoreApplication::translate("loadSetting", "U", nullptr));
        startComboBox->setItemText(21, QCoreApplication::translate("loadSetting", "V", nullptr));
        startComboBox->setItemText(22, QCoreApplication::translate("loadSetting", "W", nullptr));
        startComboBox->setItemText(23, QCoreApplication::translate("loadSetting", "X", nullptr));
        startComboBox->setItemText(24, QCoreApplication::translate("loadSetting", "Y", nullptr));
        startComboBox->setItemText(25, QCoreApplication::translate("loadSetting", "Z", nullptr));

        label->setText(QCoreApplication::translate("loadSetting", "\346\225\260\346\215\256\345\274\200\345\247\213\350\241\214", nullptr));
        dataStartLineEdit->setText(QString());
        label_5->setText(QCoreApplication::translate("loadSetting", "\347\233\270\344\275\215", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("loadSetting", "\346\234\211\347\233\270\344\275\215", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("loadSetting", "\346\227\240\347\233\270\344\275\215", nullptr));

        groupBox_2->setTitle(QString());
        okPushButton->setText(QCoreApplication::translate("loadSetting", "\347\241\256\345\256\232", nullptr));
        cancelPushButton->setText(QCoreApplication::translate("loadSetting", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loadSetting: public Ui_loadSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADSETTING_H
