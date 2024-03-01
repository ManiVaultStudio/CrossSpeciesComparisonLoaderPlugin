#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QGroupBox>
// =============================================================================
// Loading input box
// =============================================================================

class InputDialogCSV : public QDialog
{
    Q_OBJECT
public:
    InputDialogCSV(QWidget* parent, std::string& filePath, std::string checkTypeValue);

signals:
    void closeDialogCSV(QString dataSetName,QString typeofData);

public slots:
    void closeDialogAction();
    void okDialogAction();

private:
    QLineEdit* _dataNameValue;
    QLabel* messageValue;
    QPushButton* addButton;
    QRadioButton* treeDataType;
    QRadioButton* metaDataType;
    QPushButton* okButton;
};
