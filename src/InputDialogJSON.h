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

class InputDialogJSON : public QDialog
{
    Q_OBJECT
public:
    InputDialogJSON(QWidget* parent, std::string& filePath, QString checkTypeValue);

signals:
    void closeDialogJSON(QString dataSetName,QString typeofData);

public slots:
    void closeDialogAction();
    void okDialogAction();

private:
    QLineEdit* _dataNameValue;
    QLabel* messageValue;
    QPushButton* addButton;

    QPushButton* okButton;
};
