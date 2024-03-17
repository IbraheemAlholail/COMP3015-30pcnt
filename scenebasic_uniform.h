#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "helper/teapot.h"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/skybox.h"

class SceneBasic_Uniform : public Scene
{
public:
    Plane plane;
    SkyBox sky;
    float tPrev,angle, spinAngle;
    std::unique_ptr<ObjMesh> meshTable;
    std::unique_ptr<ObjMesh> meshBowl;
    std::unique_ptr<ObjMesh> meshFish;
    GLuint texWoodID, texGlassID, texCementID, texFishID;
   


    GLSLProgram prog;
    GLSLProgram progSky;
    void setmatrices();
    void compile();


public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
