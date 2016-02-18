#include "CmdFileMapToggle.hpp"
#include "ApplicationContext.hpp"
#include "MessageHandler.hpp"
#include "Filemapping.hpp"
#include "PrintHelper.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		namespace Commands
		{
			CmdFileMapToggle::CmdFileMapToggle()
			{

			}
			CmdFileMapToggle::~CmdFileMapToggle()
			{

			}
			MStatus CmdFileMapToggle::doIt(const MArgList& args)
			{
				try
				{
					//ApplicationContext::GetInstance().GetMessageHandler()->PrintVectorInfo(true);
					bool status = ApplicationContext::GetInstance().GetFilemapping()->GetFilemapStatus();
					if (status)
					{
						PrintDebug("Filemaps OFF");
						ApplicationContext::GetInstance().GetFilemapping()->CloseFilemaps();

					}
					else
					{
						PrintDebug("Filemaps ON");
						ApplicationContext::GetInstance().GetFilemapping()->CreateFilemaps();
					}
					return MS::kSuccess;
				}
				catch (...)
				{
					const std::string errorMessage = std::string("Catch: " + std::string(__FUNCTION__));
					PrintError(MString() + errorMessage.c_str());
				}
			}
			void* CmdFileMapToggle::creator()
			{
				return (void*)(new CmdFileMapToggle);
			}
		}
	}
}