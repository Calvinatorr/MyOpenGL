#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <set>


#include "Serialization.h"
#include "SceneObject.h"


class Level : Object
{
	protected:
		// Containers
		std::set<SceneObject*> sceneObjects;


	public:

		// Constructors and destructors
		Level();
		~Level();


		// Methods
		json Serialize() override;
		void Deserialize(const json& Data) override;
		void SaveToDisk(const std::string& Filename);
		void LoadFromDisk(const std::string& Filename);
		void AddSceneObject(SceneObject* Object);


		// Getters
		std::set<SceneObject*>& GetSceneObjects();
};

