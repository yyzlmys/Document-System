/********************************************************************************
** Form generated from reading UI file 'MainWindow1.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW1_H
#define UI_MAINWINDOW1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow1Class
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_13;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_11;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_10;
    QWidget *widget_6;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_11;
    QLineEdit *account;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_7;
    QLineEdit *passwd;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_12;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_9;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *remba;
    QCheckBox *rembp;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *verticalSpacer_6;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_15;
    QPushButton *sign;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *verticalSpacer_5;

    void setupUi(QWidget *MainWindow1Class)
    {
        if (MainWindow1Class->objectName().isEmpty())
            MainWindow1Class->setObjectName("MainWindow1Class");
        MainWindow1Class->resize(905, 668);
        verticalLayout = new QVBoxLayout(MainWindow1Class);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_4 = new QSpacerItem(20, 74, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_13);

        label_12 = new QLabel(MainWindow1Class);
        label_12->setObjectName("label_12");
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\215\216\346\226\207\346\226\260\351\255\217")});
        font.setPointSize(40);
        label_12->setFont(font);

        horizontalLayout_13->addWidget(label_12);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_14);


        verticalLayout->addLayout(horizontalLayout_13);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_11);

        widget_5 = new QWidget(MainWindow1Class);
        widget_5->setObjectName("widget_5");
        horizontalLayout_10 = new QHBoxLayout(widget_5);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        widget_6 = new QWidget(widget_5);
        widget_6->setObjectName("widget_6");
        verticalLayout_3 = new QVBoxLayout(widget_6);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalSpacer_3 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        widget_7 = new QWidget(widget_6);
        widget_7->setObjectName("widget_7");
        horizontalLayout_11 = new QHBoxLayout(widget_7);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        label_11 = new QLabel(widget_7);
        label_11->setObjectName("label_11");
        QFont font1;
        font1.setPointSize(15);
        label_11->setFont(font1);

        horizontalLayout_11->addWidget(label_11);

        account = new QLineEdit(widget_7);
        account->setObjectName("account");

        horizontalLayout_11->addWidget(account);


        verticalLayout_3->addWidget(widget_7);

        widget_8 = new QWidget(widget_6);
        widget_8->setObjectName("widget_8");
        horizontalLayout_12 = new QHBoxLayout(widget_8);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName("horizontalLayout_12");
        label_7 = new QLabel(widget_8);
        label_7->setObjectName("label_7");
        label_7->setFont(font1);

        horizontalLayout_12->addWidget(label_7);

        passwd = new QLineEdit(widget_8);
        passwd->setObjectName("passwd");

        horizontalLayout_12->addWidget(passwd);


        verticalLayout_3->addWidget(widget_8);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_10->addWidget(widget_6);


        horizontalLayout_9->addWidget(widget_5);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_12);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName("verticalLayout_4");
        remba = new QCheckBox(MainWindow1Class);
        remba->setObjectName("remba");
        QFont font2;
        font2.setPointSize(12);
        remba->setFont(font2);

        verticalLayout_4->addWidget(remba);

        rembp = new QCheckBox(MainWindow1Class);
        rembp->setObjectName("rembp");
        rembp->setFont(font2);

        verticalLayout_4->addWidget(rembp);


        horizontalLayout_8->addLayout(verticalLayout_4);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_10);


        verticalLayout->addLayout(horizontalLayout_8);

        verticalSpacer_6 = new QSpacerItem(20, 73, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_6);

        widget_9 = new QWidget(MainWindow1Class);
        widget_9->setObjectName("widget_9");
        horizontalLayout_14 = new QHBoxLayout(widget_9);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName("horizontalLayout_14");
        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_15);

        sign = new QPushButton(widget_9);
        sign->setObjectName("sign");
        QFont font3;
        font3.setPointSize(16);
        sign->setFont(font3);

        horizontalLayout_14->addWidget(sign);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_16);


        verticalLayout->addWidget(widget_9);

        verticalSpacer_5 = new QSpacerItem(20, 74, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);


        retranslateUi(MainWindow1Class);

        QMetaObject::connectSlotsByName(MainWindow1Class);
    } // setupUi

    void retranslateUi(QWidget *MainWindow1Class)
    {
        MainWindow1Class->setWindowTitle(QCoreApplication::translate("MainWindow1Class", "MainWindow1", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow1Class", "\345\215\216\350\257\272\350\277\233\345\207\272\345\217\243\346\234\211\351\231\220\345\205\254\345\217\270", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow1Class", "\350\264\246\345\217\267\357\274\232", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow1Class", "\345\257\206\347\240\201\357\274\232", nullptr));
        remba->setText(QCoreApplication::translate("MainWindow1Class", "\350\256\260\344\275\217\347\224\250\346\210\267\345\220\215", nullptr));
        rembp->setText(QCoreApplication::translate("MainWindow1Class", "\350\256\260\344\275\217\345\257\206\347\240\201", nullptr));
        sign->setText(QCoreApplication::translate("MainWindow1Class", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow1Class: public Ui_MainWindow1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW1_H
