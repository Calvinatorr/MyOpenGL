#include "Level.h"



Level::Level()
{
}


Level::~Level()
{
	Clear(); // Clean-up memory
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
	std::cerr << objects.dump(4) << std::endl;

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
		if (*it != nullptr)
		{
			(*it)->Destroy();	// Destroy, rely on properly implementing for sub-classes
			delete *it;			// Delete in memory
		}

		it++; // Move to next element in set
	}

	sceneObjects.clear();		// Clear list
}

std::set<SceneObject*>& Level::GetSceneObjects()
{
	return sceneObjects;
}
