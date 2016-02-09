#pragma once
#ifndef FILEMAPPING_HPP
#define FILEMAPPING_HPP
#include "Includes.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		class Filemapping
		{
		public: 
			Filemapping();
			~Filemapping();

			void CreateFilemaps(bool debug);
			void SetInfoMapValues(size_t p_head, size_t p_tail, size_t p_safetyMemorySize, size_t p_messageFilemapSize);
			void GetInfoMapValues();
			void PrintFilemapInfo(bool p_isPostMessage);

			MessageHeader CreateHeaderTransform();
			MessageHeader CreateHeaderMesh();
			MessageHeader CreateHeaderCamera();
			MessageHeader CreateHeaderLight();
			MessageHeader CreateHeaderMaterial();
			MessageHeader CreateHeaderRenameDelete();

			size_t FindWriteConfig(MessageHeader& p_header);

			bool TrySendTransform(MessageInfo& p_messageInfo);
			bool TrySendMesh(MessageInfo& p_messageInfo);
			bool TrySendCamera(MessageInfo& p_messageInfo);
			bool TrySendLight(MessageInfo& p_messageInfo);
			bool TrySendMaterial(MessageInfo& p_messageInfo);
			bool TrySendRenameDelete(MessageInfo& p_messageInfo);

			TransformMessage CreateMessageTransform(TransformInfo& p_transformInfo);
			MeshMessage CreateMessageMesh(MeshInfo& p_meshInfo);
			CameraMessage CreateCameraMessage(CameraInfo& p_cameraInfo);
			LightMessage CreateLightMessate(LightInfo& p_lightInfo);
			MaterialMessage CreateMaterialMessage(MaterialInfo& p_materialInfo);
			RenameDeleteMessage CreateRenameDeleteMessage(RenameDeleteInfo& p_info);

			bool SendTransformMessage(MessageHeader& p_header, TransformMessage& p_data, int p_config);
			bool SendMeshMessage(MessageHeader& p_header, MeshMessage& p_data, int p_config);
			bool SendCameraMessage(MessageHeader& p_header, CameraMessage& p_data, int p_config);
			bool SendLightMessage(MessageHeader& p_header, LightMessage& p_data, int p_config);
			bool SendMaterialMessage(MessageHeader& p_header, MaterialMessage& p_data, int p_config);
			bool SendRenameDeleteMessage(MessageHeader& p_header, RenameDeleteMessage& p_data, int p_config);

			size_t MakeMultiple(size_t p_inputSize);
		};
	}
}
#endif