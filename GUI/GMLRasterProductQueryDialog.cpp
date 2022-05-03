#include "GMLRasterProductQueryDialog.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include"ReferenceTheInstances.h"
#include "GMLFileResultDialog.h"
#include <vector>
using namespace std;

GMLRasterProductQueryDialog::GMLRasterProductQueryDialog(string GMLproducttype, QWidget* parent) :_GMLproducttype(GMLproducttype), QDialog(parent)
{
	setupUi(this);
	setWindowTitle(QStringLiteral("GML栅格数据查询"));
	setWindowIcon(QIcon("images/icon_is_64.png"));
	setWindowFlags(this->windowFlags()&~Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_DeleteOnClose);
	uiAddAndAdjust();
	initSettings();
}

GMLRasterProductQueryDialog::~GMLRasterProductQueryDialog()
{
	ReferenceTheInstances::showOneDialog = false;
}


void GMLRasterProductQueryDialog::uiAddAndAdjust()
{

}

void GMLRasterProductQueryDialog::initSettings()
{
	comboBox_3->clear();
	comboBox_5->clear();

	comboBox_3->addItem("");
	comboBox_5->addItem("");

	if (!ReferenceTheInstances::db.open())
	{
		QMessageBox::warning(NULL, QObject::tr("Error"), QObject::tr("Unable to connect to the database, running with limited functionality."));
		this->accept();
	}

	if (_GMLproducttype == "WaterClass")
	{
		comboBox_4->addItem(QString::fromStdString(_GMLproducttype));

		QSqlQuery strQuery(ReferenceTheInstances::db);
		QStandardItemModel *model = new QStandardItemModel(this);
		QStandardItem *itemNone1 = new QStandardItem("");
		model->appendRow(itemNone1);
		QString sql = "select distinct dataset from raster.gml_chn_layer where producttype = '" + QString::fromStdString(_GMLproducttype) + "'";
		string sql_check = sql.toStdString();
		strQuery.exec(sql);
		while (strQuery.next())
		{
			QString itemValue = strQuery.value(0).toString();
			comboBox_3->addItem(itemValue, itemValue);
		}
	}

	
	comboBox_5->addItem(QStringLiteral("小时平均"), "hourAverage");
	comboBox_5->addItem(QStringLiteral("日平均"), "dayAverage");
	comboBox_5->addItem(QStringLiteral("月平均"), "monthAverage");
	comboBox_5->addItem(QStringLiteral("年平均"), "yearAverage");
	this->dateTimeEdit_2->setDate(QDate::currentDate());
	this->dateTimeEdit->setDate(QDate::currentDate().addYears(-1));

	initSignals();

}

void GMLRasterProductQueryDialog::initSignals()
{
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(goQuery()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(goCancel()));
	connect(comboBox_3, SIGNAL(currentTextChanged(const QString)), this, SLOT(updateDataset()));
	connect(comboBox_4, SIGNAL(currentTextChanged(const QString)), this, SLOT(updateProductType()));
	connect(comboBox_5, SIGNAL(currentTextChanged(const QString)), this, SLOT(updateMixTime()));

}

//修改dataset引发的变化
void GMLRasterProductQueryDialog::updateDataset()
{
	QString datasetname = comboBox_3->currentData(Qt::UserRole).toString();

	if (!ReferenceTheInstances::db.open())
	{
		QMessageBox::warning(NULL, QObject::tr("Error"), QObject::tr("Unable to connect to the database, running with limited functionality."));
		this->accept();
	}

	if (_GMLproducttype == "WaterClass")
	{
		comboBox_5->clear();
		comboBox_5->addItem("");
		QSqlQuery strQuery(ReferenceTheInstances::db);
		QString sql = "select distinct time_type from raster.gml_chn_layer ";
		sql += "where dataset = '" + datasetname + "' and producttype = '" + QString::fromStdString(_GMLproducttype) + "' order by time_type";
		string sql_check = sql.toStdString();
		strQuery.exec(sql);
		while (strQuery.next())
		{
			int itemValue = strQuery.value(0).toInt();
			switch (itemValue)
			{
			case 0:
				comboBox_5->addItem(QStringLiteral("小时平均"), "hourAverage");
				break;
			case 1:
				comboBox_5->addItem(QStringLiteral("日平均"), "dayAverage");
				break;
			case 30:
				comboBox_5->addItem(QStringLiteral("月平均"), "monthAverage");
				break;
			case 365:
				comboBox_5->addItem(QStringLiteral("年平均"), "yearAverage");
				break;
			}
		}
	}
}

//修改ProductType引发的变化
void GMLRasterProductQueryDialog::updateProductType()
{

}

//修改Mixtime引发的变化
void GMLRasterProductQueryDialog::updateMixTime()
{
	QString datasetname = comboBox_3->currentData(Qt::UserRole).toString();
	QString productType = comboBox_4->currentText().toLocal8Bit();

	if (!ReferenceTheInstances::db.open())
	{
		QMessageBox::warning(NULL, QObject::tr("Error"), QObject::tr("Unable to connect to the database, running with limited functionality."));
		this->accept();
	}

	if (_GMLproducttype == "WaterClass")
	{
		QSqlQuery strQuery(ReferenceTheInstances::db);
		QString sql = "select max(endtime),min(starttime) from raster.gml_chn_layer where dataset = '" + datasetname + "' and producttype = '" + productType + "'";
		string sql_check = sql.toStdString();
		strQuery.exec(sql);
		while (strQuery.next())
		{
			QDate endtime = strQuery.value(0).toDate();
			QDate starttime = endtime.addYears(-1);
			dateTimeEdit->setDate(starttime);
			dateTimeEdit_2->setDate(endtime);
		}

	}
}

void GMLRasterProductQueryDialog::goQuery()
{
	if (ValidateQueryCondition())
	{
		_layerNames.clear();
		
		vector<string> pProdType;
		pProdType.push_back(_GMLproducttype);
		QDate pFromTime = dateTimeEdit->date();
		QDate pTillTime = dateTimeEdit_2->date();
		bool queryAllTime = false;
		vector<string> pL3BProdTime;
		pL3BProdTime.push_back(comboBox_5->currentText().toStdString());
		vector<string> pProdSat;
		pProdSat.push_back(comboBox_3->currentText().toStdString());
		bool queryAllTimeSpan = false;

		_layerNames = QueryProductsFromDB4(pProdType, pFromTime, pTillTime, queryAllTime, pL3BProdTime, pProdSat, queryAllTimeSpan);

		if (_layerNames.size() == 0)
		{
			QMessageBox::information(this, tr("Error"), tr("No data!"));
			return;
		}

		GMLFileResultDialog* gmlfrd = new GMLFileResultDialog(_layerNames);

		this->setVisible(false);
		int flag = gmlfrd->exec();
		if (flag == 3)
			this->setVisible(true);
		//gmlfrd->close();
		else
			this->close();
	}

}

void GMLRasterProductQueryDialog::goCancel()
{
	this->reject();
}

bool GMLRasterProductQueryDialog::ValidateQueryCondition()
{
	if (this->dateTimeEdit->isEnabled() && this->dateTimeEdit_2->isEnabled())
	{
		if (this->dateTimeEdit->date() > this->dateTimeEdit_2->date())
		{
			QMessageBox::warning(this, tr("Error"), tr("Start time is later than the end time!"));
			return false;
		}
	}

	if (this->comboBox_3->currentText() == "" || this->comboBox_5->currentText() == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Please select the parameter!"));
		return false;
	}

	return true;

}


vector<string> GMLRasterProductQueryDialog::QueryProductsFromDB4(
	vector<string> pProdType,
	QDate pFromTime,
	QDate pTillTime,
	bool queryAllTime,
	vector<string> pL3BProdTime,
	vector<string> pProdSat,
	bool queryAllTimeSpan)
{
	vector<string> FinalList;

	//产品类型筛选
	string whereclause1 = "";
	string cndType = "(";
	bool isAllType = false;

	if (pProdType[0] == "allType")
	{
		isAllType = true;
	}
	else
	{
		cndType += "producttype = '" + pProdType[0] + "'";
	}

	//数据集筛选
	whereclause1 += "where (dataset = '";
	whereclause1 += pProdSat[0];
	whereclause1 += "'";

	//融合时长筛选
	if (pL3BProdTime[0] == QStringLiteral("小时平均").toStdString())
	{
		whereclause1 += " AND time_type = 0";
	}
	else if (pL3BProdTime[0] == QStringLiteral("日平均").toStdString())
	{
		whereclause1 += " AND time_type = 1";
	}
	else if (pL3BProdTime[0] == QStringLiteral("月平均").toStdString())
	{
		whereclause1 += " AND time_type = 30";
	}
	else if (pL3BProdTime[0] == QStringLiteral("年平均").toStdString())
	{
		whereclause1 += " AND time_type = 365";
	}

	try
	{
		//日期筛选 endtime要+1天否则day, hour数据无法查到
		whereclause1 += " AND ((starttime >= to_date('" + pFromTime.toString("yyyy-MM-dd").toStdString() + "', 'YYYY-MM-DD') AND starttime <= to_date('" + pTillTime.toString("yyyy-MM-dd").toStdString() + "', 'YYYY-MM-DD')+1) " +
			"OR (endtime >= to_date('" + pFromTime.toString("yyyy-MM-dd").toStdString() + "', 'YYYY-MM-DD') AND endtime <= to_date('" + pTillTime.toString("yyyy-MM-dd").toStdString() + "', 'YYYY-MM-DD')+1) " +
			"OR (starttime <= to_date('" + pFromTime.toString("yyyy-MM-dd").toStdString() + "', 'YYYY-MM-DD') AND endtime >= to_date('" + pFromTime.toString("yyyy-MM-dd").toStdString() + "', 'YYYY-MM-DD')) " +
			"OR (starttime <= to_date('" + pTillTime.toString("yyyy-MM-dd").toStdString() + "', 'YYYY-MM-DD')+1 AND endtime >= to_date('" + pTillTime.toString("yyyy-MM-dd").toStdString() + "', 'YYYY-MM-DD')+1))";

		whereclause1 += ")";
		string sql = "select name from raster.gml_chn_layer " + whereclause1;
		if (!ReferenceTheInstances::db.open())
		{
			QMessageBox::warning(NULL, QObject::tr("Error"), QObject::tr("Unable to connect to the database, running with limited functionality."));
			return FinalList;
		}
		//ReferenceTheInstances::db.open();
		QSqlQuery strQuery(ReferenceTheInstances::db);
		strQuery.exec(QString::fromStdString(sql));
		while (strQuery.next())
		{
			try
			{
				//int fieldNo = strQuery.record().indexOf("name");
				string strLevel = strQuery.value(0).toString().toStdString();
				FinalList.push_back(strLevel);
			}
			catch (...)
			{
				continue;
			}
		}
		ReferenceTheInstances::db.close();

	}
	catch (...)
	{
		return FinalList;
	}

	return FinalList;
}
