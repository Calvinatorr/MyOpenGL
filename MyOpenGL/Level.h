#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <set>


#include "Serialization.h"
#include "SceneObject.h"


class Level;
class LevelManager;


// ===================================== LEVEL ============================================

class Level : public Object
{
private:
	bool loaded = false;
	bool visible = true;


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
		void Clear();

		void Load();
		void Unload();
		void SetVisibility(bool bNewVisibility = true);



		// Getters
		std::set<SceneObject*>& GetSceneObjects();
		bool IsLoaded() const;
		bool IsVisible() const;


		// Management
		void Update();
		void Draw();
		void Cleanup();
};


// ===================================== LEVEL ============================================


class LevelManager
{
private:
	static std::set<Level*> loadedLevels;


public:


	// Methods

	/* Load in new level */
	static bool LoadLevel(Level* NewLevel);
	/* Unloaded specified level */
	static bool UnloadLevel(Level* LevelToUnload);
	/* Update all levels */
	static void Update();
	/* Draw all loaded and visible levels */
	static void Draw();
	/* Cleanup levels */
	static void Cleanup();



	// Getters

	/* Return all levels contained by the manager */
	static std::set<Level*>& GetLoadedLevels();

};