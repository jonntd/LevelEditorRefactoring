#include "PrintHelper.hpp"
#include "ApplicationContext.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		void PrintInfo(MString p_str)
		{
			MGlobal::displayInfo("[Editor] " + p_str);
		}
		void PrintError(MString p_str)
		{
			MGlobal::displayError("[Editor Error] " + p_str);
		}
		void PrintWarning(MString p_str)
		{
			MGlobal::displayWarning("[Editor Warning] " + p_str);
		}
		void PrintDebug(MString p_str)
		{
			if (ApplicationContext::GetInstance().GetDebugStatus())
			{
				MGlobal::displayInfo("[Editor Debug] " + p_str);
			}
			else
			{

			}
		}
		MString GetNameStrings(const std::string p_name1, const std::string p_name2) 
		{
			MString str = (" ( " + MString(p_name1.c_str()) + " ) -> ( " + MString(p_name2.c_str()) + " ) ");
			return str;
		}
	}
}