#include <maya/MPxContext.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MPxContextCommand.h>

#include "EasyDressTool.h"


//////////////////////////////////////////////
// Command to create contexts
//////////////////////////////////////////////

class LassoContextCmd : public MPxContextCommand
{
public:
	LassoContextCmd() = default;
	virtual MPxContext* makeObj();
	static void*		creator();
};

MPxContext* LassoContextCmd::makeObj()
{
	return new EasyDressTool;
}

void* LassoContextCmd::creator()
{
	return new LassoContextCmd;
}



//////////////////////////////////////////////
// plugin initialization
//////////////////////////////////////////////
MStatus initializePlugin(MObject obj)
{
	MStatus		status;
	MFnPlugin	plugin(obj, PLUGIN_COMPANY, "3.0", "Any");
	std::cout << "plugin loaded" << std::endl;
	status = plugin.registerContextCommand("lassoToolContext",
		LassoContextCmd::creator);

	if (!status) {
		status.perror("registerContextCommand");
		return status;
	}

	// Register User Interface
	MString cmd_create_ui = R"(source ")" + plugin.loadPath() + R"(/easyDressCreateUI.mel")";
	MString cmd_delete_ui = R"(source ")" + plugin.loadPath() + R"(/easyDressDeleteUI.mel")";
	MGlobal::executeCommand(cmd_create_ui);
	MGlobal::executeCommand(cmd_delete_ui);

	// set the mel scripts to be run when the plugin is loaded / unloaded
	status = plugin.registerUI("easyDressCreateUI", "easyDressDeleteUI");
	if (!status) {
		status.perror("registerUIScripts");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus		status;
	MFnPlugin	plugin(obj);

	status = plugin.deregisterContextCommand("lassoToolContext");

	return status;
}
