// UD1414_Plugin.cpp : Defines the exported functions for the DLL application.

#include "maya_includes.h"
#include <iostream>

using namespace std;

MCallbackIdArray MyCallbacks;

void timerUpdateFunc(float elapsedTime, float lastTime, void* clientData)
{
	MString sa;
	sa += elapsedTime;
	MGlobal::displayInfo(sa);

}

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


void MeshChangedFunc(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* clientData)
{
	MStatus res;
	//Hexadecimal, all new numbers are flags & checks against it. | is used to add flags.
	if (msg & MNodeMessage::kAttributeSet && !plug.isArray() && plug.isElement()) 	
	{
		if (res == MStatus::kSuccess)
		{
			MObject obj = plug.node(&res);
			if (res == MStatus::kSuccess)
			{
				MFnMesh meshFn(obj, &res);
				
					MPoint aPoint;
					MFnAttribute fnAttr(plug.attribute());
					fnAttr.type();
					meshFn.getPoint(plug.logicalIndex(), aPoint, MSpace::kObject);
					//fnAttr(plug.attribute());
			}
		//MGlobal::displayInfo("Vertex Was Moved");

	
	}
	//MPoint vertex;
	//plug.getValue(vertex);
	

	
}

void TransformChangedFunc(MObject& transformNode, MDagMessage::MatrixModifiedFlags& modified, void *clientData)
{
	MGlobal::displayInfo("Node is a transform and callback is a success");
}
void InitialFunc(MObject& node, void *clientData) 
{
	//lights, Textures
	MStatus res;
	if (node.hasFn(MFn::kMesh))
	{	
		
		
		MCallbackId NameChangedCallback = MNodeMessage::addNameChangedCallback(node, NameChangeFunc, NULL, &res);
		MCallbackId MeshChangedCallback = MNodeMessage::addAttributeChangedCallback(node, MeshChangedFunc, NULL, &res);
		MyCallbacks.append(NameChangedCallback);
		MyCallbacks.append(MeshChangedCallback);

		//MFnMesh mymesh(MFnTransform(node).child(0), &res); DAVID
		//MGlobal::displayInfo(mymesh.name());


		//MFnMesh mymesh(node, &res);						CHRISTOFFER
		//MGlobal::displayInfo(mymesh.name());

		//MGlobal::displayInfo("\nYES. APPARANTLY THIS IS WHAT A TRUE MESH SHOULD BE\n");
		//MGlobal::displayInfo("Vocaloid Maya says a Mesh has been created");
		
	}

	if (node.hasFn(MFn::kTransform))
	{
		MFnTransform transformNode(node);
		MDagPath NodePath = MDagPath::getAPathTo(transformNode.child(0));


		MCallbackId TranformChanged = MDagMessage::addWorldMatrixModifiedCallback(NodePath, TransformChangedFunc, NULL, &res);
		if (res = MStatus::kSuccess)
		{
			MyCallbacks.append(TranformChanged);
		}
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


	MCallbackId NodeTimerCallback = MTimerMessage::addTimerCallback(5.f,timerUpdateFunc, NULL, &res);
	if (res == MStatus::kSuccess)
	{
		MGlobal::displayInfo("NodeCallBackCreatedSuccessfully");
		MyCallbacks.append(NodeTimerCallback);
	}
	else
	{
		MGlobal::displayInfo("Callback for Node init unsuccessful");
	}
	

	MGlobal::displayInfo("Maya plugin loaded!");

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