#ifndef UI_GMLRASTERPRODUCTQUERYDIALOG_H
#define UI_GMLRASTERPRODUCTQUERYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GMLRasterProductQueryDialog
{
public:
	QDialogButtonBox *buttonBox;
	QGroupBox *groupBox;
	QComboBox *comboBox;
	QLabel *label;
	QPushButton *pushButton;
	//QLabel *label_2;
	//QComboBox *comboBox_2;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QComboBox *comboBox_3;
	QComboBox *comboBox_4;
	QComboBox *comboBox_5;
	QGroupBox *groupBox_2;
	QDateEdit *dateTimeEdit;
	QLabel *label_6;
	QLabel *label_7;
	QDateEdit *dateTimeEdit_2;

	void setupUi(QDialog *GMLRasterProductQueryDialog)
	{
		if (GMLRasterProductQueryDialog->objectName().isEmpty())
			GMLRasterProductQueryDialog->setObjectName(QStringLiteral("GMLRasterProductQueryDialog"));
		GMLRasterProductQueryDialog->setFixedSize(616, 279);
		buttonBox = new QDialogButtonBox(GMLRasterProductQueryDialog);
		buttonBox->setObjectName(QStringLiteral("buttonBox"));
		buttonBox->setGeometry(QRect(260, 240, 341, 32));
		buttonBox->setOrientation(Qt::Horizontal);
		buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
		groupBox = new QGroupBox(GMLRasterProductQueryDialog);
		groupBox->setObjectName(QStringLiteral("groupBox"));
		groupBox->setEnabled(true);
		groupBox->setGeometry(QRect(10, 10, 591, 71));
		comboBox = new QComboBox(groupBox);
		comboBox->setObjectName(QStringLiteral("comboBox"));
		comboBox->setEnabled(false);
		comboBox->setGeometry(QRect(80, 22, 401, 25));
		comboBox->setStyleSheet(QLatin1String("QComboBox{border:1px solid gray; border-radius:3px; padding:1px 2px 1px 2px;} QComboBox::drop-down,QDateEdit::drop-down{subcontrol-position:top right;width:20px;border-left-width:1px;border-left-color:darkgray;border-left-style:solid;border-top-right-radius:3px;border-bottom-right-radius:3px;}\n"
			""));
		label = new QLabel(groupBox);
		label->setObjectName(QStringLiteral("label"));
		label->setEnabled(true);
		label->setGeometry(QRect(20, 18, 61, 31));
		pushButton = new QPushButton(groupBox);
		pushButton->setObjectName(QStringLiteral("pushButton"));
		pushButton->setEnabled(false);
		pushButton->setGeometry(QRect(500, 22, 81, 25));
		pushButton->setStyleSheet(QStringLiteral("QPushButton{border:1px solid gray; border-radius:3px; padding:1px 2px 1px 2px;}"));
		label_3 = new QLabel(GMLRasterProductQueryDialog);
		label_3->setObjectName(QStringLiteral("label_3"));
		label_3->setEnabled(true);
		label_3->setGeometry(QRect(20, 82, 61, 41)); 
		label_4 = new QLabel(GMLRasterProductQueryDialog);
		label_4->setObjectName(QStringLiteral("label_4"));
		label_4->setEnabled(true);
		label_4->setGeometry(QRect(20, 115, 61, 41));
		label_5 = new QLabel(GMLRasterProductQueryDialog);
		label_5->setObjectName(QStringLiteral("label_5"));
		label_5->setEnabled(true);
		label_5->setGeometry(QRect(340, 115, 61, 41));
		comboBox_3 = new QComboBox(GMLRasterProductQueryDialog);
		comboBox_3->setObjectName(QStringLiteral("comboBox_3"));
		comboBox_3->setEnabled(true);
		comboBox_3->setGeometry(QRect(89, 90, 511, 25));
		comboBox_3->setStyleSheet(QLatin1String("QComboBox{border:1px solid gray; border-radius:3px; padding:1px 2px 1px 2px;} QComboBox::drop-down,QDateEdit::drop-down{subcontrol-position:top right;width:20px;border-left-width:1px;border-left-color:darkgray;border-left-style:solid;border-top-right-radius:3px;border-bottom-right-radius:3px;}\n"
			""));
		comboBox_4 = new QComboBox(GMLRasterProductQueryDialog);
		comboBox_4->setObjectName(QStringLiteral("comboBox_4"));
		comboBox_4->setEnabled(false);
		comboBox_4->setGeometry(QRect(89, 125, 191, 25));
		comboBox_4->setStyleSheet(QLatin1String("QComboBox{border:1px solid gray; border-radius:3px; padding:1px 2px 1px 2px;} QComboBox::drop-down,QDateEdit::drop-down{subcontrol-position:top right;width:20px;border-left-width:1px;border-left-color:darkgray;border-left-style:solid;border-top-right-radius:3px;border-bottom-right-radius:3px;}\n"
			""));
		comboBox_5 = new QComboBox(GMLRasterProductQueryDialog);
		comboBox_5->setObjectName(QStringLiteral("comboBox_5"));
		comboBox_5->setEnabled(true);
		comboBox_5->setGeometry(QRect(410, 125, 191, 25));
		comboBox_5->setStyleSheet(QLatin1String("QComboBox{border:1px solid gray; border-radius:3px; padding:1px 2px 1px 2px;} QComboBox::drop-down,QDateEdit::drop-down{subcontrol-position:top right;width:20px;border-left-width:1px;border-left-color:darkgray;border-left-style:solid;border-top-right-radius:3px;border-bottom-right-radius:3px;}\n"
			""));
		groupBox_2 = new QGroupBox(GMLRasterProductQueryDialog);
		groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
		groupBox_2->setEnabled(true);
		groupBox_2->setGeometry(QRect(10, 160, 591, 71));
		dateTimeEdit = new QDateEdit(groupBox_2);
		dateTimeEdit->setObjectName(QStringLiteral("dateTimeEdit"));
		dateTimeEdit->setGeometry(QRect(80, 30, 191, 22));
		label_6 = new QLabel(groupBox_2);
		label_6->setObjectName(QStringLiteral("label_6"));
		label_6->setEnabled(true);
		label_6->setGeometry(QRect(10, 20, 61, 41));
		label_7 = new QLabel(groupBox_2);
		label_7->setObjectName(QStringLiteral("label_7"));
		label_7->setEnabled(true);
		label_7->setGeometry(QRect(310, 20, 61, 41));
		dateTimeEdit_2 = new QDateEdit(groupBox_2);
		dateTimeEdit_2->setObjectName(QStringLiteral("dateTimeEdit_2"));
		dateTimeEdit_2->setGeometry(QRect(390, 30, 191, 22));
		dateTimeEdit->raise();
		label_6->raise();
		label->raise();
		label_7->raise();
		dateTimeEdit_2->raise();

		retranslateUi(GMLRasterProductQueryDialog);

		QMetaObject::connectSlotsByName(GMLRasterProductQueryDialog);
	} // setupUi

void retranslateUi(QDialog *GMLRasterProductQueryDialog)
	{
		GMLRasterProductQueryDialog->setWindowTitle(QApplication::translate("GMLRasterProductQueryDialog", "GMLRasterProductQueryDialog", 0));
		groupBox->setTitle(QApplication::translate("GMLRasterProductQueryDialog", "\346\225\260\346\215\256\345\272\223", 0));
		comboBox->clear();
		comboBox->insertItems(0, QStringList()
			<< QApplication::translate("GMLRasterProductQueryDialog", "GML-Cloud", 0)
			);
		label->setText(QApplication::translate("GMLRasterProductQueryDialog", "\351\200\211\346\213\251", 0));
		pushButton->setText(QApplication::translate("GMLRasterProductQueryDialog", "\350\277\236\346\216\245", 0));
		label_3->setText(QApplication::translate("GMLRasterProductQueryDialog", "\346\225\260\346\215\256\351\233\206", 0));
		label_4->setText(QApplication::translate("GMLRasterProductQueryDialog", "\345\217\202\346\225\260\347\261\273\345\236\213", 0));
		label_5->setText(QApplication::translate("GMLRasterProductQueryDialog", "\350\236\215\345\220\210\346\227\266\351\225\277", 0));
		groupBox_2->setTitle(QApplication::translate("GMLRasterProductQueryDialog", "\346\227\266\351\227\264\350\214\203\345\233\264", 0));
		label_6->setText(QApplication::translate("GMLRasterProductQueryDialog", "\350\265\267\345\247\213\346\227\245\346\234\237", 0));
		label_7->setText(QApplication::translate("GMLRasterProductQueryDialog", "\347\273\223\346\235\237\346\227\245\346\234\237", 0));
	} // retranslateUi

};

namespace Ui {
	class GMLRasterProductQueryDialog : public Ui_GMLRasterProductQueryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif  
