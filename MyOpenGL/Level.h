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

class Level : public Object, public _DrawableBase
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
		void Update() override;
		void Draw() override;
		void Cleanup() override;
};


// ===================================== SCENE OUTLINER ============================================


class SceneOutliner
{
private:
	static std::set<Level*> loadedLevels;
	static std::set<SceneObject*> selection;

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



	/* Return all levels contained by the manager */
	static std::set<Level*>& GetLoadedLevels();


	/* Select object*/
	static void Select(SceneObject*				NewSelection,	const bool& bClearSelection = false);
	/* Select multiple objects*/
	static void Select(std::set<SceneObject*>	NewSelection,	const bool& bClearSelection = false);
	/* Deselect object */
	static void Deselect(SceneObject*			NewDeslection,	const bool& bClearSelection = false);
	/* Toggle whether object is selected */
	static void ToggleSelection(SceneObject*	NewSelection,	const bool& bClearSelection = false);
	/* Return whether object exists in current selection */
	static bool IsSelected(SceneObject*			SelectionToFind);
	/* Clear selecred assets */
	static void ClearSelection();
	/* Returns set of all objects selected by manager */
	static const std::set<SceneObject*>& GetSelection();
};