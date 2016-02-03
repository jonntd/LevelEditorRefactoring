#pragma once
#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP
#include "Includes.hpp"
#include <vector>
#include <queue>
namespace DoremiEditor
{
	namespace Plugin
	{
		class NodeHandler;
		class MessageHandler
		{
		private:
			std::vector<MessageInfo>m_msgVector;
			std::queue<MessageInfo>m_msgQueue;
			std::vector<std::string>m_msgTypes;

		public:
			
			void printAThing();
			MessageHandler();
			~MessageHandler();
		};

	}
}

#endif