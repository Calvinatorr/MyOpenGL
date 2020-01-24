#include "PythonEnvironment.h"

Python::Python()
{
}

Python::~Python()
{
}

void Python::ExecuteCommand(const std::string & Command)
{
	Log::PrintInfo("Python execute: " + Command);
	PyRun_SimpleString(Command.c_str());
}

void Python::ExecuteFile(const std::string & Filename)
{
	Log::PrintInfo("Python execute file '" + Filename + "'");

	FILE* filePy;
	filePy = _Py_fopen(Filename.c_str(), "r");
	PyRun_SimpleFile(filePy , Filename.c_str());
}

void Python::Initialize()
{
	Py_Initialize();
}

void Python::Cleanup()
{
	Py_Finalize();
}
