#include "CallbackHandler.hpp"
#include "NodeHandler.hpp"
#include "MessageHandler.hpp"
#include "ApplicationContext.hpp"
#include <string>

namespace DoremiEditor
{
	namespace Plugin
	{
		MCallbackIdArray CallbackHandler::m_callbackIDArray;
		NodeHandler* CallbackHandler::s_nodeHandler = nullptr;
		CallbackHandler::CallbackHandler()
		{
			m_callbackIDArray.append(MDGMessage::addNodeAddedCallback(CallbackHandler::cb_newNode));
		}
		CallbackHandler::~CallbackHandler()
		{
			MMessage::removeCallbacks(m_callbackIDArray);
			m_callbackIDArray.clear();
		}
		void CallbackHandler::LoadScene()
		{
			try
			{
				if (s_nodeHandler == nullptr)
				{
					s_nodeHandler = ApplicationContext::GetInstance().GetNodeHandler();
				}
				MStatus result;
				MItDependencyNodes itDep(MFn::kLambert);
				while (!itDep.isDone())
				{
					if (itDep.thisNode().apiType() == MFn::kLambert)
					{
						AddMaterial(itDep.thisNode());
						
					}
					else if (itDep.thisNode().apiType() == MFn::kPhong)
					{
						AddMaterial(itDep.thisNode());
					}
					else if (itDep.thisNode().apiType() == MFn::kBlinn)
					{
						AddMaterial(itDep.thisNode());
					}
					else
					{
						// Forget about it!
					}
					itDep.next();
				}
				MFn::Type filter = MFn::kTransform;
				MItDag itTransforms(MItDag::kDepthFirst, filter, &result);
				if (!result)
				{
					PrintError("Could not create transform iterator!");
				}
				else
				{
					for (; !itTransforms.isDone(); itTransforms.next())
					{
						MDagPath t_transDagPath;
						result = itTransforms.getPath(t_transDagPath);
						MFnTransform t_trans(t_transDagPath, &result);
						MFnDagNode t_transDagNode(t_transDagPath, &result);
						int childCount = t_transDagNode.childCount();
						AddTransform(t_transDagPath.node());
						for (int i = 0; i < childCount; ++i)
						{
							MObject t_child(t_trans.child(i));
							if (t_child.hasFn(MFn::kMesh))
							{
								AddMesh(t_child, false);
							}
							else if (t_child.hasFn(MFn::kLight))
							{
								AddLight(t_child, false);
							}
							else if (t_child.hasFn(MFn::kCamera))
							{
								AddCamera(t_child);
							}
						}
					}

				}
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_newNode(MObject& p_node, void* p_clientData)
		{
			try
			{
				MStatus result;
				if (s_nodeHandler == nullptr)
				{
					s_nodeHandler = ApplicationContext::GetInstance().GetNodeHandler();
				}
				if (p_node.hasFn(MFn::kTransform))
				{
					MFnTransform t_transform(p_node, &result);
					if (t_transform.childCount() > 0)
					{
						AddTransform(p_node);
					}
					else
					{
						// If the transform has no children, there is at the moment no need to add this node 
					}
				}
				else if (p_node.hasFn(MFn::kMesh))
				{
					AddMesh(p_node, true);
				}
				else if (p_node.hasFn(MFn::kCamera))
				{
					AddCamera(p_node);
				}
				else if (p_node.hasFn(MFn::kLight))
				{
					AddLight(p_node, true);
				}
				else if (p_node.hasFn(MFn::kLambert))
				{
					AddMaterial(p_node);
				}
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_timer(float elapsedTime, float lastTime, void* p_clientData)
		{
			try
			{

			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_preRemoveNode(MObject& p_node, void* p_clientData)
		{
			try
			{

			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_nameChange(MObject& p_node, const MString& p_str, void* p_clientData)
		{
			try
			{

			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_parentChanges(MDagPath& child, MDagPath& p_parent, void* p_clientData)
		{
			try
			{

			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_meshEvaluate(MNodeMessage::AttributeMessage p_msg, MPlug& plug_1, MPlug& plug_2, void* p_clientData)
		{
			try
			{
				MStatus result;
				std::string t_plugName(plug_1.name().asChar());
				// Determines if the doublesided attribute has been set. This is to determine whether or not the mesh 
				if (t_plugName.find("doubleSided") != std::string::npos && MNodeMessage::AttributeMessage::kAttributeSet)
				{
					MFnMesh t_mesh(plug_1.node(), &result);
					if (result)
					{
						m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(plug_1.node(), cb_meshAttributeChange));
						MMessage::removeCallback(MNodeMessage::currentCallbackId());
						s_nodeHandler->AddMeshNode(t_mesh);
						//s_nodeHandler->PrintVectorInfo();
					}
				}
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}

	
		}
		void CallbackHandler::cb_meshAttributeChange(MNodeMessage::AttributeMessage p_msg, MPlug& plug_1, MPlug& plug_2, void* p_clientData)
		{
			try
			{
				MStatus result;
				MFnMesh t_mesh(plug_1.node(), &result);
				if (result)
				{
					// Might have to put a check if this node exists in the mesh vector
					std::string t_plugName = plug_1.name().asChar();
					if (t_plugName.find("pnts") != std::string::npos && t_plugName.find("[") != std::string::npos)
					{
						PrintDebug(t_mesh.name() + " Mesh message to be added ");
					}
					else if (plug_2.node().apiType() == MFn::Type::kShadingEngine)
					{
						PrintDebug(t_mesh.name() + " Mesh message to be added (shading engine changed)");
					}
				}
			}
		
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_meshPolyChange(MObject& p_node, void* p_clientData)
		{
			try
			{
				MStatus result;
				MFnMesh t_mesh(p_node, &result);
				if (result)
				{
					PrintDebug(t_mesh.name() + " Mesh poly message to be added ");
				}
				else
				{	
					// Shouldn't happen.
				}
				
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_transformAttributeChange(MNodeMessage::AttributeMessage p_msg, MPlug& plug_1, MPlug& plug_2, void* p_clientData)
		{
			try
			{
				MStatus result;
				if (p_msg & MNodeMessage::AttributeMessage::kAttributeSet)
				{
					MFnTransform t_transform(plug_1.node(), &result);
					if (result)
					{
						//add message
						PrintDebug(t_transform.name() + " transform changed message to be added");
					}
				}
			}
			
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_cameraPanels(const MString& p_str, MObject& p_node, void* p_clientData)
		{
			try
			{

			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_cameraAttributeChange(MNodeMessage::AttributeMessage p_msg, MPlug& plug_1, MPlug& plug_2, void* p_clientData)
		{

		}
		void CallbackHandler::cb_lightEvaluate(MNodeMessage::AttributeMessage msg, MPlug& plug_1, MPlug& plug_2, void* clientData)
		{
			try
			{
				MStatus result;
				std::string plugName = plug_1.name().asChar();
				MFnLight t_light(plug_1.node(), &result);
				if (result)
				{
					std::string lightName = t_light.name().asChar();
					// Makes sure node is finished created by finding out if the name includes a '#' which means the node has not yet 
					// been fully created.
					if (plugName.find("#") == std::string::npos && MNodeMessage::AttributeMessage::kAttributeSet)
					{
						s_nodeHandler->AddLightNode(t_light);
						m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(plug_1.node(), cb_lightAttributeChange));
						m_callbackIDArray.append(MNodeMessage::addNodePreRemovalCallback(plug_1.node(), cb_preRemoveNode));
						m_callbackIDArray.append(MNodeMessage::addNameChangedCallback(plug_1.node(), cb_nameChange));
						MMessage::removeCallback(MMessage::currentCallbackId());
					}
				}
				else
				{
					// No action
				}
				
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_lightAttributeChange(MNodeMessage::AttributeMessage p_msg, MPlug& plug_1, MPlug& plug_2, void* p_clientData)
		{
			try
			{
				MFnLight t_light(plug_1.node());
				bool sendMsg = false;
				std::string lightName(t_light.name().asChar());
				std::string plugName(plug_1.name().asChar());

				if (p_msg & MNodeMessage::AttributeMessage::kAttributeSet && p_msg != 2052)
				{
					MStatus result;
					if (plugName.find(".intensity") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".color") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".decayRate") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".coneAngle") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".dropOff") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".penumbraAngle") != std::string::npos)
					{
						sendMsg = true;

					}
					if (sendMsg)
					{
						//FileMapping::printInfo(MString(plugName.c_str()) + "    " + plug_1.node().apiTypeStr() + "  " + msg);
						PrintDebug(t_light.name() + " light attribute changed, message to be added!");
					}
				}
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::cb_materialAttributeChanges(MNodeMessage::AttributeMessage p_msg, MPlug& plug_1, MPlug& plug_2, void* p_clientData)
		{
			try
			{
				MFnDependencyNode mat(plug_1.node());

				std::string lightName(mat.name().asChar());
				std::string plugName(plug_1.name().asChar());


				if (p_msg & MNodeMessage::AttributeMessage::kAttributeSet && p_msg != 2052)
				{
					bool sendMsg = false;
					//outLightData(lightName.asChar());
					MStatus result;
					if (plugName.find(".diffuse") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".color") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".transparency") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".ambient") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".specularColor") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".eccentricity") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".specularRollOff") != std::string::npos)
					{
						sendMsg = true;
					}
					else if (plugName.find(".cosinePower") != std::string::npos)
					{
						sendMsg = true;
					}

					if (sendMsg)
					{
						PrintDebug("Material change " + MString(plugName.c_str()));
						// Add a message
					}
				}
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::AddTransform(MObject& p_node)
		{
			MStatus result;
			MFnTransform t_transform(p_node, &result);
			if (result)
			{
				s_nodeHandler->AddCustomAttributesTransform(t_transform);
				m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(p_node, cb_transformAttributeChange));
				m_callbackIDArray.append(MNodeMessage::addNodePreRemovalCallback(p_node, cb_preRemoveNode));
				s_nodeHandler->AddTransformNode(t_transform);
			}
			else
			{
				PrintError("Could not create MFnTransform from node: " + MString(p_node.apiTypeStr()));
			}
			
		}
		void CallbackHandler::AddMesh(MObject& p_node, bool p_isNew)
		{
			MStatus result;
			MFnMesh t_mesh(p_node, &result);

			// Set quadsplit to left. Ensures that the internal triangulation works properly.
			MString myCommand = "setAttr -e " + t_mesh.name() + ".quadSplit 0";
			MGlobal::executeCommandOnIdle(myCommand);
			// Add Callbacks
			m_callbackIDArray.append(MPolyMessage::addPolyTopologyChangedCallback(p_node, cb_meshPolyChange));
			m_callbackIDArray.append(MNodeMessage::addNodePreRemovalCallback(p_node, cb_preRemoveNode));
			if (result)
			{
				m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(p_node, cb_meshAttributeChange));
				s_nodeHandler->AddMeshNode(t_mesh);
				// TODOJW: Investigate and add duplicate/instancing callbacks.
			}
			else if (p_isNew)
			{
					m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(p_node, cb_meshEvaluate));
			}
			else
			{
				PrintError("Could not create MFnMesh from node: " + MString(p_node.apiTypeStr()));
			}

		}
		void CallbackHandler::AddCamera(MObject& p_node)
		{
			try
			{
				MStatus result;
				MFnCamera t_camera(p_node, &result);
				if (result)
				{
					m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(p_node, cb_cameraAttributeChange));
					s_nodeHandler->AddCameraNode(t_camera);
				}
				else
				{
					// Do nada
				}
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::AddLight(MObject& p_node, bool p_isNew)
		{
			try
			{
				MStatus result;
				MFnLight t_light(p_node, &result);
				if (result)
				{
					if (p_node.hasFn(MFn::kDirectionalLight) || p_node.hasFn(MFn::kSpotLight) || p_node.hasFn(MFn::kPointLight))
					{
						if (p_isNew)
						{
							m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(p_node, cb_lightEvaluate));
						}
						else
						{
							m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(p_node, cb_lightAttributeChange));
							m_callbackIDArray.append(MNodeMessage::addNodePreRemovalCallback(p_node, cb_preRemoveNode));
							m_callbackIDArray.append(MNodeMessage::addNameChangedCallback(p_node, cb_nameChange));
							s_nodeHandler->AddLightNode(t_light);
						}
					}
				}
				else
				{
					// Do nothing maybe?
				}
			
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void CallbackHandler::AddMaterial(MObject& p_node)
		{
			try
			{
				MStatus result;
				MFnDependencyNode t_material(p_node);
				m_callbackIDArray.append(MNodeMessage::addAttributeChangedCallback(p_node, cb_materialAttributeChanges));
				m_callbackIDArray.append(MNodeMessage::addNodePreRemovalCallback(p_node, cb_preRemoveNode));
				m_callbackIDArray.append(MNodeMessage::addNameChangedCallback(p_node, cb_nameChange));
				PrintDebug(t_material.name() + " material found");
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
	}
}