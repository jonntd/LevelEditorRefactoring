#include "CmdFileMapToggle.hpp"
#include "ApplicationContext.hpp"
#include "NodeHandler.hpp"
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
				//ApplicationContext::GetInstance().GetNodeHandler()->TestFunction();
				return MS::kSuccess;
			}
			void* CmdFileMapToggle::creator()
			{
				return (void*)(new CmdFileMapToggle);
			}
		}
	}
}