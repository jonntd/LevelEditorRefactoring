#include "NodeHandler.hpp"
#include "ApplicationContext.hpp"
#include "MessageHandler.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		NodeHandler::NodeHandler()
		{
			m_attributeVector.push_back(AttributeDescription("drmRender", "bool", "1"));
			m_attributeVector.push_back(AttributeDescription("drmBBox", "bool", "0"));
			m_attributeVector.push_back(AttributeDescription("drmCollidable", "bool", "0"));
			m_attributeVector.push_back(AttributeDescription("drmAIground", "bool", "0"));
			m_attributeVector.push_back(AttributeDescription("drmInteractable", "bool", "0"));
			m_attributeVector.push_back(AttributeDescription("drmInteractIntervalX", "\"float\"", "0"));
			m_attributeVector.push_back(AttributeDescription("drmInteractIntervalY", "\"float\"", "0"));
			m_attributeVector.push_back(AttributeDescription("drmInteractIntevalZ", "\"float\"", "0"));
			m_attributeVector.push_back(AttributeDescription("drmSpawner", "long", "0"));
			m_attributeVector.push_back(AttributeDescription("drmCheckPoint", "long", "0"));
			m_attributeVector.push_back(AttributeDescription("drmLevelStartEnd", "long", "0"));
			m_testBool = false;
		}
		NodeHandler::~NodeHandler()
		{

		}

		void NodeHandler::AddCustomAttributesTransform(MFnTransform& p_transform)
		{
			try
			{
				for (std::vector<AttributeDescription>::size_type i = 0; i != m_attributeVector.size(); i++)
				{
					MStatus res;
					MPlug testPlug = p_transform.findPlug(MString(m_attributeVector[i].name.c_str()), &res);
					if (res)
					{
						// Checks whether the attribute already exists and sets it to be keyable (not sure if needed but it looks cool)
						if (!testPlug.isKeyable())
						{
							MString myCommand2 = "setAttr -e -keyable true " + p_transform.name() + "." + MString(m_attributeVector[i].name.c_str());
							MGlobal::executeCommandOnIdle(myCommand2);
						}
					}
					else
					{
						//TODOJW
						//If the data type is float2 this method is used. Not used atm, delete if not needed.
						if (m_attributeVector[i].type.find("2") != std::string::npos)
						{
							MString myCommand = "addAttr -ln \"" + MString(m_attributeVector[i].name.c_str()) + "\" -dt " + MString(m_attributeVector[i].type.c_str()) + " -dv " + MString(m_attributeVector[i].value.c_str()) + " " + p_transform.name();
							MGlobal::executeCommandOnIdle(myCommand);
						}
						else
						{
							MString myCommand = "addAttr -ln \"" + MString(m_attributeVector[i].name.c_str()) + "\" -at " + MString(m_attributeVector[i].type.c_str()) + " -dv " + MString(m_attributeVector[i].value.c_str()) + " " + p_transform.name();
							MGlobal::executeCommandOnIdle(myCommand);
						}
						MString myCommand2 = "setAttr -e -keyable true " + p_transform.name() + "." + MString(m_attributeVector[i].name.c_str());
						MGlobal::executeCommandOnIdle(myCommand2);
					}
				}
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}	
		}

		void NodeHandler::PrintVectorInfo()
		{
			int totalCount, transformCount, meshCount, camCount, lightCount, materialCount;
			transformCount = m_transformVector.size();
			meshCount = m_meshVector.size();
			camCount = m_cameraVector.size();
			lightCount = m_lightVector.size();
			materialCount = m_materialVector.size();
			totalCount = transformCount + meshCount + camCount + lightCount + materialCount;
			PrintInfo("NODE COUNTS");
			PrintInfo("Transforms: " + MString() + transformCount);
			PrintInfo("Meshes: " + MString() + meshCount);
			PrintInfo("Cameras: " + MString() + camCount);
			PrintInfo("Lights: " + MString() + lightCount);
			PrintInfo("Materials: " + MString() + materialCount);
			PrintInfo("TOTAL: " + MString() + totalCount);


		}
		void NodeHandler::AddTransformNode(const MFnTransform& p_transform)
		{
			try
			{
				MFnTransform t_transform(p_transform.object());
				std::string transformName = t_transform.name().asChar();
				std::string parentName;
				if (t_transform.parent(0).hasFn(MFn::kTransform))
				{
					MFnTransform t_transformParent(t_transform.parent(0));
					parentName = t_transformParent.name().asChar();
				}
				else
				{
					parentName = "";
				}
				TransformInfo transformInfo{ transformName, parentName };
				// If no entries are added already, no need to loop.
				if (m_transformVector.size() == 0)
				{
					m_transformVector.push_back(transformInfo);
				}
				// Otherwise loop through vector to prevent duplicates
				else
				{
					bool nodeExists = false;
					for (std::vector<TransformInfo>::size_type i = 0; i != m_transformVector.size(); ++i)
					{
						if (strcmp(transformName.c_str(), m_transformVector.at(i).nodeName.c_str()) == 0)
						{
							nodeExists = true;
							break;
						}
						else
						{
							// Node doesn't exist in vectors and will be added in the next step.
						}
					}
					if (!nodeExists)
					{
						// Add node to vector
						m_transformVector.push_back(transformInfo);
						PrintDebug("Added transform ( " + MString(transformName.c_str()) + " ) with parent ( " + MString(parentName.c_str()) + " )");
					}
					else
					{
						PrintDebug("Transform " + MString(transformName.c_str()) + " already exists!");
					}
				}
				PrintVectorInfo();
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void NodeHandler::AddMeshNode(const MFnMesh& p_mesh)
		{
			try
			{
				std::string meshName = p_mesh.name().asChar();
				std::vector<std::string> parentNames;
				MeshInfo meshInfo;
				meshInfo.nodeName = meshName;
				PrintDebug("111111");
				if (p_mesh.parentCount() > 1)
				{
					for (int i = 0; i < p_mesh.parentCount(); ++i)
					{
						if(p_mesh.parent(i).hasFn(MFn::kTransform))
						{
							MFnTransform t_parent(p_mesh.parent(i));
							parentNames.push_back(t_parent.name().asChar());
							meshInfo.transformCount++;
						}
					}
					meshInfo.transformName = parentNames;
				}
				else if (p_mesh.parent(0).hasFn(MFn::kTransform)) 
				{
					MFnTransform t_transform(p_mesh.parent(0));
					parentNames.push_back(t_transform.name().asChar());
					meshInfo.transformName = parentNames;
					meshInfo.transformCount = 1;
				}
				else
				{
					// Do nothing if no transform is available. Can be changed to add an empty string instead.
				}
				if (m_meshVector.size() == 0)
				{
					m_meshVector.push_back(meshInfo);
					PrintDebug("Added mesh ( " + MString(meshName.c_str()) + " ) with parent ( " + MString(meshInfo.transformName.at(0).c_str()) + " )");
				}
				else
				{
					bool nodeExists = false;
					for (std::vector<MeshInfo>::size_type i = 0; i < m_meshVector.size(); ++i)
					{
						if(strcmp(meshName.c_str(), m_meshVector.at(i).nodeName.c_str()) == 0)
						{
							nodeExists = true;
							break;
						}
					}
					if (!nodeExists)
					{
						m_meshVector.push_back(meshInfo);
						PrintDebug("Added mesh ( " + MString(meshName.c_str()) + " ) with parent ( " + MString(meshInfo.transformName.at(0).c_str()) + " )");
					}
					else
					{
						PrintDebug("Mesh " + MString(meshName.c_str()) + " already exists!");
					}
				}
				PrintVectorInfo();
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}
		void NodeHandler::AddCameraNode(const MFnCamera& p_camera)
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
		void NodeHandler::AddMaterialNode(const MFnDependencyNode& p_material)
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
		void NodeHandler::AddLightNode(const MFnLight& p_light)
		{
			try
			{
				std::string lightName = p_light.name().asChar();
				LightInfo lightInfo;
				lightInfo.nodeName = lightName;
				if (m_lightVector.size() == 0)
				{
					m_lightVector.push_back(lightInfo);
					PrintDebug("Added light ( " + MString(lightName.c_str()) + " )");
				}
				else
				{
					bool nodeExists = false;
					for (std::vector<LightInfo>::size_type i = 0; i < m_lightVector.size(); ++i)
					{
						if (strcmp(lightName.c_str(), m_lightVector.at(i).nodeName.c_str()) == 0)
						{
							nodeExists = true;
							break;
						}
					}
					if (!nodeExists)
					{
						m_lightVector.push_back(lightInfo);
						PrintDebug("Added light ( " + MString(lightName.c_str()) + " )");
					}
					else
					{
						PrintDebug("Light " + MString(lightInfo.nodeName.c_str()) + " already exists");
					}
				}
				PrintVectorInfo();

			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}
		}

	}
}