#include "ApplicationContext.hpp"
#include "NodeHandler.hpp"
#include "MessageHandler.hpp"
#include "CallbackHandler.hpp"
#include "PrintHelper.hpp"
#include "Filemapping.hpp"
#include "MessageBuilder.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		ApplicationContext* ApplicationContext::applicationContext = nullptr;
		void ApplicationContext::Startup()
		{
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
			m_messageBuilder = new MessageBuilder;
			m_filemapping = new Filemapping;

		}
		ApplicationContext::~ApplicationContext()
		{
			delete m_nodeHandler;
			m_nodeHandler = nullptr;
			delete m_callbackHandler;
			m_callbackHandler = nullptr;
			delete m_messageHandler;
			m_messageHandler = nullptr;
			delete m_messageBuilder;
			m_messageBuilder = nullptr;
			delete m_filemapping;
			m_filemapping = nullptr;
		}
		void ApplicationContext::InitializeClasses()
		{
			m_nodeHandler->Initialize();
			m_messageHandler->Initialize();
			m_callbackHandler->Initialize();
		}

	}
}