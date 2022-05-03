// Description: GML数据查询
#ifndef GMLRASTERPRODUCTQUERYDIALOG_H
#define GMLRASTERPRODUCTQUERYDIALOG_H

#include <string>
#include "ui_GMLRasterProductQueryDialog.h"
#include"ReferenceTheInstances.h"
using namespace std;

class GMLRasterProductQueryDialog :public QDialog, public Ui_GMLRasterProductQueryDialog
{
	Q_OBJECT
public:
	GMLRasterProductQueryDialog(std::string GMLproducttype, QWidget* parent = 0L);
	~GMLRasterProductQueryDialog();
	//inline ~GMLRasterProductQueryDialog(){ ReferenceTheInstances::showOneDialog = false;  };
	static vector<string> QueryProductsFromDB4(
		vector<string> pProdType,
		QDate pFromTime,
		QDate pTillTime,
		bool queryAllTime,
		vector<string> pL3BProdTime,
		vector<string> pProdSat,
		bool queryAllTimeSpan);

private:
	void uiAddAndAdjust();
	void initSettings();
	void initSignals();
	string _GMLproducttype;
	vector<string> _layerNames;
	bool ValidateQueryCondition();

	private slots:
	void goQuery();
	void goCancel();
	void updateDataset();
	void updateProductType();
	void updateMixTime();
};


#endif
