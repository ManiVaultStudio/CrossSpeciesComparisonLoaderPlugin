#pragma once

#include <actions/DatasetPickerAction.h>
#include "PointData/DimensionsPickerAction.h"

#include "actions/GroupAction.h"
#include "actions/TriggerAction.h"
#include "actions/OptionsAction.h"
#include "actions/OptionAction.h"
#include "actions/StringAction.h"
/** All GUI related classes are in the ManiVault Graphical User Interface namespace */
using namespace mv::gui;

class InfoSettingsAction : public GroupAction
{
public:

    /**
     * Constructor
     * @param parent Pointer to parent object
     */
    InfoSettingsAction(QObject* parent = nullptr);

public: // Action getters


    StringAction& getInfoAction() { return _infoAction; }



public:
    StringAction    _infoAction;
};
