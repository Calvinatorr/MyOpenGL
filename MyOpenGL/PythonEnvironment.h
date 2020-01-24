#pragma once

#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif


#include "Utility.h"


#include <string>
#include <stdio.h>
#include <conio.h>


class Python
{
private:
	static PyObject* pyObjectID;

public:

	Python();
	~Python();

	// Methods
	/* Execute python string */
	static void ExecuteCommand(const std::string& Command);

	/* Executes python file */
	static void ExecuteFile(const std::string& Filename);

	/* Initialize Python environment */
	static void Initialize();

	/* Clean-up Python environment */
	static void Cleanup();

};