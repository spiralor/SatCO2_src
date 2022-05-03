#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthFeatures/OgrUtils>
#include <cpl_error.h>
#include <gdal_priv.h>
#include <gdalwarper.h>
#include <ogr_spatialref.h>
#include <QMessageBox>
#include "RasterUtils.h"
#include "GMLFileResultDialog.h"
#include "OERSProductLayer.h"
#include "OERSProductLayerUtils.h"
#include "ViewControlUtils.h"
#include "StringUtils.h"
#include "ColorRampUtils.h"
#include <osgEarthDrivers/xyz/XYZOptions>

using namespace std;
using namespace osgEarth::Drivers;

GMLFileResultDialog::GMLFileResultDialog(std::vector<std::string> queryLayerNames, QWidget* parent) :QDialog(parent)
{
	setupUi(this);
	setWindowTitle(tr("Query results"));
	setWindowIcon(QIcon("images/icon_is_64.png"));
	QTreeWidgetItem* qtreewidgetitem = twFileResult->headerItem();
	QString qtreewidgetitemText = "Server: GML-Cloud";
	qtreewidgetitem->setText(0, qtreewidgetitemText);
	qtreewidgetitem->setIcon(0, QIcon("images/01-32.png"));
	cbLoadAndShow->setChecked(true);
	cbLoadAndShow->setVisible(false);

	int k = 0;
	if (queryLayerNames.size() > 0)
	{
		QStringList layerNameList = QString::fromLocal8Bit(queryLayerNames[0].c_str()).split("_");
		QString layerDataset = layerNameList[0] + "_" + layerNameList[1] + "_" + layerNameList[2] + "_" + layerNameList[5];//yww
		QTreeWidgetItem *DatasetItem = new QTreeWidgetItem(twFileResult, QStringList(layerDataset));
		for (int i = 0; i < queryLayerNames.size(); i++)
		{
			QStringList layerNameList2 = QString::fromLocal8Bit(queryLayerNames[i].c_str()).split("_");
			QString layerDataset2 = layerNameList2[0] + "_" + layerNameList2[1] + "_" + layerNameList2[2] + "_" + layerNameList2[5];
			if (layerDataset2 != layerDataset)
			{
				DatasetItem = new QTreeWidgetItem(twFileResult, QStringList(layerDataset2));
				layerDataset = layerDataset2;

				k = 0;
			}

			k++;
			if (k > 300)
			{
				continue;
			}
			else
			{
				QTreeWidgetItem* twi = new QTreeWidgetItem(DatasetItem);
				DatasetItem->addChild(twi); //添加子节点
				twi->setText(0, QString::fromStdString(queryLayerNames[i]));
				twi->setIcon(0, QIcon("images/53.png"));
				twi->setCheckState(0, Qt::Unchecked);
			}
		}
		//twFileResult->collapseAll(); //结点全部折叠
		twFileResult->expandAll(); //结点全部张开
	}
	//hide all choose
	cbAllChoose->setVisible(false);


	initSettings();

	setAttribute(Qt::WA_DeleteOnClose);
}

void GMLFileResultDialog::initSettings()
{
	if (ReferenceTheInstances::useChn)
	{
		bbDecide->button(QDialogButtonBox::Cancel)->setText(QString::fromLocal8Bit("取消"));
		bbDecide->button(QDialogButtonBox::Ok)->setText(QString::fromLocal8Bit("确定"));
	}

	connect(pbBack, SIGNAL(clicked()), this, SLOT(backToUpper()));
	connect(cbAllChoose, SIGNAL(stateChanged(int)), this, SLOT(chooseAllOrNot(int)));
	connect(bbDecide, SIGNAL(accepted()), this, SLOT(loadRasterLayers()));
	connect(bbDecide, SIGNAL(rejected()), this, SLOT(reject()));
}


void GMLFileResultDialog::backToUpper()
{
	this->done(3);
}

void GMLFileResultDialog::chooseAllOrNot(int state)
{
	int count = twFileResult->topLevelItemCount();
	for (int i = 0; i < count; i++)
	{
		twFileResult->topLevelItem(i)->setCheckState(0, state ? Qt::Checked : Qt::Unchecked);
	}
}

void GMLFileResultDialog::loadRasterLayers()
{
	int count = twFileResult->topLevelItemCount();
	std::vector<OERSProductLayer> addLayers;
	std::vector<ModelLayer*> addVecLayers;
	QTreeWidgetItemIterator it(twFileResult);
	while (*it) {
		if ((*it)->checkState(0) == Qt::Checked)
		{
			std::string layerName = (*it)->text(0).toStdString().c_str();


			layerName = QString::fromStdString(layerName).replace("GML", "SIO").replace("\r\n", "").toStdString();

			//
			OERSProductLayer* productLayer = GMLFileResultDialog::getProductLayerFromDB(layerName);
			addLayers.push_back(*productLayer);

			/*OERSProductLayer* productLayer = OERSProductLayerUtils::getProductLayer(layerName);
			addLayers.push_back(*productLayer);*/

			//if (productLayer->GetProperties().Imagehdfspath.find("SSW")!=string::npos)
			//{
			//	QStringList pathList=tr(productLayer->GetProperties().Imagehdfspath.c_str()).split("/");
			//	string sswLayerName=pathList.at(pathList.length()-1).toStdString();
			//	sswLayerName=sswLayerName.substr(0,sswLayerName.length()-4);
			//	ModelLayer* sswLayer=OERSProductLayerUtils::getSSWSLayer(sswLayerName);
			//	addVecLayers.push_back(sswLayer);
			//}
		}
		++it;
	}
	if (addLayers.size() == 0 && addVecLayers.size() == 0)
	{
		return;
	}

	//limit
	if (addLayers.size() > 20 || addVecLayers.size() > 20)
	{
		int ret = QMessageBox::warning(this, "Warning",
			"To many layers have been selected.\nDo you want to change your selections ?",
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::Yes);
		if (ret == QMessageBox::Yes)
		{
			return;
		}
	}

	OERSProductLayerUtils::AddProductLayers(addLayers, cbLoadAndShow->checkState() == Qt::Checked);

	ViewControlUtils::ZoomToLayer(addLayers.at(addLayers.size() - 1).GetLayer());
	OERSProductLayerUtils::AddSSWLayers(addVecLayers, cbLoadAndShow->checkState() == Qt::Checked);
	//RasterUtils::CreateOrUpdateColorRamp();
	//ReferenceTheInstances::ColorRampBoxCamera->setNodeMask(1);
	//ReferenceTheInstances::tbRColorRamp->setChecked(true);
	/*{

	int selectedItemNum = 0;

	for(int i=0;i<this->twResult->topLevelItemCount();i++)
	{
	QTreeWidgetItem *item = this->twResult->topLevelItem(i);
	if(item->checkState(0)==Qt::Checked)
	{
	selectedItemNum++;
	OERSObserveProperties properties = _queryResults[i];
	OERSObserveLayer* observeLayer = OERSObserveLayerUtils::getObserveLayer(properties);
	if(observeLayer!=NULL)
	{
	addLayers.push_back(*observeLayer);
	}
	}
	}
	WaitingProgressDialog* waitProgressDialog = new WaitingProgressDialog(this->parentWidget());
	waitProgressDialog->show();
	AddOERSObserveLayerThread mythread(addLayers,this->cbAddAndShow->isChecked());
	mythread.start();
	QEventLoop eventLoop;
	connect(&mythread,SIGNAL(finished()),&eventLoop,SLOT(quit()));
	connect(&mythread,SIGNAL(finished()),waitProgressDialog,SLOT(accept()));
	eventLoop.exec();
	mythread.quit();
	}*/

	this->done(1);
}


OERSProductLayer* GMLFileResultDialog::getProductLayerFromDB(string layerName)
{
	OERSProductProperties prodProp;
	if (!getProductProperties(layerName, prodProp))
	{
		return NULL;
	}

	ImageLayer* rasterImageLayer = NULL;

	GDALColorTable* colorTable = OERSProductRenderer::getProdColorRamp(prodProp.ProdType);

	TileRendererOptions tileRendererOptions;

	if (colorTable != NULL)
	{
		tileRendererOptions.rendererType() = enumTileRendererType::ProductRenderer;
		tileRendererOptions.colorTable() = ColorRampUtils::getInstance()->convertColorRamp(*colorTable);
		tileRendererOptions.paletteBandIndex() = 1;

		double* breakValues = OERSProductRenderer::getBreakValues(prodProp.ProdType);
		int breakCount = colorTable->GetColorEntryCount();

		tileRendererOptions.breakValues() = breakValues;
		tileRendererOptions.breakCount() = breakCount;
	}
	else
	{
		GDALColorEntry startColor;
		GDALColorEntry endColor;

		startColor.c1 = 255;
		startColor.c2 = 0;
		startColor.c3 = 0;
		startColor.c4 = 255;

		endColor.c1 = 0;
		endColor.c2 = 0;
		endColor.c3 = 255;
		endColor.c4 = 255;

		colorTable = ColorRampUtils::getInstance()->createHSLColorRamp(startColor, endColor, 256);

		tileRendererOptions.rendererType() = enumTileRendererType::StretchRenderer;
		tileRendererOptions.colorTable() = ColorRampUtils::getInstance()->convertColorRamp(*colorTable);

		tileRendererOptions.paletteBandIndex() = 1;
	}

	tileRendererOptions.pixelMax() = prodProp.pixelMax;
	tileRendererOptions.pixelMin() = prodProp.pixelMin;
	tileRendererOptions.pixelMean() = prodProp.pixelMean;
	tileRendererOptions.pixelDev() = prodProp.pixelDev;

	if (SystemSettingUtils::getMapSettingByNodePath("/mapSetting/VPB/userDefinedVPB") == "TRUE")
	{
		XYZOptions imageryXYZOptions;
		imageryXYZOptions.url() = prodProp.Imagehdfspath.substr(16, prodProp.Imagehdfspath.length() - 20) + "/{z}/{x}/{y}.zip";
		imageryXYZOptions.format() = "tif";
		imageryXYZOptions.invertY() = true;
		imageryXYZOptions.saveDisk() = SystemSettingUtils::getMapSettingByNodePath("/mapSetting/cachePath");
		imageryXYZOptions.profile() = ProfileOptions("global-geodetic");
		imageryXYZOptions.L2CacheSize() = 128;//缓存默认16个切片
		imageryXYZOptions.userDefinedXYZ() = true;
		imageryXYZOptions.extentMinX() = prodProp.ProdLonMin;
		imageryXYZOptions.extentMinY() = prodProp.ProdLatMin;
		imageryXYZOptions.extentMaxX() = prodProp.ProdLonMax;
		imageryXYZOptions.extentMaxY() = prodProp.ProdLatMax;
		double* backgroundValue = new double[1];
		backgroundValue[0] = -9999;
		tileRendererOptions.backgroundValues() = backgroundValue;
		imageryXYZOptions.tileRendererOptions() = tileRendererOptions;
		imageryXYZOptions.maxDataLevel() = prodProp.maxDataLevel;

		ImageLayerOptions imageLayerOptionsglc(prodProp.LayerName, imageryXYZOptions);
		imageLayerOptionsglc.cachePolicy() = CachePolicy::NO_CACHE;
		imageLayerOptionsglc.guid() = ReferenceTheInstances::GuidMax + 1;
		ReferenceTheInstances::GuidMax++;
		rasterImageLayer = new ImageLayer(imageLayerOptionsglc);
	}
	else
	{
		GDALDataset *pDataset = (GDALDataset *)GDALOpen(prodProp.Imagehdfspath.c_str(), GA_ReadOnly);

		//手动设置数据的空间参考为WGS84
		if (pDataset->GetProjectionRef() == NULL || strlen(pDataset->GetProjectionRef()) <= 0)
		{
			OGRSpatialReference sr;
			sr.SetWellKnownGeogCS("WGS84");
			char* srWgs84Wtk = NULL;
			sr.exportToWkt(&srWgs84Wtk);
			pDataset->SetProjection(srWgs84Wtk);
		}

		RasterUtils::InitializeRasterBand(pDataset, colorTable);

		GDALOptions rasterGDALOption;
		rasterGDALOption.tileRendererOptions() = tileRendererOptions;

		rasterGDALOption.externalDataset() = new GDALOptions::ExternalDataset(pDataset, true);
		rasterGDALOption.url() = prodProp.Imagehdfspath;

		ImageLayerOptions rasterImageLayerOptions(prodProp.LayerName, rasterGDALOption);
		rasterImageLayerOptions.cachePolicy() = CachePolicy::NO_CACHE;
		rasterImageLayerOptions.guid() = ReferenceTheInstances::GuidMax + 1;
		ReferenceTheInstances::GuidMax++;
		rasterImageLayer = new ImageLayer(rasterImageLayerOptions);
		rasterImageLayer->setName(layerName);
	}

	OERSProductLayer* newProdLyr = new OERSProductLayer(rasterImageLayer, prodProp);
	return newProdLyr;
}

bool GMLFileResultDialog::getProductProperties(string layerName, OERSProductProperties& prodProp)
{
	if (SystemSettingUtils::getMapSettingByNodePath("/mapSetting/VPB/userDefinedVPB") == "TRUE")
	{
		prodProp.userDefinedVPB = true;
	}
	else
	{
		prodProp.userDefinedVPB = false;
	}

	if (!ReferenceTheInstances::db.open())
	{
		QMessageBox::warning(NULL, QObject::tr("Error"), QObject::tr("Unable to connect to the database, running with limited functionality."));
		return false;
	}
	//ReferenceTheInstances::db.open();
	QSqlQuery strQuery(ReferenceTheInstances::db);

	string sql = "select name,level,producttype,sta,sensor,facility, arithmetic, time_type, hourtime,region,starttime,endtime, lat_max,lat_min,lon_max,lon_min,unit,imagehdfspath,pixel_max, pixel_min, pixel_mean, pixel_dev, maxdatalevel, rnum, cnum,productmonth,resolutionnum from raster.chn_layer where name = '" + layerName + "'";
	strQuery.exec(sql.c_str());
	while (strQuery.next())
	{
		int idxfld = strQuery.record().indexOf("name");
		prodProp.LayerName = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("level");
		prodProp.ProdLevel = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("producttype");
		prodProp.ProdType = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("sta");
		prodProp.ProdSat = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("sensor");
		prodProp.SensorType = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("facility");
		prodProp.Facility = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("arithmetic");
		prodProp.ArithmeticType = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("time_type");
		prodProp.ProdTimeType = strQuery.value(idxfld).toInt();

		idxfld = strQuery.record().indexOf("hourtime");
		prodProp.ProdHour = strQuery.value(idxfld).toInt();

		idxfld = strQuery.record().indexOf("region");
		prodProp.ProductRegion = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("starttime");
		prodProp.StartTime = strQuery.value(idxfld).toDate();

		idxfld = strQuery.record().indexOf("endtime");
		prodProp.EndTime = strQuery.value(idxfld).toDate();

		idxfld = strQuery.record().indexOf("lat_max");
		prodProp.ProdLatMax = strQuery.value(idxfld).toDouble();

		idxfld = strQuery.record().indexOf("lat_min");
		prodProp.ProdLatMin = strQuery.value(idxfld).toDouble();

		idxfld = strQuery.record().indexOf("lon_max");
		prodProp.ProdLonMax = strQuery.value(idxfld).toDouble();

		idxfld = strQuery.record().indexOf("lon_min");
		prodProp.ProdLonMin = strQuery.value(idxfld).toDouble();

		idxfld = strQuery.record().indexOf("unit");
		prodProp.Unit = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();

		idxfld = strQuery.record().indexOf("imagehdfspath");
		//prodProp.Imagehdfspath = "http://" + ReferenceTheInstances::m_strServer +"/"+ strQuery.value(idxfld).toString().toLocal8Bit().toStdString();
		string imagehdfspath = strQuery.value(idxfld).toString().toLocal8Bit().toStdString();
		//prodProp.Imagehdfspath = "/vsizip/vsicurl/http://"+ReferenceTheInstances::m_strServer+"/SourceTiff/TIFF/"+imagehdfspath.substr(9,imagehdfspath.length()-12)+"zip"+SystemSettingUtils::HDFS_PATH_OPERATION;
		prodProp.Imagehdfspath = "/vsizip/vsicurl/" + SystemSettingUtils::HDFS_PATH + imagehdfspath.substr(9, imagehdfspath.length() - 12) + "zip";

		idxfld = strQuery.record().indexOf("pixel_max");
		prodProp.pixelMax = strQuery.value(idxfld).toDouble();

		idxfld = strQuery.record().indexOf("pixel_min");
		prodProp.pixelMin = strQuery.value(idxfld).toDouble();

		idxfld = strQuery.record().indexOf("pixel_mean");
		prodProp.pixelMean = strQuery.value(idxfld).toDouble();

		idxfld = strQuery.record().indexOf("pixel_dev");
		prodProp.pixelDev = strQuery.value(idxfld).toDouble();

		idxfld = strQuery.record().indexOf("maxdatalevel");
		prodProp.maxDataLevel = strQuery.value(idxfld).toInt();

		idxfld = strQuery.record().indexOf("rnum");
		prodProp.rnum = strQuery.value(idxfld).toInt();;

		idxfld = strQuery.record().indexOf("cnum");
		prodProp.cnum = strQuery.value(idxfld).toString().toInt();;

		idxfld = strQuery.record().indexOf("productmonth");
		prodProp.ProdMonth = strQuery.value(idxfld).toString().toInt();

		idxfld = strQuery.record().indexOf("resolutionnum");
		prodProp.Resolution = strQuery.value(idxfld).toString().toDouble();

		return true;
	}
	return false;
}
