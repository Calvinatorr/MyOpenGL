#include "Level.h"



Level::Level()
{
}


Level::~Level()
{
	//Clear(); // Clean-up memory
}




json Level::Serialize()
{
	json jsonData = Object::Serialize();

	const char* OBJECTS_KEY = "objects";
	json objectData = { {OBJECTS_KEY, {}} };


	auto it = sceneObjects.begin(); // Return iterator for this set type
	while (it != sceneObjects.end())
	{
		if (*it != nullptr)
		{
			//std::cerr << (*it)->GetName() << std::endl;
			objectData[OBJECTS_KEY] += (*it)->Serialize(); // Apend json objects
		}

		it++; // Move to next element in set
	}

	jsonData.update(objectData); // Merge JSON objects
	return jsonData;
}


void Level::Deserialize(const json & Data)
{
	Object::Deserialize(Data);
	json objects = Data["objects"];
	//std::cerr << objects.dump(4) << std::endl;

	// For each object stored in the level
	for (auto& x : objects.items())
	{
		//std::cerr << x.key() << ":" << x.value() << std::endl;

		// Create new Object type, deserialize it from JSON data
		SceneObject* newObject = new SceneObject();	// Allocate object on heap
		newObject->Deserialize(x.value());			// Deserialize from JSON data
		AddSceneObject(newObject);					// Add to list
	}
}

void Level::SaveToDisk(const std::string & Filename)
{
	json jsonData = Serialize();

	// Output json stream to disk
	std::ofstream file(Filename);
	if (file.is_open())
	{
		// Write JSON data to file at indentation level 4
		file << jsonData.dump(4) << std::endl;
		file.close();
		Log::Print("Saved level '" + Filename + "'");
	}
	else
	{
		Log::PrintError("Failed to save level '" + Filename + "'");
	}
}


void Level::LoadFromDisk(const std::string & Filename)
{
	std::ifstream file(Filename);
	if (file.is_open())
	{
		// Load json from disk
		json jsonData;
		file >> jsonData;

		// If we successfully loaded the file from disk, then clear the level & load everything else in
		Clear();
		Deserialize(jsonData);

		Log::Print("Loaded level '" + Filename + "'");
	}
	else
	{
		Log::PrintError("Failed to load level '" + Filename + "'");
	}
}

void Level::AddSceneObject(SceneObject* Object)
{
	sceneObjects.insert(Object);
}

void Level::Clear()
{
	auto it = sceneObjects.begin(); // Return iterator for this set type
	while (it != sceneObjects.end())
	{
		if ((*it) != nullptr)
		{
			(*it)->Destroy();	// Destroy, rely on properly implementing for sub-classes
			delete *it;			// Delete in memory
		}

		it++; // Move to next element in set
	}

	sceneObjects.clear();		// Clear list
}

void Level::Load()
{
	if (!IsLoaded())
	{
		loaded = true;

		LevelManager::LoadLevel(this);
	}
}

void Level::Unload()
{
	if (IsLoaded())
	{
		loaded = false;

		LevelManager::UnloadLevel(this);
	}
}

void Level::SetVisibility(bool bNewVisibility)
{
	visible = bNewVisibility;
}

std::set<SceneObject*>& Level::GetSceneObjects()
{
	return sceneObjects;
}

bool Level::IsLoaded() const
{
	return loaded;
}

bool Level::IsVisible() const
{
	return visible;
}

void Level::Update()
{
	if (IsLoaded())
	{
		for (auto&& x : sceneObjects)
		{
			if (x != nullptr)
			{
				x->Update();
			}
		}
	}
}

void Level::Draw()
{
	if (IsLoaded() && IsVisible())
	{
		for (auto&& x : sceneObjects)
		{
			if (x != nullptr)
			{
				x->Draw();
			}
		}

		/*
		auto it = sceneObjects.begin(); // Return iterator for this set type
		while (it != sceneObjects.end())
		{
			if (*it != nullptr)
			{
				(*it)->Draw();
			}

			it++; // Move to next element in set
		}*/
	}
}

void Level::Cleanup()
{
	for (auto&& x : sceneObjects)
	{
		if (x != nullptr)
		{
			x->Destroy();
		}
	}
}


// ===================================== LEVEL MANAGER ============================================


std::set<Level*> LevelManager::loadedLevels = std::set<Level*>();


bool LevelManager::LoadLevel(Level * NewLevel)
{
	try
	{
		loadedLevels.insert(NewLevel); // Insert unique
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: ' " << e.what() << " '\n";
		return false;
	}
	return true;
}

bool LevelManager::UnloadLevel(Level * LevelToUnload)
{
	try
	{
		auto foundLevel = loadedLevels.find(LevelToUnload); // Return iterator to level

		// Derefer point and try to clear level
		if (*foundLevel != nullptr)
		{
			(*foundLevel)->Clear();
		}

		loadedLevels.erase(foundLevel); // Remove from manager
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: ' " << e.what() << " '\n";
		return false;
	}
	return true;
}

void LevelManager::Update()
{
	for (auto it = loadedLevels.begin(); it != loadedLevels.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->Update();
		}
	}
}

void LevelManager::Draw()
{
	for (auto it = loadedLevels.begin(); it != loadedLevels.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->Draw();
		}
	}
}

void LevelManager::Cleanup()
{
	for (auto it = loadedLevels.begin(); it != loadedLevels.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->Cleanup();
		}
	}
}

std::set<Level*>& LevelManager::GetLoadedLevels()
{
	return loadedLevels;
}