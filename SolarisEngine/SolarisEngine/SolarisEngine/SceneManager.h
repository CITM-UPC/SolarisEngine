#pragma once

#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Scene.h"


class SceneManager {
public:
    SceneManager();
    ~SceneManager();


    void SaveScene();
    
    void Cleanup();

private:

    std::vector<Scene*> scenes;
    Scene* activeScene;


};

#endif // __SCENE_MANAGER_H__