#include "LoadSaveScene.h"
#include "SaveSceneManager.h"
#include "SaveGameObject.h"
#include "SaveScene.h"

void LoadAndManageScenes() {
    SaveSceneManager sceneManager;

    // Crea y añadir scene(Test)
    SaveScene scene1;
    SaveGameObject obj1 = {SaveGameObjectFactory::GenerateUniqueID(), 0, "RootNode1", {0, 0, 0}, {1, 1, 1}, {0, 0, 0, 1}, {}};
    scene1.AddSaveGameObject(obj1);
    sceneManager.AddScene("Scene1", scene1);

    //SaveScene scene2;
    //SaveGameObject obj2 = {SaveGameObjectFactory::GenerateUniqueID(), obj1.uid, "ChildNode1", {1, 0, 0}, {1, 1, 1}, {0, 0, 0, 1}, {}};
    //scene2.AddSaveGameObject(obj2);
    //sceneManager.AddScene("Scene2", scene2);

    // Guaradar archivo de scene 
    sceneManager.Save("Scene1", "Assets/scene1.json");
    //sceneManager.SaveScene("Scene2", "scene2.json");

    // carregar archivo de scene
    sceneManager.Load("LoadedScene1", "Assets/scene1.json");
    //sceneManager.LoadScene("LoadedScene2", "scene2.json");
}
