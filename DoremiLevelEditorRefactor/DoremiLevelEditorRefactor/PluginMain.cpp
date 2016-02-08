#include "PluginMain.hpp"
#include "ApplicationContext.hpp"
#include "NodeHandler.hpp"
#include "MessageHandler.hpp"
#include "CallbackHandler.hpp"
#include "CmdFileMapToggle.hpp"
#include <iostream>

//using namespace std;

// called when the plugin is loaded
EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus res = MS::kSuccess;
	try
	{
		DoremiEditor::Plugin::ApplicationContext::Startup();
		// most functions will use this variable to indicate for errors

		MFnPlugin myPlugin(obj, "Maya plugin", "1.0", "Any", &res);
		if (MFAIL(res)) {
			CHECK_MSTATUS(res);
		}

		MGlobal::displayInfo("Maya plugin loaded! ");
		// if res == kSuccess then the plugin has been loaded,
		// otherwise is has not.
	//	DoremiEditor::Plugin::ApplicationContext::GetInstance().GetNodeHandler()->TestFunction();
		DoremiEditor::Plugin::ApplicationContext::GetInstance().GetMessageHandler()->printAThing();
		DoremiEditor::Plugin::ApplicationContext::GetInstance().GetCallbackHandler()->LoadScene();
		myPlugin.registerCommand("drmTest", DoremiEditor::Plugin::Commands::CmdFileMapToggle::creator);
		//nhandle->TestFunction();
	}
	catch (...)
	{
		res = MS::kFailure;
		MGlobal::displayInfo("Somethung krasched! xx");
	}

	return res;
}


EXPORT MStatus uninitializePlugin(MObject obj)
{
	// simply initialize the Function set with the MObject that represents
	// our plugin
	MFnPlugin plugin(obj);
	DoremiEditor::Plugin::ApplicationContext::Shutdown();
	// if any resources have been allocated, release and free here before
	// returning...
	plugin.deregisterCommand("drmTest");
	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}