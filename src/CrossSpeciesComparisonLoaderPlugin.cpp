#include "CrossSpeciesComparisonLoaderPlugin.h"

#include "PointData/PointData.h"
#include "ClusterData/ClusterData.h"
#include "Set.h"
#include <vector>
#include <QtCore>
#include <QtDebug>
#include <random>
#include <QInputDialog>
#include <QSet>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <algorithm>
#include <QSet>
#include <set>
#include <iostream>
#include <string>
#include <map>
#include <limits.h>
#include <sstream>
#include <QFileDialog>

Q_PLUGIN_METADATA(IID "nl.tudelft.CrossSpeciesComparisonLoaderPlugin")

using namespace mv;

// =============================================================================
// View
// =============================================================================

CrossSpeciesComparisonLoaderPlugin::CrossSpeciesComparisonLoaderPlugin(const PluginFactory* factory):
    LoaderPlugin(factory),
    _dataSetName("")
{
    //_infoSettingsAction.setDefaultWidgetFlags(StringAction::TextEdit);
    //_infoSettingsAction.setShowLabels(false);
    //_infoSettingsAction.setDisabled(true);
}

CrossSpeciesComparisonLoaderPlugin::~CrossSpeciesComparisonLoaderPlugin(void)
{
    
}

/**
 * Mandatory plugin override function. Any initial state can be set here.
 * This function gets called when an instance of the plugin is created.
 * In this case when someone select the loader option from the menu.
 */
void CrossSpeciesComparisonLoaderPlugin::init()
{

}

/**
 * Mandatory override function. Gets called when someone selects the CrossSpeciesComparison Loader option
 * from the menu containing loader plugins. This function is responsible for opening the
 * file(s) the user wants to open, process them appropriately and pass the final data to the core.
 */

bool areAllStrings(const QStringList& list) {
    for (const QString& str : list) {
        if (str.toInt() != 0 || str.isEmpty()) {
            // If conversion to int is successful or the string is empty,
            // it's not a pure string value
            return false;
        }
    }
    return true;
}



bool areAllNumeric(const QStringList& list) {
    for (const QString& str : list) {
        if (str.isEmpty() || str.toInt() == 0) {
            // If conversion to int is unsuccessful or the result is zero, it's not a numeric value
            return false;
        }
    }
    return true;
}

bool areAllValuesUnique(const QStringList& list) {
    QSet<QString> uniqueSet;

    for (const QString& str : list) {
        if (uniqueSet.contains(str)) {
            return false;  // Found a duplicate
        }
        uniqueSet.insert(str);
    }

    return true;  // All values are unique
}

bool areAllValuesNonEmpty(const QStringList& list) {
    // Check if all values are non-empty
    for (const QString& str : list) {
        if (str.isEmpty()) {
            return false;  // Found an empty string
        }
    }
    return true;  // All values are non-empty
}

bool areListsEqual(const QStringList& list1, const QStringList& list2) {
    if (list1.size() != list2.size()) {
        return false;  // Different sizes, can't be equal
    }

    for (int i = 0; i < list1.size(); ++i) {
        if (list1.at(i) != list2.at(i)) {
            return false;  // Different elements at the same position
        }
    }

    return true;  // Lists are equal
}

bool areAllValuesNumeric(const std::vector<QStringList>& data) {
    for (const auto& strList : data) {
        for (const QString& str : strList) {
            bool isNumeric;
            str.toDouble(&isNumeric);  // Try to convert to a double
            if (!isNumeric) {
                return false;  // Found a non-numeric value
            }
        }
    }
    return true;  // All values are numeric
}
bool areAllValuesZeroOrOne(const std::vector<QStringList>& data) {
    for (const auto& strList : data) {
        for (const QString& str : strList) {
            bool isNumeric;
            double numericValue = str.toDouble(&isNumeric);

            if (!isNumeric || (numericValue != 0.0 && numericValue != 1.0)) {
                return false;  // Found a non-0 or non-1 value
            }
        }
    }
    return true;  // All values are 0 or 1
}


bool areAllValuesNonEmpty(const std::vector<QStringList>& loadedData) {
    for (const auto& tokens : loadedData) {
        for (const QString& value : tokens) {
            if (value.isEmpty()) {
                return false; // At least one value is empty
            }
        }
    }
    return true; // All values are non-empty
}
bool isNumeric(const QString& str)
{
    bool ok;
    str.toDouble(&ok);
    return ok;
}
bool hasNumericColumn(const std::vector<QStringList>& loadedData)
{
    for (const auto& row : loadedData)
    {
        for (const QString& element : row)
        {
            if (isNumeric(element))
            {
                return true; // At least one column is numeric
            }
        }
    }
    return false; // No numeric column found
}


QJsonObject convertJsonArray(QJsonObject& jsonObject, int& id) {
    QJsonObject newObject;

    if (jsonObject.contains("name")) {
        newObject["name"] = jsonObject["name"].toString();
        newObject["color"] = "#000000";
        newObject["hastrait"] = true;
        newObject["iscollapsed"] = false;
    }

    if (jsonObject.contains("children")) {
        QJsonArray childrenArray = jsonObject["children"].toArray();
        QJsonArray newChildrenArray;

        for (int i = 0; i < childrenArray.size(); i++) {
            QJsonObject childObject = childrenArray[i].toObject();
            newChildrenArray.append(convertJsonArray(childObject, id));
        }

        newObject["children"] = newChildrenArray;

        // If the object has a "name" field, it's a leaf node and should not have "id", "score", and "width" fields
        if (!jsonObject.contains("name")) {
            newObject["id"] = id++;
            newObject["score"] = 1.0;
            newObject["width"] = 1;
        }
    }

    return newObject;
}



void CrossSpeciesComparisonLoaderPlugin::loadData()
{

    //const auto fileName = AskForFileName(QObject::tr("JSON and CSV Files (*.json *.csv)"));
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        "Open File",
        "",
        "JSON and CSV Files (*.json *.csv)"
    );
    checkTypeValue = "None";
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw DataLoadException(fileName, "File was not found at location.");

    if (fileName.endsWith(".csv")) {
        qDebug() << "Loading CSV file: " << fileName;


        QTextStream in(&file);

        if (!file.atEnd()) {
            // Read data from the file
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList tokens = line.split(",");
                _loadedData.push_back(tokens);
            }
        }
        else
        {
            throw DataLoadException(fileName, "File is empty.");
        }
        // Remove last empty rows
        while (!_loadedData.empty() && _loadedData.back().isEmpty())
        {
            _loadedData.pop_back();
        }

        // Remove last empty columns
        if (!_loadedData.empty())
        {
            int maxColumns = _loadedData[0].size();

            for (int i = _loadedData.size() - 1; i >= 0; --i)
            {
                QStringList& row = _loadedData[i];

                while (!row.isEmpty() && row.back().isEmpty())
                {
                    row.pop_back();
                }

                if (row.size() < maxColumns)
                {
                    // Add empty strings to make the row size equal to maxColumns
                    row += QStringList(maxColumns - row.size(), "");
                }
            }
        }

        //  Extract header row 
        QStringList headerRow = _loadedData[0];
        //headerRow.removeLast(); //without the last column name

        if (!_loadedData.empty())
        {


            if (hasNumericColumn(_loadedData))
            {
                checkTypeValue = "AllData";
            }
            else
            {
                checkTypeValue = "MetaData";
            }
        }


        // Gather some knowledge about the data from the user
        auto fileNameString = fileName.toStdString();
        InputDialogCSV inputDialog(nullptr, fileNameString, checkTypeValue, headerRow);
        inputDialog.setModal(true);

        connect(&inputDialog, &InputDialogCSV::closeDialogCSV, this, &CrossSpeciesComparisonLoaderPlugin::dialogClosedCSV);


        int inputOk = inputDialog.exec();


    }
    else if (fileName.endsWith(".json")) {
        qDebug() << "Loading JSON file: " << fileName;
        /*
         QString jsonInput = R"([
   {"value1": "Human", "value2": "Chimpanzee", "cluster": "Cluster 1"},
   {"value1": "Gorilla", "value2": "Cluster 1", "cluster": "Cluster 2"},
   {"value1": "Rhesus", "value2": "Cluster 2", "cluster": "Cluster 3"},
   {"value1": "Marmoset", "value2": "Cluster 3", "cluster": "Cluster 4"}
   ])";
               */
        QString message = "";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw DataLoadException(fileName, "File was not found at location.");

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);

        if (!doc.isNull()) {
            if (doc.isObject()) {
                QJsonObject jsonObject= doc.object();

                int id = 1;
                _treeData = convertJsonArray(jsonObject, id);

                //std::cout << QJsonDocument(_treeData).toJson().toStdString() << std::endl;

                message = "Processed";
            }
            else {
                qDebug() << "Document is not an array";
                message = "Document is not an array";
            }
        }
        else {
            qDebug() << "Invalid JSON...\n" << jsonData;
            message = "Invalid JSON...";
        }
    
    // Gather some knowledge about the data from the user
    auto fileNameString = fileName.toStdString();
    //checkTypeValue = "None"; //"CrossSpeciesComparisonTree" or "Trait" or "None"
    InputDialogJSON inputDialog(nullptr, fileNameString, message);
    inputDialog.setModal(true);

    connect(&inputDialog, &InputDialogJSON::closeDialogJSON, this, &CrossSpeciesComparisonLoaderPlugin::dialogClosedJSON);


    int inputOk = inputDialog.exec();
}

}

// Function to generate a distance matrix
std::vector<std::vector<float>> generateDistanceMatrix(const std::vector<ClusterInput>& clusterOrder, std::vector<QString> species) {
    // Sort species in alphabetical order
    //std::sort(species.begin(), species.end());

    int numSpecies = species.size();

    // Initialize distance matrix with 0
    std::vector<std::vector<float>> distanceMatrix(numSpecies, std::vector<float>(numSpecies, 0));

    // Create a map to store the desired order of clustering
    std::map<std::string, int> orderMap;
    int order = 0;
    for (const auto& input : clusterOrder) {
        if (orderMap.count(input.value1) == 0) {
            orderMap[input.value1] = order++;
        }
        if (orderMap.count(input.value2) == 0) {
            orderMap[input.value2] = order++;
        }
    }

    // Set distances based on desired clustering
    for (int i = 0; i < numSpecies; ++i) {
        for (int j = i + 1; j < numSpecies; ++j) {
            // Set distance based on the order in which clusters should be merged
            int order_i = (orderMap.count(species[i].toStdString()) > 0) ? orderMap[species[i].toStdString()] : INT_MAX;
            int order_j = (orderMap.count(species[j].toStdString()) > 0) ? orderMap[species[j].toStdString()] : INT_MAX;
            distanceMatrix[i][j] = distanceMatrix[j][i] = std::max(order_i, order_j);
        }
    }

    return distanceMatrix;
}


void storeColumns(const std::vector<QStringList>& loadedData, std::map<int, std::vector<float>>& numericColumns, std::map<int, std::vector<std::string>>& stringColumns, std::map<int, std::string>& columnNames) {
    // Assuming headerRow is already initialized
    QStringList headerRow = loadedData[0];
    int numRows = loadedData.size() - 1; // Number of rows excluding the header

    for (int col = 0; col < headerRow.size(); ++col) {
        // Get column name
        std::string columnName = headerRow[col].toStdString();
        columnNames[col] = columnName;

        // Check if the column is numeric
        bool isNumericColumn = true;
        for (int row = 1; row < loadedData.size(); ++row) {
            bool isNumericValue;
            loadedData[row][col].toFloat(&isNumericValue);
            if (!isNumericValue) {
                isNumericColumn = false;
                break;
            }
        }

        // Store values based on whether the column is numeric or not
        if (isNumericColumn) {
            std::vector<float> numericValues;
            for (int row = 1; row <= numRows; ++row) {
                float value = -1.0; // Placeholder for missing numeric value
                if (row < loadedData.size()) {
                    bool isNumericValue;
                    value = loadedData[row][col].toFloat(&isNumericValue);
                    if (!isNumericValue) {
                        value = -1.0; // If the value is not numeric, use the placeholder
                    }
                }
                numericValues.push_back(value);
            }
            numericColumns[col] = numericValues;
        }
        else {
            std::vector<std::string> stringValues;
            for (int row = 1; row <= numRows; ++row) {
                std::string value = "None"; // Placeholder for missing non-numeric value
                if (row < loadedData.size()) {
                    value = loadedData[row][col].toStdString();
                }
                stringValues.push_back(value);
            }
            stringColumns[col] = stringValues;
        }
    }
}

void CrossSpeciesComparisonLoaderPlugin::dialogClosedJSON(QString dataSetName, QString typeName)
{
    // Generate distance matrix
    std::vector<std::vector<float>> distanceMatrix = generateDistanceMatrix(_speciesOrder, _speciesNames);

    Dataset<CrossSpeciesComparisonTree> treeDataset = mv::data().createDataset("CrossSpeciesComparisonTree", dataSetName + "_Tree");
    events().notifyDatasetAdded(treeDataset);
    treeDataset->setTreeData(_treeData);
    QJsonDocument jsonDoc(_treeData);
    QString jsonString = jsonDoc.toJson();
    //auto data= treeDataset->getFullDataset<CrossSpeciesComparisonTree>()->getData();

    /*auto datasetName = dataSetName + "_Tree";
    Dataset<Points> valuesDataset = mv::data().createDataset("Points", datasetName);
    

    std::vector<float> flatMatrix;
    int numRows = distanceMatrix.size();
    int numCols = numRows > 0 ? distanceMatrix[0].size() : 0;

    for (const auto& row : distanceMatrix) {
        for (const auto& element : row) {
            flatMatrix.push_back(element);
        }
    }

    valuesDataset->setData(flatMatrix.data(), numRows, numCols);
    valuesDataset->setDimensionNames(_speciesNames);*/

    events().notifyDatasetDataChanged(treeDataset);

    /*
    // Create an output string stream
    std::ostringstream oss;

    // Iterate over the vector and concatenate elements with commas
    for (size_t i = 0; i < _speciesNames.size(); ++i) {
        if (i != 0) {
            oss << ",";
        }
        oss << _speciesNames[i];
    }

    // Get the resulting string
    std::string result = oss.str();

    // Print the resulting string
    std::cout << result << std::endl;

    int speciesCounter = 0;
    // Print distance matrix
    for (const auto& row : distanceMatrix) {
        for (int dist : row) {
            std::cout << dist << ',';
        }
        std::cout << _speciesNames[speciesCounter];
        speciesCounter = speciesCounter + 1;
        std::cout << '\n';
    } */
    
}

void CrossSpeciesComparisonLoaderPlugin::dialogClosedCSV(QString dataSetName, QString typeName,QString leafColumn)
{


    if (checkTypeValue ==  "AllData" || checkTypeValue == "MetaData")
    {


        identifierDatasetName = dataSetName;

        std::string realType= typeName.toStdString();

        if(checkTypeValue == "CrossSpeciesComparisonTree" && realType == "CrossSpeciesComparisonTree")
        {
            if (checkTypeValue == "CrossSpeciesComparisonTree")
            {



                pointValuesDatasetName = identifierDatasetName + "_Tree";



            }


            Dataset<Points> pointValuesDataset = mv::data().createDataset("Points", pointValuesDatasetName /*,identifierDataset*/);

            events().notifyDatasetAdded(pointValuesDataset);


            pointValuesDataset->setData(pointValuesDatasetDatasetDataValues.data(), pointValuesDatasetDatasetNumPoints, pointValuesDatasetDatasetNumDimensions);
            pointValuesDataset->setDimensionNames(pointValuesDatasetDatasetDimensionNames);


            events().notifyDatasetDataChanged(pointValuesDataset);
        }
        else
        {
            if (checkTypeValue == "Meta" || (checkTypeValue == "CrossSpeciesComparisonTree"))
            {
                _numericColumnValues.clear();
                _stringColumnStrings.clear();
                _columnHeaderNameValues.clear();
                pointValuesDatasetDatasetDataValues.clear();

                // Call the method to store columns
                storeColumns(_loadedData, _numericColumnValues, _stringColumnStrings, _columnHeaderNameValues);

                pointValuesDatasetName = identifierDatasetName + "_Metadata";

                Dataset<Points> pointValuesDataset = mv::data().createDataset("Points", pointValuesDatasetName);
                events().notifyDatasetAdded(pointValuesDataset);
                pointValuesDatasetDatasetDimensionNames.clear();
                // Iterate over the map using a range-based for loop
                for (const auto& pair : _numericColumnValues)
                {
                    int key = pair.first;  // Access the key

                    if (_columnHeaderNameValues.find(key) != _columnHeaderNameValues.end()) {
                        std::string value = _columnHeaderNameValues[key];
                        pointValuesDatasetDatasetDimensionNames.push_back(QString::fromStdString(value));
                    }
                    else {
                        pointValuesDatasetDatasetDimensionNames.push_back("NotAvailabe");
                    }

                    const std::vector<float>& values = pair.second;  // Access the vector of floats
                    // Output vector of floats
                    for (const auto& value : values) {
                        pointValuesDatasetDatasetDataValues.push_back(value);
                    }


                }
                pointValuesDatasetDatasetNumDimensions = pointValuesDatasetDatasetDimensionNames.size();
                pointValuesDatasetDatasetNumPoints = pointValuesDatasetDatasetDataValues.size() / pointValuesDatasetDatasetDimensionNames.size();


                pointValuesDataset->setData(pointValuesDatasetDatasetDataValues.data(), pointValuesDatasetDatasetNumPoints, pointValuesDatasetDatasetNumDimensions);
                pointValuesDataset->setDimensionNames(pointValuesDatasetDatasetDimensionNames);
                events().notifyDatasetDataChanged(pointValuesDataset);


                    // Iterate through the map

                for (const auto& pair : _stringColumnStrings) {

                    int key = pair.first;
                    const std::vector<std::string>& value = pair.second;
                    std::map<std::string, std::vector<unsigned>> tempClusterDetailsContainer;

                    if (_columnHeaderNameValues.find(key) != _columnHeaderNameValues.end()) {
                        std::string value = _columnHeaderNameValues[key];
                        clusterValuesDatasetName = identifierDatasetName + "_" + QString::fromStdString(value);
                    }
                    else {
                        clusterValuesDatasetName = identifierDatasetName + "_" + "NotAvailabe";
                    }
                    Dataset<Clusters>  clusterValuesDataset = mv::data().createDataset("Cluster", clusterValuesDatasetName, pointValuesDataset);

                    events().notifyDatasetAdded(clusterValuesDataset);

                    for (unsigned i = 0; i < value.size(); ++i) {
                        tempClusterDetailsContainer[value[i]].push_back(i);
                    }

                    for (const auto& pair : tempClusterDetailsContainer)
                    {
                        Cluster cluster;
                        cluster.setName(QString::fromStdString(pair.first));
                        cluster.setIndices(pair.second);
                        cluster.setColor(QColor(Qt::gray));
                        clusterValuesDataset->addCluster(cluster);
                    }
                    events().notifyDatasetDataChanged(clusterValuesDataset);


                }


            }
        }


    }



}


QIcon CrossSpeciesComparisonLoaderPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return mv::Application::getIconFont("FontAwesome").getIcon("upload", color);
}

CrossSpeciesComparisonLoaderPlugin* CrossSpeciesComparisonLoaderPluginFactory::produce()
{
    return new CrossSpeciesComparisonLoaderPlugin(this);
}

mv::DataTypes CrossSpeciesComparisonLoaderPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    supportedTypes.append(ClusterType);
    return supportedTypes;
}
