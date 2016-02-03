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
			
			void PrintVectorInfo();
				


		};
	}
}
#endif