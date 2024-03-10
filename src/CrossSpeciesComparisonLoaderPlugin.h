#pragma once

#include <LoaderPlugin.h>
#include "InputDialogCSV.h"
#include "InputDialogJSON.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "CrossSpeciesComparisonTreeData.h"
using namespace mv::plugin;

// =============================================================================
// Loader
// =============================================================================
struct ClusterInput {
    std::string value1;
    std::string value2;
    std::string cluster;
};
class CrossSpeciesComparisonLoaderPlugin : public LoaderPlugin
{
    Q_OBJECT
public:
    CrossSpeciesComparisonLoaderPlugin(const PluginFactory* factory);
    ~CrossSpeciesComparisonLoaderPlugin(void) override;
    
    void init() override;

    void loadData() Q_DECL_OVERRIDE;
public slots:
    void dialogClosedCSV(QString dataSetName, QString TypeName);
    void dialogClosedJSON(QString dataSetName, QString TypeName);

private:
    QString _dataSetName;
    std::vector<QStringList> _loadedData;
    std::string checkTypeValue = "None";
    std::map<int, std::vector<float>> _numericColumnValues;
    std::map<int, std::vector<std::string>> _stringColumnStrings;
    std::map<int, std::string> _columnHeaderNameValues;

    std::vector<QString> _speciesNames;
    std::vector<ClusterInput> _speciesOrder;
    QString pointValuesDatasetName;
    int pointValuesDatasetDatasetNumPoints;
    int pointValuesDatasetDatasetNumDimensions;
    std::vector<QString>  pointValuesDatasetDatasetDimensionNames;
    std::vector<QString>  clusterValuesDatasetDatasetClusterNames;
    std::vector<float> pointValuesDatasetDatasetDataValues;
    QString clusterValuesDatasetName;
    QString identifierDatasetName;
    std::vector<float> identifierDatasetCell;
    std::vector<QString> identifierDatasetCellDimensionNames;
    QJsonObject _treeData;  

};


// =============================================================================
// Factory
// =============================================================================

class CrossSpeciesComparisonLoaderPluginFactory : public LoaderPluginFactory
{
    Q_INTERFACES(mv::plugin::LoaderPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.CrossSpeciesComparisonLoaderPlugin"
                      FILE  "CrossSpeciesComparisonLoaderPlugin.json")
    
public:
    CrossSpeciesComparisonLoaderPluginFactory(void) {}
    ~CrossSpeciesComparisonLoaderPluginFactory(void) override {}
    QIcon getIcon(const QColor& color = Qt::black) const override;
    CrossSpeciesComparisonLoaderPlugin* produce() override;

    mv::DataTypes supportedDataTypes() const override;
};
