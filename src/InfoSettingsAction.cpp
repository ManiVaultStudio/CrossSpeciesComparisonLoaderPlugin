#include "InfoSettingsAction.h"



InfoSettingsAction::InfoSettingsAction(QObject* parent) :
    GroupAction(parent, "InfoSettingsAction", true),
    _treeInfoAction(this, "Tree info"),
    _leafInfoAction(this, "Leaf info")
{
    setText("Tree JSON Info");


    _treeInfoAction.setDefaultWidgetFlags(StringAction::TextEdit);
    _leafInfoAction.setDefaultWidgetFlags(StringAction::TextEdit);
    //_infoAction.setDefaultWidgetFlags(StringAction::TextEdit);
    //_infoAction.setClearable(false);
    //_infoAction->setReadOnly(true);
    // Add the widgetActionDatasetNameAction to your widget
    addAction(&_leafInfoAction, -1, [this](WidgetAction* action, QWidget* widget) -> void {
        auto textEdit = widget->findChild<QTextEdit*>("LineEdit");

        if (textEdit)
            textEdit->setReadOnly(true);

        });
    addAction(&_treeInfoAction, -1, [this](WidgetAction* action, QWidget* widget) -> void {
        auto textEdit = widget->findChild<QTextEdit*>("LineEdit");

        if (textEdit)
            textEdit->setReadOnly(true);

        });
}
