#pragma once

#include "SceneComponent.h"
#include "StaticMesh.h"


/*#ifdef __unix__                    

#define OS_Windows 0
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#elif defined(_WIN32) || defined(WIN32)

#define OS_Windows 1
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <commdlg.h>
#define DIV 1048576
#define WIDTH 7

#endif*/


// Container for static mesh in the scene
class StaticMeshComponent : public SceneComponent
{
public:

	// Constructors & Destructors
	StaticMeshComponent();
	StaticMeshComponent(StaticMesh* StaticMesh);
	~StaticMeshComponent();


	// Properties
	StaticMesh* staticMesh;


	// Events
	void Draw() override;
	void Cleanup() override;
	void DrawDetails() override;
};

