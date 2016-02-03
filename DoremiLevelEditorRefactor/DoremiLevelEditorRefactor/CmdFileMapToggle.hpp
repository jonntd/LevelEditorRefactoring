#pragma once
#ifndef CMDFILEMAPTOGGLE_HPP
#define CMDFILEMAPTOGGLE_HPP
#include "Includes.hpp"
namespace DoremiEditor
{
	namespace Plugin
	{
		namespace Commands
		{
			class NodeHandler;
			class CmdFileMapToggle : public MPxCommand
			{
			public:
				CmdFileMapToggle();
				virtual ~CmdFileMapToggle();
				MStatus doIt(const MArgList& args);
				static void* creator();
			};

		}
	}
}

#endif