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


void NameChangeFunc(MObject& Node, const MString & str, void* clientData)
{
	MGlobal::displayInfo("Mesh Name Has been Changed");
}


void MeshChangedFunc(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* clientData)
{
	MStatus res;
	//Hexadecimal, all new numbers are flags & checks against it. | is used to add flags.
	if (msg & MNodeMessage::AttributeMessage::kAttributeSet) {
		//MFnMesh mesh(plug.node(), &res);
		MGlobal::displayInfo("Mesh Has been changed!");

	}
		
	
}

void TransformChangedFunc(MObject& transformNode, MDagMessage::MatrixModifiedFlags& modified, void *clientData)
{
	MGlobal::displayInfo("transform is a a success!");
}

void topologyChanged(MObject& node, void *clientData)
{
	MGlobal::displayInfo("Mesh Topology has been changed!");
}

void InitialFunc(MObject& node, void *clientData) 
{
	MStatus res;
	if (node.hasFn(MFn::kMesh))
	{	
		
		MCallbackId NameChangedCallback = MNodeMessage::addNameChangedCallback(node, NameChangeFunc, NULL, &res);
		if (res = MStatus::kSuccess)
		{
			MyCallbacks.append(NameChangedCallback);
		}

		MCallbackId MeshChangedCallback = MNodeMessage::addAttributeChangedCallback(node, MeshChangedFunc, NULL, &res);
		if (res = MStatus::kSuccess)
		{
			MyCallbacks.append(MeshChangedCallback);
		}

		MCallbackId TopologyChangedCallback = MPolyMessage::addPolyTopologyChangedCallback(node, topologyChanged, NULL, &res);
		if (res = MStatus::kSuccess)
		{
			MyCallbacks.append(TopologyChangedCallback);
		}

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




EXPORT MStatus initializePlugin(MObject obj)
{

	MStatus res = MS::kSuccess; 	// most functions will use this variable to indicate for errors

	MFnPlugin myPlugin(obj, "Maya plugin", "1.0", "Any", &res);
	if (MFAIL(res))  {
		CHECK_MSTATUS(res);
	}
	
	//make a callback function here.

	MCallbackId NodeCreatedCallback = MDGMessage::addNodeAddedCallback(InitialFunc, kDefaultNodeType, NULL, &res);
	if(res==MStatus::kSuccess)
	{
		MyCallbacks.append(NodeCreatedCallback);
	}

	MCallbackId NodeTimerCallback = MTimerMessage::addTimerCallback(5.f,timerUpdateFunc, NULL, &res);
	
	if (res == MStatus::kSuccess)
	{
		MyCallbacks.append(NodeTimerCallback);
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