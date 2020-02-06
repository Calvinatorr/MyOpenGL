#include "Primitives.h"


StaticMesh Primitives::cube;

void Primitives::Initialize()
{
	cube.Import(ENGINE_PATH + "Primitives/Cube.obj");
	AssetManager::Register(&cube);
}
