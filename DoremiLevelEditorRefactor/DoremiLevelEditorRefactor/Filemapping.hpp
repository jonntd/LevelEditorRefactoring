#pragma once
#ifndef FILEMAPPING_HPP
#define FILEMAPPING_HPP
#include "Includes.hpp"
#include "Mutex.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		class MessageBuilder;
		class Filemapping
		{
		private:
			size_t m_localTail;
			size_t m_localHead;
			size_t m_messageMapSize;
			size_t m_infoMapSize;
			size_t m_nonAccessMemoryOffset;
			size_t m_chunkSize;
			Mutex m_mutex;
			FilemapInfo m_fileMapInfoStructured;

			HANDLE f_messageFilemap;
			LPVOID f_messageMapData;
			HANDLE f_infoFilemap;
			LPVOID f_infoMapData;

			bool m_isActive;
		public: 
			MessageBuilder* m_messageBuilder = nullptr;
			

			Filemapping();
			~Filemapping();

			bool GetFilemapStatus();

			void CreateFilemaps(size_t p_filemapSize = 0);
			void CloseFilemaps();
			void SetInfoMapValues(size_t p_head = 0, size_t p_tail = 0, size_t p_safetyMemorySize = 256, size_t p_messageFilemapSize = 0);
			void GetInfoMapValues();
			void PrintFilemapInfo(bool p_isPostMessage);

			MessageHeader CreateHeaderTransform(MessageInfo& p_messageInfo);
			MessageHeader CreateHeaderMesh(MessageInfo& p_messageInfo);
			MessageHeader CreateHeaderCamera(MessageInfo& p_messageInfo);
			MessageHeader CreateHeaderLight(MessageInfo& p_messageInfo);
			MessageHeader CreateHeaderMaterial(MessageInfo& p_messageInfo);
			MessageHeader CreateHeaderRenameDelete(MessageInfo& p_messageInfo);

			size_t FindWriteConfig(MessageHeader& p_header);

			bool TrySendTransform(MessageInfo& p_messageInfo);
			bool TrySendMesh(MessageInfo& p_messageInfo);
			bool TrySendCamera(MessageInfo& p_messageInfo);
			bool TrySendLight(MessageInfo& p_messageInfo);
			bool TrySendMaterial(MessageInfo& p_messageInfo);
			bool TrySendRenameDelete(MessageInfo& p_messageInfo);

			TransformMessage CreateMessageTransform(MessageInfo& p_messageInfo);
			MeshMessage CreateMessageMesh(MessageInfo& p_messageInfo);
			CameraMessage CreateMessageCamera(MessageInfo& p_messageInfo);
			LightMessage CreateMessageLight(MessageInfo& p_messageInfo);
			MaterialMessage CreateMessageMaterial(MessageInfo& p_messageInfo);
			RenameDeleteMessage CreateMessageRenameDelete(MessageInfo& p_messageInfo);

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