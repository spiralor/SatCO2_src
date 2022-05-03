#include "OERSMainWindow.h"

#include <osgEarthUtil/LatLongFormatter>
#include <osgEarthUtil/BrightnessContrastColorFilter>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarthDrivers/agglite/AGGLiteOptions>
//#include <osgEarthDrivers/model_feature_stencil/FeatureStencilModelOptions>
//#include <osgEarthDrivers/feature_ogr/FeatureCursorOGR>
//#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelSource>
#include <osgUtil/Optimizer>

#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>

#include <osgEarthDrivers/engine_mp/MPTerrainEngineNode>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthAnnotation/ScaleDecoration>

#include "Gui/SpaceRangeWidget.h"
#include "Gui/RasterRenderDialog.h"
#include "Gui/FlyModeDialog.h"
#include "Gui/LayerInfoDialog.h"
#include "Gui/ComboBoxGlobeLayer.h"
#include "Gui/VectorRenderDialog.h"
#include "Gui/RSCompareDataDialog.h"
#include "Gui/RasterProductQueryDialog.h" 
#include "Gui/RasterProductBroadcastDialog.h" 
#include "Gui/RSPixelQueryDialog.h"
#include "Gui/RasterCalculatorDialog.h"
#include "Gui/VectorFluxCalculateDialog.h"
#include "Gui/MapPrintDialog.h"
#include "Gui/DefaultAreaSeaFluxDialog.h"
#include "Gui/RemoteSensingFluxDialog.h"
#include "Gui/LineStatisticsDialog.h"
#include "Gui/PolygonStatisticDialog.h"
#include "Gui/RectangleStatisticDialog.h"
#include "Gui/TimeSeriesDataQueryDialog.h"
#include "Gui/DynamicSimulationDialog.h"
#include "Gui/LoadLocalExcelDialog.h"
#include "Gui/UserInformationDialog.h"
#include "Gui/AboutDialog.h"
#include "Gui/MapSourceDialog.h"
#include "Gui/SatelliteinsituComparisonDialog.h"
#include "Gui/LoadUserHDFDataDialog.h"
#include "Gui/CdiacQueryDialog.h"
#include "Gui/BuoyQueryDialog.h"
#include "Gui/DownloadProgressDialog.h"
#include "Gui/RasterGridDialog.h"
#include "Gui/InsituGridDialog.h"
#include "Gui/UDRemoteSensingFluxDialog.h"
#include "Gui/LoadNASANCDataDialog.h"
#include "Gui/RedTideDialog.h"
#include "Gui\RasterDataFilterDialog.h"
#include "Gui\ROIExtractDialog.h"
#include "Gui\RasterClassificationDialog.h"
#include "Gui\ChangjiangFluxDialog.h"
#include "Gui\OtherFluxDialog.h"
#include "Gui\GOCIProcessDialog.h"
#include "Gui\GFProcessDialog.h"
#include "Gui\HY1CProcessDialog.h"
#include "Gui\LandsatProcessDialog.h"
#include "Gui\WaterClassDialog.h"
#include "Gui\AutoRasterProductBroadcastDialog.h"
#include "Gui\UserInfoDialog.h"
#include "Gui\OffshoreWaterQualityDialog.h"
#include "Gui\GulfWaterQualityDialog.h"
#include "Gui\UrbanWaterQualityDialog.h"
#include "Gui\WarmDischargeDialog.h"
#include "Gui\MangroveForestDialog.h"
#include "Gui\SeaweedBedDialog.h"
#include "Gui\CoralReefDialog.h"
#include "Gui\AerosolDialog.h"
#include "Gui\RedTideTiffDialog.h"
#include "Gui\GMLRasterProductQueryDialog.h"

#include "HandlerUtils.h"
#include "OERSEarthManipulator.h"
#include "ThreadUtils.h"
#include "WaitingProgressDialog.h"
#include "ViewControlUtils.h"
#include "ImageLayerUtils.h"
#include "ColorRampUtils.h"


#include <Windows.h>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QDesktopServices>
#include <QtSql>
#include <QtXml> 
#include <QSqlQuery>

#include <osgEarthDrivers/wms/WMSOptions>
#include "ViewControlUtils.h"

using namespace std;

using namespace osgEarth;
using namespace osgEarth::Drivers;
using namespace osgEarth::Util;
using namespace osgEarth::Features;
using namespace osgEarth::Util::Controls;
using namespace osgEarth::Annotation;



void OERSMainWindow::showAddRasterLayerDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{

		if (!SystemSettingUtils::putUseFunction("Online data Satellite"))
			return;


		RasterProductQueryDialog* rpqd = new RasterProductQueryDialog(this->_mapNode, _colorRampBox, _viewer, _root);
		rpqd->show();
		ReferenceTheInstances::showOneDialog = true;
	}

}

void OERSMainWindow::showAddVectorLayerDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Online data In-situ"))
		return;


	CdiacQueryDialog* dia = new CdiacQueryDialog();
	dia->exec();
}

//void OERSMainWindow::showBuoyQueryDialog()
//{
//
//	BuoyQueryDialog* bqd = new BuoyQueryDialog();
//	bqd->exec();
//
//}


void OERSMainWindow::doAddNasaData()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("NC"))
		return;

	LoadNASANCDataDialog* lnc = new LoadNASANCDataDialog();
	lnc->exec();
}

void OERSMainWindow::doAddUserHDFData()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("HDF"))
		return;
	LoadUserHDFDataDialog *luhdfdd = new LoadUserHDFDataDialog();
	luhdfdd->exec();
}

void OERSMainWindow::addLocalVectorData()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Tiff"))
		return;
	QString path = QFileDialog::getOpenFileName(this, tr("Local raster data"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("Images (*.tif);"));
	string pathOK = path.toLocal8Bit().toStdString();
	path = QString::fromLocal8Bit(pathOK.data());
	if (path == "")
	{
		return;
	}
	QStringList list = path.split("/");
	QString FileName = list.at(list.length() - 1);
	if (path.toStdString() == "")
		return;
	if (path.endsWith(".tif") || path.endsWith(".img"))
	{
		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
		
		//计算
		//RasterUtils::DeleteNanValueFromImage(path);

		GDALDataset* pDataset = (GDALDataset *)GDALOpen(path.toStdString().c_str(), GA_ReadOnly);

		double *dGeotransform = new double[6];
		if (pDataset->GetGeoTransform(dGeotransform))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Lack of coordinates!"), tr("Yes"));
			return;
		}

		if (pDataset->GetProjectionRef() == NULL || strlen(pDataset->GetProjectionRef()) <= 0)
		{
			OGRSpatialReference sr;
			sr.SetWellKnownGeogCS("WGS84");
			char* srWgs84Wtk = NULL;
			sr.exportToWkt(&srWgs84Wtk);
			pDataset->SetProjection(srWgs84Wtk);
		}

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

		GDALColorTable* colorTable = ColorRampUtils::getInstance()->createHSLColorRamp(startColor, endColor, 256);

		TileRendererOptions tileRendererOptions;

		tileRendererOptions.rendererType() = enumTileRendererType::StretchRenderer;
		tileRendererOptions.colorTable() = ColorRampUtils::getInstance()->convertColorRamp(*colorTable);
	
		tileRendererOptions.paletteBandIndex() = 1;

		double         min, max, mean, dev;
		GDALRasterBand * pRasterBand = pDataset->GetRasterBand(1);
		pRasterBand->ComputeStatistics(0, &min, &max, &mean, &dev, NULL, NULL);
		string minstr = QString::number(min).toStdString();
		string maxstr = QString::number(max).toStdString();
		if (minstr == "1.#INF00" || minstr == "-1.#INF00")
		{
			pRasterBand->SetNoDataValue(min);
		}
		if (maxstr == "1.#INF00" || maxstr == "-1.#INF00" || maxstr == "inf")
		{
			pRasterBand->SetNoDataValue(max);
		}
		
		double* ignoreValues = new double[4];
		ignoreValues[0] = 1.1754943508222875e-038;
		ignoreValues[1] = -999;
		//数据异常值情况
		ignoreValues[2] = -3.4028230607370965e+038;
		ignoreValues[3] = -498.68804931640625;
		pRasterBand->SetStatIngoreValues(&ignoreValues, 4);
		pRasterBand->ComputeStatistics(0, &min, &max, &mean, &dev, NULL, NULL);
		//pRasterBand->SetNoDataValue(-3.4028230607370965e+038); //new add
		tileRendererOptions.pixelMax() = max;
		tileRendererOptions.pixelMin() = min;
		tileRendererOptions.pixelMean() = mean;
		tileRendererOptions.pixelDev() = dev;

		GDALOptions rasterGDALOption;
		rasterGDALOption.tileRendererOptions() = tileRendererOptions;
		rasterGDALOption.externalDataset() = new GDALOptions::ExternalDataset(pDataset, true);
		rasterGDALOption.url() = path.toStdString().c_str();


		pDataset->GetGeoTransform(dGeotransform);

		OERSProductProperties m_Properties;
		m_Properties.userDefinedVPB = false;
		m_Properties.Imagehdfspath = path.toStdString();
		m_Properties.LayerName = FileName.toStdString();
		m_Properties.pixelMax = max;
		m_Properties.pixelMin = min;
		m_Properties.pixelMean = mean;
		m_Properties.pixelDev = dev;
		m_Properties.Resolution = abs(dGeotransform[1]);
		ImageLayerOptions rasterImageLayerOptions(FileName.toStdString(), rasterGDALOption);
		rasterImageLayerOptions.cachePolicy() = CachePolicy::NO_CACHE;
		rasterImageLayerOptions.guid() = ReferenceTheInstances::GuidMax + 1;
		ReferenceTheInstances::GuidMax++;
		ImageLayer* rasterImageLayer = new ImageLayer(rasterImageLayerOptions);

		OERSProductLayer* productLayer = new OERSProductLayer(rasterImageLayer, m_Properties);
		std::vector<OERSProductLayer> addLayers;
		addLayers.push_back(*productLayer);
		OERSProductLayerUtils::AddProductLayers(addLayers, true);
		ViewControlUtils::ZoomToLayer(addLayers.at(addLayers.size() - 1).GetLayer());
	}

}
void OERSMainWindow::addLoadLocalExcel()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Local data In-situ"))
		return;
	LoadLocalExcelDialog* lled = new LoadLocalExcelDialog();
	lled->exec();
}


void OERSMainWindow::ColorbarSettingDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
	contextLayer = ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer();
	if (contextLayer!=NULL)
	{		

		ImageLayer* imageLayer = (ImageLayer*)contextLayer;
		RasterRenderDialog* rrd = new RasterRenderDialog(imageLayer, _mapNode, _colorRampBox);
		rrd->exec();
	}
}
void OERSMainWindow::showRasterLightnessWidgetByToolbar()
{
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
	contextLayer = ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;
	showRasterLightnessWidget(imageLayer);
}

void OERSMainWindow::showRasterContrastWidgetByToolbar()
{
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
	contextLayer = ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;
	showRasterContrastWidget(imageLayer);
}

void OERSMainWindow::showRasterAlphaWidgetByToolbar()
{
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
	contextLayer = ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;

	showRasterAlphapWidget(imageLayer);
}

void OERSMainWindow::doColorRamp(bool checked)
{
	if (checked)
	{
		RasterUtils::CreateOrUpdateColorRamp();
	}

	_colorRampBox->setNodeMask(checked?~0:0);
}

void OERSMainWindow::doSymbolRender(bool checked)
{
	if (checked)
	{
		_vlf = new VectorLayerFrame(ReferenceTheInstances::ViewerWidgetPtr);
		_vlf->move(ReferenceTheInstances::ViewerWidgetPtr->x(),ReferenceTheInstances::ViewerWidgetPtr->y()+19);
		_vlf->show();
	}
	else
	{
		if (_vlf)
		{
			_vlf->close();
		}
	}
	
}


void OERSMainWindow::viewGloable()
{
	_viewer->getCameraManipulator()->home(2.0);
}

void OERSMainWindow::earthRotateCW(bool checked)
{
	if(checked)
	{
		if(_tbRotateCCW->isChecked())
		{
			//_viewer->removeEventHandler(_earthRotateHandler);
			_tbRotateCCW->setChecked(false);
		}
		_earthRotateHandler = new EarthRotateHandler(-0.002,0);
		_viewer->addEventHandler(_earthRotateHandler);
	}
	else
	{
		if(_earthRotateHandler)
			_viewer->removeEventHandler(_earthRotateHandler);
	}
}

void OERSMainWindow::earthRotateCCW(bool checked)
{
	if(checked)
	{
		if(_tbRotateCW->isChecked())
		{
			//_viewer->removeEventHandler(_earthRotateHandler);
			_tbRotateCW->setChecked(false);
		}
		_earthRotateHandler = new EarthRotateHandler(0.002,0);
		_viewer->addEventHandler(_earthRotateHandler);
	}
	else
	{
		if(_earthRotateHandler)
			_viewer->removeEventHandler(_earthRotateHandler);
	}
	//osgDB::writeNodeFile( *(ReferenceTheInstances::MapNodePtr),"OERSEARTH.earth");
}


void OERSMainWindow::doFlyMode()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	switch(QMessageBox::question(this,tr("Tips"),tr("Please design the flight path first!"),QMessageBox::Ok|QMessageBox::Cancel))
	{
	case QMessageBox::Ok:
		{
			_myPathLineEventHandler = new MyPathLineEventHandler(_mapNode,_root);
			_viewer->addEventHandler(_myPathLineEventHandler);
			ReferenceTheInstances::FlyModeDialog = new FlyModeDialog(ReferenceTheInstances::OERSEarthManipulatorPtr,_myPathLineEventHandler->_pathLine);
			connect(ReferenceTheInstances::FlyModeDialog,SIGNAL(startFly()),this,SLOT(removeFlyMode()));
			connect(ReferenceTheInstances::FlyModeDialog,SIGNAL(accepted()),this,SLOT(removeFlyMode()));
			connect(ReferenceTheInstances::FlyModeDialog,SIGNAL(rejected()),this,SLOT(removeFlyMode()));
		}
	default:break;
	};
	return;
}

void OERSMainWindow::removeFlyMode()
{
	_viewer->removeEventHandler(_myPathLineEventHandler);
}

//点测量
void OERSMainWindow::doPointMeasure(bool state)
{
	if(state&&_pointMeasureHandler==NULL)
	{
		_pointMeasureHandler = new PointMeasureHandler();
		this->_viewer->addEventHandler(_pointMeasureHandler);
	}
	else if(!state&&_pointMeasureHandler!=NULL)
	{
		this->_viewer->removeEventHandler(_pointMeasureHandler);
		_pointMeasureHandler = NULL;
	}
}

void OERSMainWindow::doLineMeasure(bool state)
{
	if(state&&_lineMeasureToolHandler==NULL)
	{
		_lineMeasureToolHandler = new MeasureToolHandler(_root,_mapNode);
		ReferenceTheInstances::OERSEarthManipulatorPtr->isUserDefinedCursor(true,"images/MeasureCursor.png");
		_lineMeasureToolHandler->setIsPath(true);
		//measureTool->setIntersectionMask( 0x1 );
		_lineMeasureToolHandler->addEventHandler( new LineMeasureToolCallback() );
		Style style = _lineMeasureToolHandler->getLineStyle();
		style.getOrCreate<LineSymbol>()->stroke()->color() = Color::White;
		style.getOrCreate<LineSymbol>()->stroke()->width() = 2.0f;
		style.getOrCreate<LineSymbol>()->tessellation() = 20;
		style.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		style.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;

		_lineMeasureToolHandler->setLineStyle(style);
		_viewer->addEventHandler( _lineMeasureToolHandler );
	}
	else if(!state&&_lineMeasureToolHandler!=NULL)
	{
		this->_viewer->removeEventHandler(_lineMeasureToolHandler);
		ReferenceTheInstances::OERSEarthManipulatorPtr->isUserDefinedCursor(false);
		_lineMeasureToolHandler = NULL;
	}
}

void OERSMainWindow::doPolygonMeasure(bool state)
{
	if(state&&_polygonMeasureHandler==NULL)
	{
		_polygonMeasureHandler = new PolygonMeasureHandler();
		_viewer->addEventHandler(_polygonMeasureHandler);
	}
	else if(!state&&_polygonMeasureHandler!=NULL)
	{
		this->_viewer->removeEventHandler(_polygonMeasureHandler);
		_polygonMeasureHandler = NULL;
	}
}

void OERSMainWindow::refreshScene()
{
	_mapNode->getTerrainEngine()->update();
}
	
void OERSMainWindow::showWindowFullScreen()
{
		
	if(this->isFullScreen())
	{
		//ReferenceTheInstances::StatusBarPtr->show();
		ReferenceTheInstances::FunctionToolBarPtr->show();
		ReferenceTheInstances::CatalogDockWidgetPtr->show();
		ReferenceTheInstances::ComboboxDockWidgetPtr->show();
		ReferenceTheInstances::OutputDockWidgetPtr->show();
		//ReferenceTheInstances::mapFramePtr->hide();
		this->showMaximized();
	}
	else
	{
		//ReferenceTheInstances::StatusBarPtr->hide();
		ReferenceTheInstances::FunctionToolBarPtr->hide();
		ReferenceTheInstances::CatalogDockWidgetPtr->hide();
		ReferenceTheInstances::ComboboxDockWidgetPtr->hide();
		ReferenceTheInstances::OutputDockWidgetPtr->hide();
		//ReferenceTheInstances::mapFramePtr->show();
		this->showFullScreen();
	}
}

void OERSMainWindow::showRSPixelQueryDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count() == 0)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
			return;
		}

		if (!SystemSettingUtils::putUseFunction("Single image Point"))
			return;

		RSPixelQueryDialog* rspqd = new RSPixelQueryDialog(_mapNode, _viewer);
		rspqd->show();
		ReferenceTheInstances::showOneDialog = true;
	}


}

void OERSMainWindow::showLineStatisticDialog()
{	
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}

	QMessageBox::information(this, tr("Tips"), tr("Please draw lines on the earth."), QMessageBox::Ok);
	OERSProductLayer* proLayer = OERSProductLayerUtils::getProductLayerByImageLayer(ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer());
	
	if (!SystemSettingUtils::putUseFunction("Single image Line"))
		return;

	LineStatisticsDialog* lsd = new LineStatisticsDialog(proLayer, _mapNode, _viewer, _statusLabel);
	
}

void OERSMainWindow::showRectangleStatisticDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count() == 0)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
			return;
		}
		vector<OERSProductLayer> oersProductLayerList;
		for (int i = 0; i<ReferenceTheInstances::GlobeProductLayer->size(); i++)
		{
			OERSProductLayer oersProductLayer = ReferenceTheInstances::GlobeProductLayer->at(i);
			oersProductLayerList.push_back(oersProductLayer);
		}

		if (oersProductLayerList.size()>0)
		{
			if (!SystemSettingUtils::putUseFunction("Single image Rectangle"))
				return;

			RectangleStatisticDialog* psd = new RectangleStatisticDialog(oersProductLayerList, _mapNode, _viewer, _root);
			psd->show();
			ReferenceTheInstances::showOneDialog = true;
		}
	}


}

void OERSMainWindow::showPolygonStatisticDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count() == 0)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
			return;
		}
		vector<OERSProductLayer> oersProductLayerList;

		for (int i = 0; i<ReferenceTheInstances::GlobeProductLayer->size(); i++)
		{
			OERSProductLayer oersProductLayer = ReferenceTheInstances::GlobeProductLayer->at(i);
			oersProductLayerList.push_back(oersProductLayer);
		}

		if (oersProductLayerList.size()>0)
		{
			if (!SystemSettingUtils::putUseFunction("Single image Polygon"))
				return;

			PolygonStatisticDialog* psd = new PolygonStatisticDialog(oersProductLayerList, _mapNode, _viewer, _root);
			psd->show();
			ReferenceTheInstances::showOneDialog = true;
		}
	}


}

void OERSMainWindow::showPointSeriesStatisticsChartDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (!SystemSettingUtils::putUseFunction("Time-series Point"))
			return;

		TimeSeriesDataQueryDialog* tsdqd = new TimeSeriesDataQueryDialog(0);
		tsdqd->show();
		ReferenceTheInstances::showOneDialog = true;
	}
}

void OERSMainWindow::showLineSeriesStatisticsChartDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (!SystemSettingUtils::putUseFunction("Time-series Line"))
			return;

		TimeSeriesDataQueryDialog* tsdqd = new TimeSeriesDataQueryDialog(1);
		tsdqd->show();
		ReferenceTheInstances::showOneDialog = true;
	}
}

void OERSMainWindow::showPolygonSeriesStatisticsChartDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (!SystemSettingUtils::putUseFunction("Time-series Polygon"))
			return;

		TimeSeriesDataQueryDialog* tsdqd = new TimeSeriesDataQueryDialog(2);
		tsdqd->show();
		ReferenceTheInstances::showOneDialog = true;
	}
}

void OERSMainWindow::showRasterProductBroadcastDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (!SystemSettingUtils::putUseFunction("Animation Images"))
			return;

		RasterProductBroadcastDialog* rpbd = new RasterProductBroadcastDialog(this->_mapNode, _viewer, _root);
		rpbd->show();
		ReferenceTheInstances::showOneDialog = true;
	}

}

void OERSMainWindow::showWindDynamicSimulationDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (!SystemSettingUtils::putUseFunction("Animation Vector data"))
			return;

		DynamicSimulationDialog* dsd = new DynamicSimulationDialog(_viewer);
		dsd->show();
		ReferenceTheInstances::showOneDialog = true;
	}
}

void OERSMainWindow::showRasterCalculatorDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (!SystemSettingUtils::putUseFunction("Formula editor"))
		return;

	RasterCalculatorDialog* rcd = new RasterCalculatorDialog();
	rcd->exec();
}

void OERSMainWindow::doRasterGrid()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count() > 0)
	{
		if (!SystemSettingUtils::putUseFunction("Resample"))
			return;

		RasterGridDialog *rgd = new RasterGridDialog();
		rgd->exec();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
}

void OERSMainWindow::doInsituGrid()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::GlobeSituLayer->size()!= 0 )
	{
		if (!SystemSettingUtils::putUseFunction("Gridding"))
			return;

		InsituGridDialog *igd = new InsituGridDialog();
		igd->exec();
	}
	else
	{
		QMessageBox::warning(this,tr("Warning"),tr("Please add in-situ data first."));
		return;
	}
}


void OERSMainWindow::showRasterDataFilterDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()>0)
	{
		if (!SystemSettingUtils::putUseFunction("Data Filter"))
			return;

		RasterDataFilterDialog* rdfd = new RasterDataFilterDialog();
		rdfd->exec();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
}

void OERSMainWindow::showROIExtractDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()>0)
	{
		if (!SystemSettingUtils::putUseFunction("ROI Extraction"))
			return;

		ROIExtractDialog* roied = new ROIExtractDialog();
		roied->exec();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
}

void OERSMainWindow::showRasterClassificationDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()>0)
	{
		if (!SystemSettingUtils::putUseFunction("Classification"))
			return;

		RasterClassificationDialog* rcd = new RasterClassificationDialog();
		rcd->exec();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
}
void OERSMainWindow::showMapPrintDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count() > 0)
	{
		if (OERSProductLayerUtils::getProductLayerByImageLayer(ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer())->GetProperties().userDefinedVPB)
		{
			if (!SystemSettingUtils::putUseFunction("Map Print"))
				return;

			MapPrintDialog* mpd = new MapPrintDialog();
			mpd->exec();
		}
		else
		{
			QMessageBox::warning(this, tr("Warning"), tr("Please use online image data."));
			return;
		}

	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;

	}

}
void OERSMainWindow::showRSDataAccuracyEvaluateDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::GlobeSituLayer->size()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add in-situ data first."));
	} 
	else
	{
		if (!SystemSettingUtils::putUseFunction("Satellite-insitu"))
			return;

		SatelliteinsituComparisonDialog* sicd = new SatelliteinsituComparisonDialog();
		sicd->exec();
	}
}

void OERSMainWindow::showRSDataCompareEvaluateDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	vector<string> layerNames;
	if (ReferenceTheInstances::GlobeProductLayer->size()>0)
	{
		for (int i=0;i<ReferenceTheInstances::GlobeProductLayer->size();i++)
		{
			layerNames.push_back(ReferenceTheInstances::GlobeProductLayer->at(i).GetProperties().LayerName);
		}

		if (!SystemSettingUtils::putUseFunction("Satellite-satellite"))
			return;

		RSCompareDataDialog* rscdd = new RSCompareDataDialog(layerNames);
		rscdd->exec();
	} 
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
	}
}

void OERSMainWindow::showRSFluxDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Air-sea CO2 flux Satellite"))
		return;

	RemoteSensingFluxDialog* rsfd = new RemoteSensingFluxDialog();
	rsfd->exec();
}

void OERSMainWindow::showUDRSFluxDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()>0)
	{
		if (!SystemSettingUtils::putUseFunction("Air-sea CO2 flux User-defined"))
			return;

		UDRemoteSensingFluxDialog* udrsfd = new UDRemoteSensingFluxDialog();
		udrsfd->exec();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
	}

}

void OERSMainWindow::showMeasurementFluxDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::GlobeSituLayer->size()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add in-situ data first."));
	} 
	else
	{
		if (!SystemSettingUtils::putUseFunction("Air-sea CO2 flux In-situ"))
			return;

		VectorFluxCalculateDialog* vfcd = new VectorFluxCalculateDialog();
		vfcd->exec();
	}
}


void OERSMainWindow::doUserDefinedRectArea(bool isChecked)
{
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
	if(isChecked)
	{
		OERSProductLayer* oersLayer = OERSProductLayerUtils::getProductLayerByImageLayer(ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer());
		try
		{
			if (oersLayer)
			{
				if (!SystemSettingUtils::putUseFunction("Carbon budget Rectangle"))
					return;

				_rmth = new RectMeasureToolHandler(oersLayer,_root,_mapNode);
				ReferenceTheInstances::ViewerPtr->addEventHandler(_rmth);
				ReferenceTheInstances::StatusLabelPtr->setText(tr("Please draw the rectangle"));			
			}
		}
		catch(...)
		{}

	}
	else
	{
		if(_rmth!=NULL)
		{
			ReferenceTheInstances::ViewerPtr->removeEventHandler(_rmth);
			_rmth = NULL;
			ReferenceTheInstances::StatusLabelPtr->setText("");
		}
	}
}

void OERSMainWindow::doUserDefinedArea(bool isChecked)
{
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
	if(isChecked)
	{
		OERSProductLayer* oersLayer = OERSProductLayerUtils::getProductLayerByImageLayer(ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer());
		try
		{
			if (oersLayer)
			{
				if (!SystemSettingUtils::putUseFunction("Carbon budget Polygon"))
					return;

				_esfieh = new UserDefinedFluxIntegralEventHandler(oersLayer);
				ReferenceTheInstances::ViewerPtr->addEventHandler(_esfieh);
				ReferenceTheInstances::StatusLabelPtr->setText(tr("Please draw the rectangle."));
			}
		}
		catch(...){}
	}
	else
	{
		if(_esfieh!=NULL)
		{
			ReferenceTheInstances::ViewerPtr->removeEventHandler(_esfieh);
			_esfieh = NULL;
			ReferenceTheInstances::StatusLabelPtr->setText("");
		}
	}
		
}

void OERSMainWindow::showSeaFluxDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	if (ReferenceTheInstances::ComBoxGlobeLayerPtr->count()==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please add image data first."));
		return;
	}
	else
	{
		if (!SystemSettingUtils::putUseFunction("Carbon budget Import region"))
			return;

		DefaultAreaSeaFluxDialog* sfd = new DefaultAreaSeaFluxDialog(OERSProductLayerUtils::getProductLayerByImageLayer(ReferenceTheInstances::ComBoxGlobeLayerPtr->getCurActiveLayer()));
		sfd->exec();
	}
}

//近海水质
void OERSMainWindow::showOffshoreWaterQualityDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Offshore Water Quality"))
		return;

	OffshoreWaterQualityDialog* owqd = new OffshoreWaterQualityDialog();
	owqd->exec();
}

//海湾水质
void OERSMainWindow::showGulfWaterQualityDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Gulf Water Quality"))
		return;

	GulfWaterQualityDialog* gwqd = new GulfWaterQualityDialog();
	gwqd->exec();
}

//城市水质
void OERSMainWindow::showUrbanWaterQualityDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Urban Water Quality"))
		return;

	UrbanWaterQualityDialog* uwqd = new UrbanWaterQualityDialog();
	uwqd->exec();
}

//赤潮Tiff
void OERSMainWindow::showRedTideTiffDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Red Tide Tiff"))
		return;

	RedTideTiffDialog* rttd = new RedTideTiffDialog();
	rttd->exec();
}

//温排水
void OERSMainWindow::showWarmDischargeDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Warm Discharge"))
		return;

	WarmDischargeDialog* wdd = new WarmDischargeDialog();
	wdd->exec();

}

//红树林
void OERSMainWindow::showMangroveForestDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Mangrove Forest"))
		return;

	MangroveForestDialog* mfd = new MangroveForestDialog();
	mfd->exec();

}

//海草床
void OERSMainWindow::showSeaweedBedDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Seaweed Bed"))
		return;

	SeaweedBedDialog* swbd = new SeaweedBedDialog();
	swbd->exec();

}

//珊瑚礁
void OERSMainWindow::showCoralReefDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Coral Reef"))
		return;

	CoralReefDialog* crd = new CoralReefDialog();
	crd->exec();
	 
}

//气溶胶
void OERSMainWindow::showAerosolDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Aerosol"))
		return;

	AerosolDialog* aerd = new AerosolDialog();
	aerd->exec();

}

//陆源入海CO2
void OERSMainWindow::showChangjiangFluxDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("CJ River and ECS"))
		return;

	ChangjiangFluxDialog* cfd = new ChangjiangFluxDialog(_mapNode, _viewer);
	cfd->show();
	ReferenceTheInstances::showOneDialog = true;
}

//其他流域通量
void OERSMainWindow::showOtherFluxDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Other Basins"))
		return;

	OtherFluxDialog* ofd = new OtherFluxDialog();
	ofd->exec();
}

//处理GOCI出图制作
void OERSMainWindow::showGOCIProcessDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("GOCI Process"))
		return;

	GOCIProcessDialog* gpd = new GOCIProcessDialog();
	gpd->exec();
}

//处理GF出图制作
void OERSMainWindow::showGFProcessDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("GF Process"))
		return;

	GFProcessDialog* gfpd = new GFProcessDialog();
	gfpd->exec();
}

//处理HY1C出图制作
void OERSMainWindow::showHY1CProcessDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("HY1C Process"))
		return;

	HY1CProcessDialog* hypd = new HY1CProcessDialog();
	hypd->exec();
}

//处理Landsat出图制作
void OERSMainWindow::showLandsatProcessDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("Landsat Process"))
		return;

	LANDSATProcessDialog* ldstpd = new LANDSATProcessDialog();
	ldstpd->exec();
}

//加载GML TIFF数据
void OERSMainWindow::loadGMLTiffData()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("GML Tiff"))
		return;
	QString path = QFileDialog::getOpenFileName(this, tr("Local raster data"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("Images (*.tif);"));
	string pathOK = path.toLocal8Bit().toStdString();
	path = QString::fromLocal8Bit(pathOK.data());
	if (path == "")
	{
		return;
	}
	QStringList list = path.split("/");
	QString FileName = list.at(list.length() - 1);
	if (path.toStdString() == "")
		return;
	if (path.endsWith(".tif") || path.endsWith(".img"))
	{
		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

		//计算
		//RasterUtils::DeleteNanValueFromImage(path);

		GDALDataset* pDataset = (GDALDataset *)GDALOpen(path.toStdString().c_str(), GA_ReadOnly);

		double *dGeotransform = new double[6];
		if (pDataset->GetGeoTransform(dGeotransform))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Lack of coordinates!"), tr("Yes"));
			return;
		}

		if (pDataset->GetProjectionRef() == NULL || strlen(pDataset->GetProjectionRef()) <= 0)
		{
			OGRSpatialReference sr;
			sr.SetWellKnownGeogCS("WGS84");
			char* srWgs84Wtk = NULL;
			sr.exportToWkt(&srWgs84Wtk);
			pDataset->SetProjection(srWgs84Wtk);
		}

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

		GDALColorTable* colorTable = ColorRampUtils::getInstance()->createHSLColorRamp(startColor, endColor, 256);

		TileRendererOptions tileRendererOptions;

		tileRendererOptions.rendererType() = enumTileRendererType::StretchRenderer;
		tileRendererOptions.colorTable() = ColorRampUtils::getInstance()->convertColorRamp(*colorTable);

		tileRendererOptions.paletteBandIndex() = 1;

		double         min, max, mean, dev;
		GDALRasterBand * pRasterBand = pDataset->GetRasterBand(1);
		pRasterBand->ComputeStatistics(0, &min, &max, &mean, &dev, NULL, NULL);
		string minstr = QString::number(min).toStdString();
		string maxstr = QString::number(max).toStdString();
		if (minstr == "1.#INF00" || minstr == "-1.#INF00")
		{
			pRasterBand->SetNoDataValue(min);
		}
		if (maxstr == "1.#INF00" || maxstr == "-1.#INF00" || maxstr == "inf")
		{
			pRasterBand->SetNoDataValue(max);
		}

		//pRasterBand->SetNoDataValue(-9999);
		pRasterBand->SetNoDataValue(-999);

		double* ignoreValues = new double[2];
		ignoreValues[0] = 1.1754943508222875e-038;
		ignoreValues[1] = -999;
		//数据异常值情况
		//ignoreValues[2] = -3.4028230607370965e+038;
		//ignoreValues[3] = -498.68804931640625;
		pRasterBand->SetStatIngoreValues(&ignoreValues, 2);
		pRasterBand->ComputeStatistics(0, &min, &max, &mean, &dev, NULL, NULL);
		//pRasterBand->SetNoDataValue(-3.4028230607370965e+038); //new add
		tileRendererOptions.pixelMax() = max;
		tileRendererOptions.pixelMin() = min;
		tileRendererOptions.pixelMean() = mean;
		tileRendererOptions.pixelDev() = dev;

		GDALOptions rasterGDALOption;
		rasterGDALOption.tileRendererOptions() = tileRendererOptions;
		rasterGDALOption.externalDataset() = new GDALOptions::ExternalDataset(pDataset, true);
		rasterGDALOption.url() = path.toStdString().c_str();


		pDataset->GetGeoTransform(dGeotransform);

		OERSProductProperties m_Properties;
		m_Properties.userDefinedVPB = false;
		m_Properties.Imagehdfspath = path.toStdString();
		m_Properties.LayerName = FileName.toStdString();
		m_Properties.pixelMax = max;
		m_Properties.pixelMin = min;
		m_Properties.pixelMean = mean;
		m_Properties.pixelDev = dev;
		m_Properties.Resolution = abs(dGeotransform[1]);
		ImageLayerOptions rasterImageLayerOptions(FileName.toStdString(), rasterGDALOption);
		rasterImageLayerOptions.cachePolicy() = CachePolicy::NO_CACHE;
		rasterImageLayerOptions.guid() = ReferenceTheInstances::GuidMax + 1;
		ReferenceTheInstances::GuidMax++;
		ImageLayer* rasterImageLayer = new ImageLayer(rasterImageLayerOptions);

		OERSProductLayer* productLayer = new OERSProductLayer(rasterImageLayer, m_Properties);
		std::vector<OERSProductLayer> addLayers;
		addLayers.push_back(*productLayer);
		OERSProductLayerUtils::AddProductLayers(addLayers, true);
		ViewControlUtils::ZoomToLayer(addLayers.at(addLayers.size() - 1).GetLayer());
	}

}

//水质分类
void OERSMainWindow::showWaterClassDialog()
{

	if (!ReferenceTheInstances::showOneDialog)
	{
		if (!SystemSettingUtils::putUseFunction("Water Class"))
			return;

		GMLRasterProductQueryDialog* gmlrpqd = new GMLRasterProductQueryDialog("WaterClass");
		gmlrpqd->show();
		ReferenceTheInstances::showOneDialog = true;
	}

}

//水质优良率
void OERSMainWindow::showWaterQualityRate()
{

}

//富营养化
void OERSMainWindow::showEutrophication()
{

}

void OERSMainWindow::showFishersEnvImpact()
{

}

void OERSMainWindow::showRiverNetworkQuality()
{

}

void OERSMainWindow::showOutfallQuery()
{

}

//自动播放
void OERSMainWindow::showAutoRasterProductBroadcastPlayDialog()
{
	if (!ReferenceTheInstances::showOneDialog)
	{
		if (!SystemSettingUtils::putUseFunction("Auto Broadcast"))
			return;

		AutoRasterProductBroadcastPlayDialog* arpbpd = new AutoRasterProductBroadcastPlayDialog(_mapNode, _viewer, _root);
		arpbpd->show();
		ReferenceTheInstances::showOneDialog = true;
	}
}

//加载shp文件
void OERSMainWindow::addShapeFileData()
{

	//QString path = "C:/Users/PC/Desktop/coral_southchinasea/coral_southchinasea.shp";
	QString path = QFileDialog::getOpenFileName(this, tr("Local raster data"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("*.shp"));
	string pathOK = path.toLocal8Bit().toStdString();
	path = QString::fromLocal8Bit(pathOK.data());

	QStringList list = path.split("/");
	QString FileName = list.at(list.length() - 1);

	{
		OGRDataSource* poDS = OGRSFDriverRegistrar::Open(path.toStdString().c_str());
		if (!poDS)
			return;
		OGRLayer* layer = poDS->GetLayer(0);
		if (!layer)
		{
			OGR_DS_Destroy(poDS);
			return;
		}
		OGRwkbGeometryType type = layer->GetGeomType();

		Style style;
		if (type == OGRwkbGeometryType::wkbPoint || type == OGRwkbGeometryType::wkbPoint25D || type == OGRwkbGeometryType::wkbMultiPoint || type == OGRwkbGeometryType::wkbMultiPoint25D)
		{

			PointSymbol* pointSymbol = style.getOrCreateSymbol<PointSymbol>();
			pointSymbol->size() = 50.0f;
			pointSymbol->fill()->color() = Color::Yellow;
			//pointSymbol->isEfficient() = true; 
		}
		else if (type == OGRwkbGeometryType::wkbLineString || type == OGRwkbGeometryType::wkbLineString25D || type == OGRwkbGeometryType::wkbMultiLineString || type == OGRwkbGeometryType::wkbMultiLineString25D)
		{
			LineSymbol* lineSymbol = style.getOrCreateSymbol<LineSymbol>();
			//ls->stroke()->widthUnits() = osgEarth::Util::Controls::Units::PIXELS;
			lineSymbol->stroke()->color() = Color::Yellow;
			lineSymbol->stroke()->width() = 5.0f;
			//lineSymbol->stroke()->stipple() = 0xFF;
			//lineSymbol->isWind() = true;
		}
		else if (type == OGRwkbGeometryType::wkbPolygon || type == OGRwkbGeometryType::wkbPolygon25D || type == OGRwkbGeometryType::wkbMultiPolygon || type == OGRwkbGeometryType::wkbMultiPolygon25D)
		{
			PolygonSymbol* polygonSymbol = style.getOrCreateSymbol<PolygonSymbol>();
			polygonSymbol->fill()->color() = Color(1.0, 0.0, 0.0, 1.0);  //shp颜色
			//polygonSymbol->isEfficient() = true;

			/*LineSymbol* lineSymbol = style.getOrCreateSymbol<LineSymbol>();
			lineSymbol->stroke()->color() = Color::Yellow;
			lineSymbol->stroke()->width() = 2.0f;
			lineSymbol->isWind() = true;*/
			//lineSymbol->tessellation() = 10;
		}
		else
		{
			OGR_DS_Destroy(poDS);
			cout << "Data format error" << endl;
			return;
		}

		//ls->tessellation() = 10;
		//叠合到地形上，否者会遮挡
		style.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		style.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;//也可以用其他方式
		//style.getOrCreate<RenderSymbol>()->depthOffset() = 20;
		//style.getOrCreate<ExtrusionSymbol>()->height() = 20.0; // meters MSL
		OGRFeatureOptions featureOptions;
		featureOptions.name() = path.toStdString();
		featureOptions.url() = path.toStdString();
		FeatureGeomModelOptions featureGeomModelOptions;
		featureGeomModelOptions.featureOptions() = featureOptions;
		featureGeomModelOptions.styles() = new StyleSheet();
		featureGeomModelOptions.styles()->addStyle(style);
		featureGeomModelOptions.enableLighting() = false;
		FeatureSourceIndexOptions fsio;
		//fsio.embedFeatures() = true;
		featureGeomModelOptions.featureIndexing() = fsio;


		ModelLayerOptions modelLayerOption(path.toStdString(), featureGeomModelOptions);
		ModelLayer* modelLayer = new ModelLayer(FileName.toStdString(), modelLayerOption);

		OGR_DS_Destroy(poDS);

		/*WaitingProgressDialog* waitProgressDialog = new WaitingProgressDialog(this->parentWidget());*/
		ReferenceTheInstances::WaitingProgressDialogPtr->show();
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		QApplication::processEvents();
		/*waitProgressDialog->show();*/
		AddVectorLayerThread mythread(modelLayer);
		mythread.start();
		QEventLoop eventLoop;
		connect(&mythread, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		connect(&mythread, SIGNAL(finished()), &eventLoop, SLOT(accept()));
		eventLoop.exec();
		mythread.quit();
		ReferenceTheInstances::WaitingProgressDialogPtr->hide();
		QApplication::restoreOverrideCursor();
		ViewControlUtils::ZoomToLayer(modelLayer);
	}

}

void OERSMainWindow::doRedTideFind()
{
	if (ReferenceTheInstances::showOneDialog)
		return;

	if (!SystemSettingUtils::putUseFunction("HAB Detection"))
		return;

	RedTideDialog* rtd=new RedTideDialog();
	rtd->exec();
}

void OERSMainWindow::doProcessPOC()
{
	string cmd = "rasterCalc\\HAB\\HABgen.exe";
	system(cmd.c_str());
}

void OERSMainWindow::showUserInformationDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	UserInformationDialog* uid=new UserInformationDialog();
	uid->exec();
}

void OERSMainWindow::GetManual()
{
	QDesktopServices::openUrl(QUrl("http://www.satco2.com/achievement/25"));

}

void OERSMainWindow::doUpdateSoftware()
{
	string currentversion = SystemSettingUtils::getMapSettingByNodePath("/mapSetting/version");
	
	string versionid;
	string filepath;
	ReferenceTheInstances::db.open();
	QSqlQuery strQuery(ReferenceTheInstances::db);
	strQuery.exec("SELECT * FROM version");
	strQuery.next();
	versionid = strQuery.value(0).toString().toStdString();

	if (versionid == currentversion)
	{
		QMessageBox::information(this,tr("Tips"),tr("No updates available!"));
	} 
	else
	{
		int ret = QMessageBox::information(this, tr("Tips"), tr("A new version is available.\nClick ok to update."), QMessageBox::Ok, QMessageBox::Cancel);
		if (ret == QMessageBox::Ok)
		{
			QDesktopServices::openUrl(QUrl("http://www.satco2.com/achievement/25"));
		}

	}
		
}

void OERSMainWindow::showMapSourceDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	QAction *act = qobject_cast<QAction*>(sender());

	if(act->objectName().toStdString() == "tbMapSource")
	{
		MapSourceDialog* msd=new MapSourceDialog(0);
		msd->exec();
	}
	else
	{
		MapSourceDialog* msd=new MapSourceDialog(1);
		msd->exec();
	}
}
void OERSMainWindow::showAboutDialog()
{
	if (ReferenceTheInstances::showOneDialog)
		return;
	AboutDialog* ad=new AboutDialog();
	ad->show();
}

void OERSMainWindow::changelanguage()
{
	string setflag;
	if (ReferenceTheInstances::useChn == true)
	{		
		setflag = "FALSE";

	}
	else
	{
		//change to chn
		setflag = "TRUE";
	}


	QStringList nodePathList = QString::fromLocal8Bit("/mapSetting/useChn").split("/");
	QDomDocument doc("mydocument");
	QFile file("demo_data//mapSetting.xml");
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();

	QDomElement docElem = doc.documentElement();

	for (int index = 1; index < nodePathList.count(); index++)
	{
		while (!docElem.isNull())
		{
			if (docElem.tagName() == nodePathList[index])
			{
				if (index == nodePathList.count() - 1)
				{
					docElem.firstChild().setNodeValue(setflag.c_str());
					break;
				}
				else
				{
					docElem = docElem.firstChild().toElement();
					break;
				}

			}
			else
			{
				docElem = docElem.nextSibling().toElement();
			}

		}
	}

	file.open(QFile::WriteOnly | QFile::Truncate);
	QTextStream ts(&file);
	doc.save(ts, 4);
	file.close();
	
	QMessageBox::information(this, tr("Tips"), tr("Please restart."));
	return;

}

void OERSMainWindow::showPointVectorRenderDialog()
{
	string layerName = _layerCatalog->getTreeWidget()->selectedItems().at(0)->text(0).toLocal8Bit().toStdString();
	VectorRenderDialog* vrd = new VectorRenderDialog(layerName.c_str());
	vrd->exec();
}

void OERSMainWindow::showRasterRenderDialog()
{
	ImageLayer* imageLayer = dynamic_cast<ImageLayer*>(((LayerTreeItem*)(_layerCatalog->getTreeWidget()->selectedItems().at(0)))->getLayer());
	RasterRenderDialog* rrd = new RasterRenderDialog(imageLayer, _mapNode, _colorRampBox);
	rrd->exec();
}

void OERSMainWindow::doFrameRateUpdate()
{
	QString r1=tr("Frame rate: ");
	//QString result = r1+QString::number(_viewer->getFrameStamp()->getFrameNumber()-_lastFrame)+" fps    ";

	double distance = ReferenceTheInstances::OERSEarthManipulatorPtr->getDistance();
	QString altitudeStr=tr("Altitude: ");
	QString disString = altitudeStr+QString::number(distance,'f',2)+QString::fromLocal8Bit(" m");
	//result+=disString;

	_statusLabel->setText(disString);
	_lastFrame = _viewer->getFrameStamp()->getFrameNumber();
}

void OERSMainWindow::catalogItemDoubleClickedAction(QTreeWidgetItem* item, int col)
{
	if(item->childCount()>0||item->text(0).toLocal8Bit()=="实测图层"||item->text(0).toLocal8Bit()=="SituLayer")
	{
		return;
	}
	else if (item->parent()->text(0).toLocal8Bit()=="实测图层"||item->parent()->text(0).toLocal8Bit()=="SituLayer")
	{
		QString layerName=item->text(0);
		for (int i=0;i<ReferenceTheInstances::GlobeSituLayer->size();i++)
		{
			if (ReferenceTheInstances::GlobeSituLayer->at(i).GetLayerName()==layerName.toLocal8Bit().toStdString())
			{
				OERSSituProperties pProperties=ReferenceTheInstances::GlobeSituLayer->at(i).GetProperties();
				if (pProperties.prodLonMin*pProperties.prodLonMax<0)
				{
					ViewControlUtils::ZoomToLayer(pProperties.prodLonMax,pProperties.prodLonMin,pProperties.prodLatMin,pProperties.prodLatMax);
				} 
				else
				{
					ViewControlUtils::ZoomToLayer(pProperties.prodLonMin,pProperties.prodLonMax,pProperties.prodLatMin,pProperties.prodLatMax);
				}
				return;
			}
		}
	}
	else if((LayerTreeItem*)item)
	{
		Layer* layer = ((LayerTreeItem*)item)->getLayer();
		ViewControlUtils::ZoomToLayer(layer);
	}

}


void OERSMainWindow::itemMenuShow(QPoint point)
{
	QList<QTreeWidgetItem*> itemList = _layerCatalog->getTreeWidget()->selectedItems();
	if(itemList.count()==0||itemList.at(0)->text(0).toLocal8Bit()=="实测图层"||itemList.at(0)->text(0).toLocal8Bit()=="SituLayer"||itemList.at(0)->text(0).toLocal8Bit()=="栅格图层"||itemList.at(0)->text(0).toLocal8Bit()=="Image Layers")
		_blankAreaMenu->exec(QCursor::pos());
	else if (itemList.at(0)->parent()->text(0).toLocal8Bit()=="实测图层"||itemList.at(0)->parent()->text(0).toLocal8Bit()=="SituLayer")
	{
		_pointVectorMenu->exec(QCursor::pos());
	}
	else if(itemList.count()==1)
	{
		if(itemList.at(0)->childCount()>0)
			return;
			//_fatherMenu->exec(QCursor::pos());
		else if((LayerTreeItem*)(itemList.at(0)))
		{
			Layer* layer = ((LayerTreeItem*)(itemList.at(0)))->getLayer();
			if(dynamic_cast<ImageLayer*>(layer))
			{
				ImageLayer* imageLayer = dynamic_cast<ImageLayer*>(layer);
				optional<osgEarth::TileSourceOptions> option = imageLayer->getImageLayerOptions().driver();
				if(imageLayer->getName() == "Base map" || imageLayer->getName() == "底图")
				{
					_globeTileMenu->exec(QCursor::pos());
				}
				else if (imageLayer->getName() == "Calculation results")
				{
					_resultMenu->exec(QCursor::pos());
				}
				else if(option->getDriver() == "gdal" || option->getDriver() == "vpb")
				{
					//_convertToImageLayerAction->setDisabled(true);
					//_convertToElevationLayerAction->setDisabled(false);
					_rasterMenu->exec(QCursor::pos());
				}
				else
				{
					_mapTileMenu->exec(QCursor::pos());
				}
			}
			else if (dynamic_cast<ElevationLayer*>(layer))
			{
				ElevationLayer* elevationLayer = dynamic_cast<ElevationLayer*>(layer);
				optional<osgEarth::TileSourceOptions> option = elevationLayer->getElevationLayerOptions().driver();
				if(option.get().getDriver()=="gdal")
				{
					_convertToElevationLayerAction->setDisabled(true);
					_convertToImageLayerAction->setDisabled(false);
					_rasterMenu->exec(QCursor::pos());
				}
			}
		}
	}
}

void OERSMainWindow::zoomToLayer()
{
	contextLayer = ((LayerTreeItem*)(_layerCatalog->getTreeWidget()->selectedItems().at(0)))->getLayer();
	ViewControlUtils::ZoomToLayer(contextLayer);
}



void OERSMainWindow::cdiacCheckChange(QTreeWidgetItem* item,int index)
{
	string layerName=item->text(0).toLocal8Bit().toStdString();
	string ayLayerName=layerName+"AY";
	if (item->checkState(0)==Qt::Checked)
	{
		for(int i=ReferenceTheInstances::SituGroup->situGroup->getNumChildren()-1;i>=0;i--)
		{
			osg::Node* node = ReferenceTheInstances::SituGroup->situGroup->getChild(i);
			if(node->getName()==layerName||node->getName()==ayLayerName)
			{
				node->setNodeMask(1);
			}
		}
	}
	else
	{
		for(int i=ReferenceTheInstances::SituGroup->situGroup->getNumChildren()-1;i>=0;i--)
		{
			osg::Node* node = ReferenceTheInstances::SituGroup->situGroup->getChild(i);
			if(node->getName()==layerName||node->getName()==ayLayerName)
			{
				node->setNodeMask(0);
			}
		}
	}
}

//下载栅格数据
void OERSMainWindow::downloadRasterProduct()
{
	contextLayer = ((LayerTreeItem*)(_layerCatalog->getTreeWidget()->selectedItems().at(0)))->getLayer();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;
	OERSProductLayer* currLayer=OERSProductLayerUtils::getProductLayerByImageLayer(imageLayer);

	if (!currLayer->GetProperties().userDefinedVPB)
	{
		string pszFile = currLayer->GetProperties().Imagehdfspath;
		QString path = QFileDialog::getSaveFileName(this, tr("Save as Image"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("TIF(*.tif)"));
		if (path.toStdString() == "")
			return;

		if (QFile::copy(pszFile.c_str(), path))
		{
			QMessageBox::warning(NULL, tr("Tips"), tr("Download completed!"));
		}
		else
		{
			QMessageBox::warning(NULL, tr("Tips"), tr("Download failed!"));
		}
		return;
	}

	string filePath=currLayer->GetProperties().Imagehdfspath;
	filePath=filePath.substr(16,filePath.length()-16);

	QString path=QFileDialog::getExistingDirectory(this,tr("Select the download path"),"/Desktop", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (path=="")
	{
		return;
	}
	std::vector<QString> downloadURL;
	downloadURL.push_back(QString::fromLocal8Bit(filePath.c_str()));
	DownloadProgressDialog* dpd=new DownloadProgressDialog(downloadURL,path);
	dpd->exec();
}

void OERSMainWindow::showRasterAlphaWidgetByToc()
{
	contextLayer = ((LayerTreeItem*)(_layerCatalog->getTreeWidget()->selectedItems().at(0)))->getLayer();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;

	showRasterAlphapWidget(imageLayer);
}

void OERSMainWindow::setLayerAlpha()
{
	QSlider* slider= (QSlider*)sender();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;
	ImageLayerUtils::getInstance()->setOpacity(imageLayer, slider->value() / 100.0f);
}

void OERSMainWindow::showRasterLightnessWidgetByToc()
{
	contextLayer = ((LayerTreeItem*)(_layerCatalog->getTreeWidget()->selectedItems().at(0)))->getLayer();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;
	showRasterLightnessWidget(imageLayer);
}

void OERSMainWindow::setLayerLightness()
{
	QSlider* slider= (QSlider*)sender();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;
	ImageLayerUtils::getInstance()->setBrightness(imageLayer,((float)slider->value()) / 100.0f + 1);
}

void OERSMainWindow::showRasterContrastWidgetByToc()
{
	contextLayer = ((LayerTreeItem*)(_layerCatalog->getTreeWidget()->selectedItems().at(0)))->getLayer();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;
	showRasterContrastWidget(imageLayer);
}

void OERSMainWindow::setLayerContrast()
{
	QSlider* slider= (QSlider*)sender();
	ImageLayer* imageLayer = (ImageLayer*)contextLayer;
	ImageLayerUtils::getInstance()->setContrast(imageLayer,slider->value() / 100.0f + 1);
}



void OERSMainWindow::removeCdiacLayer()
{
	string layerName = _layerCatalog->getTreeWidget()->selectedItems().at(0)->text(0).toLocal8Bit().toStdString();
	removeCdiacLayerByName(QString::fromLocal8Bit(layerName.c_str()));
}

void OERSMainWindow::removeLayer()
{
	contextLayer = ((LayerTreeItem*)(_layerCatalog->getTreeWidget()->selectedItems().at(0)))->getLayer();
	//WaitingProgressDialog* waitProgressDialog = new WaitingProgressDialog(this->parentWidget());
	//waitProgressDialog->show();
	vector<Layer*> layerVec;
	layerVec.push_back(contextLayer);
	RemoveLayerVectorThread mythread(layerVec);
	mythread.start(QThread::Priority::LowestPriority);
	//mythread.setStackSize(50*1000*1000);
	QEventLoop eventLoop;
	connect(&mythread,SIGNAL(finished()),&eventLoop,SLOT(quit()));
	//connect(&mythread,SIGNAL(finished()),waitProgressDialog,SLOT(accept()));
	eventLoop.exec();
	mythread.quit();
}

void OERSMainWindow::removeSelectLayersAction()
{
	QTreeWidget* tw = _layerCatalog->getTreeWidget();
	vector<Layer*> layerVec;
	vector<QString> cdiacLayerNames;
	QTreeWidgetItemIterator it(tw,QTreeWidgetItemIterator::Checked);
	QTreeWidgetItem* item;
	while (*it) {
		item=(QTreeWidgetItem*)*it;
		if (item->parent()->text(0).toLocal8Bit()!="实测图层"&&item->parent()->text(0).toLocal8Bit()!="SituLayer")
		{
			contextLayer = ((LayerTreeItem*)(*it))->getLayer();
			layerVec.push_back(contextLayer);
		}
		else
		{
			cdiacLayerNames.push_back(item->text(0));
		}
		++it;
	}


	for (int i=0;i<cdiacLayerNames.size();i++)
	{
		removeCdiacLayerByName(cdiacLayerNames.at(i));
	}

	RemoveLayerVectorThread mythread(layerVec);
	mythread.start(QThread::Priority::LowestPriority);
	//mythread.setStackSize(50*1000*1000);
	QEventLoop eventLoop;
	connect(&mythread,SIGNAL(finished()),&eventLoop,SLOT(quit()));
	//connect(&mythread,SIGNAL(finished()),waitProgressDialog,SLOT(accept()));
	eventLoop.exec();
	mythread.quit();
}

void OERSMainWindow::removeCdiacLayerByName(QString cdiacLayerName)
{
	string ayLayerName=cdiacLayerName.toLocal8Bit().toStdString()+"AY";
	for (int i=0;i<ReferenceTheInstances::modelsItem->childCount();i++)
	{
		QTreeWidgetItem* cdiacItem=ReferenceTheInstances::modelsItem->child(i);
		if (cdiacItem->text(0)==cdiacLayerName)
		{
			ReferenceTheInstances::modelsItem->removeChild(cdiacItem);
		}
	}
	for(int i=ReferenceTheInstances::SituGroup->situGroup->getNumChildren()-1;i>=0;i--)
	{
		osg::Node* node = ReferenceTheInstances::SituGroup->situGroup->getChild(i);
		if(node->getName()==cdiacLayerName.toLocal8Bit().toStdString()||node->getName()==ayLayerName)
		{
			ReferenceTheInstances::SituGroup->situGroup->removeChild(node);
		}
	}
	for(int i=0;i<ReferenceTheInstances::GlobeSituLayer->size();i++)
	{
		if(cdiacLayerName.toStdString() == ReferenceTheInstances::GlobeSituLayer->at(i).GetLayerName())
		{
			ReferenceTheInstances::GlobeSituLayer->erase(ReferenceTheInstances::GlobeSituLayer->begin()+i);
		}
	}
	if (ReferenceTheInstances::modelsItem->childCount()==0)
	{
		ReferenceTheInstances::modelsItem->setHidden(true);
	}
}

void OERSMainWindow::onLayerItemChanged(QTreeWidgetItem* item, int col)
{
	RasterUtils::CreateOrUpdateColorRamp();//刷新色标
}

void OERSMainWindow::setNavigationMode(bool checked)
{
	ReferenceTheInstances::OERSEarthManipulatorPtr->setFlyMode(checked);
} 

void OERSMainWindow::removeAllLayersAction()
{
	int layerNum;
	{
		int i = 0;
		layerNum = _mapNode->getMap()->getNumImageLayers();
		for(int j=0;j<layerNum;j++)
		{
			ImageLayer* imageLayer = _mapNode->getMap()->getImageLayerAt(i);
			if(imageLayer->getName() == "Base map" || imageLayer->getName() == "中国行政区划" || imageLayer->getName() == "底图")
			{
				i++;
			}
			else
			{
				GDALDataset* pDataset = ImageLayerUtils::getInstance()->getDataset(imageLayer);
				if (pDataset != NULL)
				{
					GDALClose(pDataset);
				}
				
				_mapNode->getMap()->removeImageLayer(imageLayer);
			}
		}
		if(ReferenceTheInstances::GlobeProductLayer!=NULL)
		{
			ReferenceTheInstances::GlobeProductLayer->clear();
		}
		
	}
	{
		layerNum = _mapNode->getMap()->getNumModelLayers();
		for(int j=0;j<layerNum;j++)
		{
			ModelLayer* modelLayer = _mapNode->getMap()->getModelLayerAt(0);
			_mapNode->getMap()->removeModelLayer(modelLayer);
		}
	}
	{
		layerNum = _mapNode->getMap()->getNumElevationLayers();
		for(int j=0;j<layerNum;j++)
		{
			ElevationLayer* eleLayer = _mapNode->getMap()->getElevationLayerAt(0);
			_mapNode->getMap()->removeElevationLayer(eleLayer);
		}
	}

	int itemNum = ReferenceTheInstances::modelsItem->childCount();
	for (int i=0;i<itemNum;i++)
	{
		QTreeWidgetItem* cdiacItem=ReferenceTheInstances::modelsItem->child(0);
		removeCdiacLayerByName(cdiacItem->text(0));
	}
}

void OERSMainWindow::moveRaster2Top()
{
	ImageLayer* imageLayer = dynamic_cast<ImageLayer*>(((LayerTreeItem*)(_layerCatalog->getTreeWidget()->selectedItems().at(0)))->getLayer());
	if (imageLayer->getName() == "Base map" || imageLayer->getName() == "底图")
	{
		return;
	}
	_mapNode->getMap()->moveImageLayer(imageLayer, _mapNode->getMap()->getNumImageLayers() - 1);
	ViewControlUtils::ZoomToLayer(imageLayer);
}


