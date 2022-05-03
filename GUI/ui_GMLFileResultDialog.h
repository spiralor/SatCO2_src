/********************************************************************************
** Form generated from reading UI file 'FileResult.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GMLFILERESULT_H
#define UI_GMLFILERESULT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GMLFileResultDialog
{
public:
	QGridLayout *gridLayout;
	QVBoxLayout *verticalLayout;
	QTreeWidget *twFileResult;
	QHBoxLayout *horizontalLayout;
	QCheckBox *cbAllChoose;
	QCheckBox *cbLoadAndShow;
	QDialogButtonBox *bbDecide;
	QPushButton *pbBack;
	QSpacerItem *horizontalSpacer;
	QSpacerItem *horizontalSpacer1;

	void setupUi(QDialog *GMLFileResultDialog)
	{
		if (GMLFileResultDialog->objectName().isEmpty())
			GMLFileResultDialog->setObjectName(QString::fromUtf8("GMLFileResultDialog"));
		GMLFileResultDialog->setFixedSize(580, 607);

		gridLayout = new QGridLayout(GMLFileResultDialog);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		verticalLayout = new QVBoxLayout();
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		twFileResult = new QTreeWidget(GMLFileResultDialog);
		twFileResult->setObjectName(QString::fromUtf8("twFileResult"));

		verticalLayout->addWidget(twFileResult);

		horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

		horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		horizontalLayout->addItem(horizontalSpacer);

		cbAllChoose = new QCheckBox(GMLFileResultDialog);
		cbAllChoose->setObjectName(QString::fromUtf8("cbAllChoose"));
		horizontalLayout->addWidget(cbAllChoose);

		cbLoadAndShow = new QCheckBox(GMLFileResultDialog);
		cbLoadAndShow->setObjectName(QString::fromUtf8("cbLoadAndShow"));
		horizontalLayout->addWidget(cbLoadAndShow);

		horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		horizontalLayout->addItem(horizontalSpacer1);

		pbBack = new QPushButton(GMLFileResultDialog);
		pbBack->setObjectName(QString::fromUtf8("pbBack"));
		//pbBack->setGeometry(QRect(240, 590, 60, 23));
		pbBack->setMinimumSize(QSize(60, 0));
		//pbBack->setMinimumSize(QSize(100, 0));
		//pbBack->setLayoutDirection(Qt::LeftToRight);

		horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		horizontalLayout->addItem(horizontalSpacer);

		horizontalLayout->addWidget(pbBack);

		bbDecide = new QDialogButtonBox(GMLFileResultDialog);
		bbDecide->setObjectName(QString::fromUtf8("bbDecide"));
		bbDecide->setOrientation(Qt::Horizontal);
		bbDecide->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

		horizontalLayout->addWidget(bbDecide);

		verticalLayout->addLayout(horizontalLayout);


		gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


		retranslateUi(GMLFileResultDialog);

		QMetaObject::connectSlotsByName(GMLFileResultDialog);
	} // setupUi

	void retranslateUi(QDialog *GMLFileResultDialog)
	{
		//GMLFileResultDialog->setWindowTitle(QApplication::translate("FileResultDialog", "\346\237\245\350\257\242\347\273\223\346\236\234", 0 ));
		//cbAllChoose->setText(QApplication::translate("FileResultDialog", "\345\205\250\351\200\211/\345\205\250\344\270\215\351\200\211", 0 ));
		//cbLoadAndShow->setText(QApplication::translate("FileResultDialog", "\345\212\240\350\275\275\345\220\216\347\233\264\346\216\245\346\230\276\347\244\272", 0 ));
		//pbBack->setText(QApplication::translate("FileResultDialog", "\344\270\212\344\270\200\347\272\247", 0 ));
		GMLFileResultDialog->setWindowTitle(QApplication::translate("FileResultDialog", "Search results", 0));
		cbAllChoose->setText(QApplication::translate("FileResultDialog", "Select All / None", 0));
		cbLoadAndShow->setText(QApplication::translate("FileResultDialog", "Display after loading", 0));
		pbBack->setText(QApplication::translate("FileResultDialog", "Back", 0));
	} // retranslateUi

};

namespace Ui {
	class GMLFileResultDialog : public Ui_GMLFileResultDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILERESULT_H
