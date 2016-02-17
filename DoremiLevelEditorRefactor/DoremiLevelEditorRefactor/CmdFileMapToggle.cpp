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
				//ApplicationContext::GetInstance().GetMessageHandler()->PrintVectorInfo(true);
				bool status = ApplicationContext::GetInstance().GetFilemapping()->GetFilemapStatus();
				if (status)
				{
					ApplicationContext::GetInstance().GetFilemapping()->CloseFilemaps();
					PrintDebug("Filemaps OFF");
				}
				else
				{
					ApplicationContext::GetInstance().GetFilemapping()->CreateFilemaps();
					PrintDebug("Filemaps ON");
				}
				return MS::kSuccess;
			}
			void* CmdFileMapToggle::creator()
			{
				return (void*)(new CmdFileMapToggle);
			}
		}
	}
}