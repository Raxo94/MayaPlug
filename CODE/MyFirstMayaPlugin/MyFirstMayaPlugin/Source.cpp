// UD1414_Plugin.cpp : Defines the exported functions for the DLL application.

#include "maya_includes.h"
#include <iostream>

using namespace std;

MCallbackIdArray MyCallbacks;


void SetCallbackExistingNodes(MObject& Node, const MString & str, void* clientData)
{
	MItDag it(MItDag::kDepthFirst, MFn::kMesh);
	MGlobal::displayInfo("HereITCOMES!\n\n");
	while (it.isDone() == false)
	{
		MGlobal::displayInfo(it.fullPathName() + "\n\n");
		it.next();
	}

}

void NameChangeFunc(MObject& Node, const MString & str, void* clientData)
{
	MGlobal::displayInfo("YOUR Favorite Anime Character Says NAMES HAVE BEEN CHANGED");
}

void InitialFunc(MObject& node, void *clientData)
{
	//lights, Textures
	if (node.hasFn(MFn::kMesh))
	{	
		MStatus res;
		MFnMesh mymesh(node, &res);
		
		if (res==MStatus::kSuccess)
		{
			MGlobal::displayInfo("\nYES. APPARANTLY THIS IS WHAT A TRUE MESH SHOULD BE\n");
		}
		else
		{
			MGlobal::displayInfo("\nWHAT!!!THE NODE WAS NOT TRUE MESHIAN");
		}

		MGlobal::displayInfo("Vocaloid Maya says a Mesh has been created");
		MCallbackId NameChangedCallback = MNodeMessage::addNameChangedCallback(node, NameChangeFunc, NULL, &res);
		MyCallbacks.append(NameChangedCallback);
	}

}





// called when the plugin is loaded
EXPORT MStatus initializePlugin(MObject obj)
{
	// most functions will use this variable to indicate for errors
	MStatus res = MS::kSuccess;

	MFnPlugin myPlugin(obj, "Maya plugin", "1.0", "Any", &res);
	if (MFAIL(res))  {
		CHECK_MSTATUS(res);
	}
	
	//make a callback function here.

	MObject Node;

	MCallbackId NodeCreatedCallback = MDGMessage::addNodeAddedCallback(InitialFunc, kDefaultNodeType, NULL, &res);
	if(res==MStatus::kSuccess)
	{
		MGlobal::displayInfo("NodeCallBackCreatedSuccessfully");
		MyCallbacks.append(NodeCreatedCallback);
	}
	else
	{
		MGlobal::displayInfo("Callback for Node init unsuccessful");
	}
	//MyCallbacks.append(NameChangedCallback);
	// end of callbacks

	MGlobal::displayInfo("Maya plugin loaded!");

	// if res == kSuccess then the plugin has been loaded,
	// otherwise is has not.
	return res;
}


EXPORT MStatus uninitializePlugin(MObject obj)
{
	// simply initialize the Function set with the MObject that represents
	// our plugin
	MFnPlugin plugin(obj);

	// if any resources have been allocated, release and free here before
	MMessage::removeCallbacks(MyCallbacks); // returning...
	
	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}