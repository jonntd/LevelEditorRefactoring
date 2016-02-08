#include "ApplicationContext.hpp"
#include "NodeHandler.hpp"
#include "MessageHandler.hpp"
#include "CallbackHandler.hpp"
#include "PrintHelper.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		ApplicationContext* ApplicationContext::applicationContext = nullptr;
		void ApplicationContext::Startup()
		{
			MGlobal::displayInfo("Bulte n1.  5");
			if (applicationContext == nullptr)
			{
				applicationContext = new ApplicationContext();
			}
		}
		void ApplicationContext::Shutdown()
		{
			if (applicationContext != nullptr)
			{
				delete applicationContext;
				applicationContext = nullptr;
			}
		}
		ApplicationContext::ApplicationContext()
		{
			m_debugMode = true;
			m_nodeHandler = new NodeHandler;
			m_callbackHandler = new CallbackHandler;
			m_messageHandler = new MessageHandler;
		}
		ApplicationContext::~ApplicationContext()
		{
			delete m_nodeHandler;
			m_nodeHandler = nullptr;
			delete m_callbackHandler;
			m_callbackHandler = nullptr;
			delete m_messageHandler;
			m_messageHandler = nullptr;
		}

	}
}