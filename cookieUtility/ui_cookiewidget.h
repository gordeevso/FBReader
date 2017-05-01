/********************************************************************************
** Form generated from reading UI file 'cookiewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COOKIEWIDGET_H
#define UI_COOKIEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CookieWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *m_nameLabel;
    QLabel *label_2;
    QLabel *m_domainLabel;
    QVBoxLayout *verticalLayout;
    QPushButton *m_viewButton;
    QPushButton *m_deleteButton;

    void setupUi(QWidget *CookieWidget)
    {
        if (CookieWidget->objectName().isEmpty())
            CookieWidget->setObjectName(QStringLiteral("CookieWidget"));
        CookieWidget->resize(300, 71);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CookieWidget->sizePolicy().hasHeightForWidth());
        CookieWidget->setSizePolicy(sizePolicy);
        CookieWidget->setMinimumSize(QSize(300, 0));
        CookieWidget->setMaximumSize(QSize(310, 16777215));
        horizontalLayout = new QHBoxLayout(CookieWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label = new QLabel(CookieWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        m_nameLabel = new QLabel(CookieWidget);
        m_nameLabel->setObjectName(QStringLiteral("m_nameLabel"));

        formLayout->setWidget(0, QFormLayout::FieldRole, m_nameLabel);

        label_2 = new QLabel(CookieWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        m_domainLabel = new QLabel(CookieWidget);
        m_domainLabel->setObjectName(QStringLiteral("m_domainLabel"));

        formLayout->setWidget(1, QFormLayout::FieldRole, m_domainLabel);


        horizontalLayout->addLayout(formLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        m_viewButton = new QPushButton(CookieWidget);
        m_viewButton->setObjectName(QStringLiteral("m_viewButton"));

        verticalLayout->addWidget(m_viewButton);

        m_deleteButton = new QPushButton(CookieWidget);
        m_deleteButton->setObjectName(QStringLiteral("m_deleteButton"));

        verticalLayout->addWidget(m_deleteButton);


        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 1);

        retranslateUi(CookieWidget);

        QMetaObject::connectSlotsByName(CookieWidget);
    } // setupUi

    void retranslateUi(QWidget *CookieWidget)
    {
        CookieWidget->setWindowTitle(QApplication::translate("CookieWidget", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("CookieWidget", "Name:", Q_NULLPTR));
        m_nameLabel->setText(QApplication::translate("CookieWidget", "Empty", Q_NULLPTR));
        label_2->setText(QApplication::translate("CookieWidget", "Domain:", Q_NULLPTR));
        m_domainLabel->setText(QApplication::translate("CookieWidget", "Emtpy", Q_NULLPTR));
        m_viewButton->setText(QApplication::translate("CookieWidget", "View", Q_NULLPTR));
        m_deleteButton->setText(QApplication::translate("CookieWidget", "Delete", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CookieWidget: public Ui_CookieWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COOKIEWIDGET_H
