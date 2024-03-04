#include "InfoSettingsAction.h"



InfoSettingsAction::InfoSettingsAction(QObject* parent) :
    GroupAction(parent, "InfoSettingsAction", true),
    _treeInfoAction(this, "Tree info"),
    _leafInfoAction(this, "Leaf info")
{
    setText("Tree JSON Info");


    _treeInfoAction.setDefaultWidgetFlags(StringAction::Label);
    _leafInfoAction.setDefaultWidgetFlags(StringAction::Label);
    //_infoAction.setDefaultWidgetFlags(StringAction::TextEdit);
    //_infoAction.setClearable(false);
    //_infoAction->setReadOnly(true);
    // Add the widgetActionDatasetNameAction to your widget
    addAction(&_leafInfoAction);
    addAction(&_treeInfoAction);
}
