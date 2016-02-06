#pragma once
#ifndef NODEHANDLER_HPP
#define NODEHANDLER_HPP
#include "Includes.hpp"
#include <vector>
namespace DoremiEditor
{
	namespace Plugin
	{
		class MessageHandler;
		class NodeHandler
		{
		private:
			/**
			Vectors for managing loaded objects
			*/
			std::vector<TransformInfo>	m_transformVector;
			std::vector<MeshInfo>		m_meshVector;
			std::vector<CameraInfo>		m_cameraVector;
			std::vector<LightInfo>		m_lightVector;
			std::vector<MaterialInfo>	m_materialVector;

			/**
			Custom Attribute vector;
			*/
			std::vector<AttributeDescription> m_attributeVector;

			bool m_testBool;
		public:
			MessageHandler* m_messageHandler =nullptr;
			NodeHandler();
			~NodeHandler();
			
			void AddCustomAttributesTransform( MFnTransform& p_transform);
			void AddTransformNode(const MFnTransform& p_transform);
			void AddMeshNode(const MFnMesh& p_mesh);
			void AddCameraNode(const MFnCamera& p_camera);
			void AddMaterialNode(const MFnDependencyNode& p_material);
			void AddLightNode(const MFnLight& p_light);
			//void NodeNameChange(const std::string p_oldName, const std::string p_newName, const NodeType p_type);
			void NodeNameChangeTransform(MFnTransform& p_transform, const std::string p_oldName);
			void NodeNameChangeMesh(MFnMesh& p_mesh, const std::string p_oldName);
			void NodeNameChangeCamera(MFnCamera& p_camera, const std::string p_oldName);
			void NodeNameChangeLight(MFnLight& p_light, const std::string p_oldName);
			void NodeNameChangeMaterial(MFnDependencyNode& p_material, const std::string p_oldName);
			void NodeDeletedTransform(const std::string p_nodeName);
			void NodeDeletedMesh(const std::string p_nodeName);
			void NodeDeletedCamera(const std::string p_nodeName);
			void NodeDeletedLight(const std::string p_nodeName);
			void NodeDeletedMaterial(const std::string p_nodeName);
			void PrintVectorInfo();
				


		};
	}
}
#endif