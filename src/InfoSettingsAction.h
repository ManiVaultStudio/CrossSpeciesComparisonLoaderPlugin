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

/**
 * CrossSpeciesComparisonInfoSettingsAction class
 *
 * Class that houses settings for the CrossSpeciesComparison analysis plugin
 *
 * This settings class is derived from the group action class. A group action
 * is a special type of action; when injected into a dataset its user interface
 * becomes available in the data properties widget. The group action list the child
 * actions in a form-like fashion. The order in which they appear corresponds with
 * the order of declaration.
 *
 * Note: we strongly encourage you to use ManiVault core actions to build the user
 * interface. Actions separate the data and business logic from the user interface.
 * We have standard actions for editing of strings, decimals, integrals, options,
 * color and color maps. With these components, there is no need to write to create
 * the user interface yourself. The actions will take care of this. For more
 * information regarding actions, please visit actions/Actions.h
 */
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
