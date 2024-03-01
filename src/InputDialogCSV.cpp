#include "InputDialogCSV.h"
#include "PointData/PointData.h"

InputDialogCSV::InputDialogCSV(QWidget* parent, std::string& filePath,std::string checkTypeValue) :
    QDialog(parent)
{
    setWindowTitle(tr("Cross Species Comparison JSON Data Loader"));   
    std::string fullfileName = filePath.substr(filePath.find_last_of("/\\") + 1);
    _dataNameValue = new QLineEdit();
    _dataNameValue->setText(QString::fromStdString(fullfileName.substr(0, (fullfileName.find_last_of(".")))));

    messageValue = new QLabel();  // Use QLabel instead of QLineEdit
    messageValue->setWordWrap(false);  // Allow text to wrap within QLabel
    messageValue->setAlignment(Qt::AlignTop | Qt::AlignCenter); // Adjust alignment as needed
    messageValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);


    if (checkTypeValue == "Tree")
    {
        messageValue->setText("Data can be loaded as a tree or a meta dataset! Please choose the option below.");
        messageValue->setStyleSheet("QLabel { color : black; font-size: 12pt; }");  // Adjust styles as needed
    }
    /*else if (checkTypeValue == "Trait")
    {
        messageValue->setText("Data can be loaded as a trait dataset!");
        messageValue->setStyleSheet("QLabel { color : black;  font-size: 12pt; }");  // Adjust styles as needed
    }*/
    else if (checkTypeValue == "Meta")
    {
        messageValue->setText("Data can can be loaded as a metadata dataset!");
        messageValue->setStyleSheet("QLabel { color : black;  font-size: 12pt; }");  // Adjust styles as needed
    }
    else
    {
        messageValue->setText("Data cannot be loaded! Please check the data format and try again.");
        messageValue->setStyleSheet("QLabel { color : red;  font-size: 12pt; }");  // Adjust styles as needed
    }
    addButton = new QPushButton(tr("add file"));
    addButton->setDefault(true);


     treeDataType = new QRadioButton("Tree");
    metaDataType = new QRadioButton("Meta");
    treeDataType->setChecked(true);

    okButton = new QPushButton(tr("close"));
    okButton->setDefault(true);

    connect(okButton, &QPushButton::pressed, this, &InputDialogCSV::okDialogAction);

    connect(addButton, &QPushButton::pressed, this, &InputDialogCSV::closeDialogAction);
    connect(this, &InputDialogCSV::closeDialogCSV, this, &QDialog::accept);

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(messageValue);
    if(checkTypeValue == "Tree" /*|| checkTypeValue == "Trait"*/ || checkTypeValue == "Meta")
    {
        auto labelDataNameValue = std::make_unique<QLabel>(tr("&File name: "));
        labelDataNameValue->setBuddy(_dataNameValue);
        layout->addWidget(labelDataNameValue.release());
        layout->addWidget(_dataNameValue);


        if(checkTypeValue == "Tree")
        {
            QHBoxLayout* dataTypeLayout = new QHBoxLayout();
            dataTypeLayout->addWidget(treeDataType);
            dataTypeLayout->addWidget(metaDataType);

            auto labelDataTypeValue =new QLabel(tr("Data type: "));
            layout->addWidget(labelDataTypeValue);

            layout->addLayout(dataTypeLayout);
        }


        layout->addWidget(addButton);
    }
    else
    {
        layout->addWidget(okButton);
    }

    setLayout(layout);
    adjustSize();
}

void InputDialogCSV::closeDialogAction()
{
    if(treeDataType->isChecked())
    {
               emit closeDialogCSV(_dataNameValue->text(), "Tree");
    }
    else if(metaDataType->isChecked())
    {
               emit closeDialogCSV(_dataNameValue->text(), "Meta");
    }

    //emit closeDialog(_dataNameValue->text());
}


void InputDialogCSV::okDialogAction()
{
    accept();
}