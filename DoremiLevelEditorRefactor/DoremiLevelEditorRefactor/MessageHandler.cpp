#include "MessageHandler.hpp"
#include "NodeHandler.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		MessageHandler::MessageHandler()
		{
			m_msgTypes.push_back("Invalid msg type");
			m_msgTypes.push_back("Added");
			m_msgTypes.push_back("Edited");
			m_msgTypes.push_back("Deleted");
			m_msgTypes.push_back("Renamed");
			m_msgTypes.push_back("Switched");
		}
		MessageHandler::~MessageHandler()
		{

		}
		void MessageHandler::printAThing()
		{
			MGlobal::displayInfo("MessageHandler reached from NodeHandler");
		}
	}
}