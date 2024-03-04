#include "InfoSettingsAction.h"



InfoSettingsAction::InfoSettingsAction(QObject* parent) :
    GroupAction(parent, "InfoSettingsAction", true),
    _infoAction(this, "Info")
{
    setText("Tree JSON Info");
    _infoAction.setToolTip("Tree Info");

    _infoAction.setDefaultWidgetFlags(StringAction::Label);
    

    // Add the widgetActionDatasetNameAction to your widget
    addAction(&_infoAction);

}
