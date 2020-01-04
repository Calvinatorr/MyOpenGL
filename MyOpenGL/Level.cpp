#include "Level.h"



Level::Level()
{
}


Level::~Level()
{
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
	std::cerr << objects << std::endl;

	for (auto& x : objects.items())
	{

		if (x.key() == "metadata")
		{

		}
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

		for (auto&[key, val] : jsonData.items())
		{
			std::cerr << key << ":" << val << std::endl;

		}
	}

	/*
	file.open(Filename);

	
	if (file.is_open())
	{
		std::string jsonData;

		// Accumulate buffer
		std::string line;
		while (std::getline(file, line))
		{
			jsonData += line + '\n';
		}

		file.close();
		LoadFromJson(jsonData);
	}
	*/
}

void Level::AddSceneObject(SceneObject* Object)
{
	sceneObjects.insert(Object);
}

std::set<SceneObject*>& Level::GetSceneObjects()
{
	return sceneObjects;
}
