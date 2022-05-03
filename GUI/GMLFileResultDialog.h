#ifndef GMLFILERESULTDIALOG_H
#define GMLFILERESULTDIALOG_H

#include "ui_GMLFileResultDialog.h"
#include <string>
#include <vector>
#include <osgEarthDrivers/feature_ogr/FeatureCursorOGR>
#include <osgEarth/MapNode>
#include "OERSProductLayer.h"

class GMLFileResultDialog :public QDialog, public Ui_GMLFileResultDialog
{
	Q_OBJECT
public:
	GMLFileResultDialog(std::vector<std::string> queryLayerNames, QWidget* parent = 0L);
	static OERSProductLayer* getProductLayerFromDB(string layerName);
	static bool getProductProperties(string layerName, OERSProductProperties& prodProp);

private:
	void initSettings();
	private slots:
	void backToUpper();
	void chooseAllOrNot(int state);
	void loadRasterLayers();
private:
	//MapNode* _mapnode;
	//osg::Camera* _hud;
};

#endif
