#include "scenebasic_uniform.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include "helper/glutils.h"
#include "helper/texture.h"

using std::cerr;
using std::endl;

using std::string;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

SceneBasic_Uniform::SceneBasic_Uniform() :
    tPrev(0),
    plane(50.0f, 50.0f, 1, 1), sky(100.0f) {
    meshBowl = ObjMesh::load("media/bowl.obj", true);
    meshTable = ObjMesh::load("media/table.obj", true);
    meshFish = ObjMesh::load("media/fish.obj", true);
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    model = mat4(1.0f);
    view = glm::lookAt(
        vec3(5.0f, 12.0f, 5.0f),
        vec3(0.0f, 3.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    projection = mat4(1.0f);
    angle = 0.0;
    prog.setUniform("Light.L", vec3(0.9f));
    prog.setUniform("Light.La", vec3(0.5f));

    
    prog.setUniform("Spot.L", vec3(0.9f));
    prog.setUniform("Spot.La", vec3(0.5f, 0.7f,0.5f));
    prog.setUniform("Spot.Exponent", 50.0f);
    prog.setUniform("Spot.Cutoff", glm::radians(15.0f));
    prog.setUniform("Spot.Direction", vec3(0.0f, -1.0f, 0.0f));


    GLuint glassTexID = Texture::loadTexture("media/texture/glass.jpg");
    GLuint woodTexID = Texture::loadTexture("media/texture/wood.jpg");
    GLuint cementTexID = Texture::loadTexture("media/texture/cement.jpg");
    GLuint fishTexID = Texture::loadTexture("media/texture/fish.jpg");
    GLuint fish2TexID = Texture::loadTexture("media/texture/fish2.jpg");

    GLuint skyTexID = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");

   

    


    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, glassTexID );
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, woodTexID); 
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, cementTexID);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, fishTexID);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, fish2TexID);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexID);

}

void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();
        prog.use();

        progSky.compileShader("shader/skybox.vert");
        progSky.compileShader("shader/skybox.frag");
        progSky.link();

    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t)
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;
    angle += 0.25f * deltaT;
    if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();

    //make the fish spin
    spinAngle += 0.25f ;
    if (spinAngle > glm::two_pi<float>()) spinAngle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    progSky.use(); 
    progSky.setUniform("MVP", projection * view); 
    sky.render(); 
    prog.use(); 

    vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
    mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    prog.setUniform("Light.Position", vec4(view * lightPos));
    prog.setUniform("Spot.Position", vec3(view * lightPos));
    prog.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));

    

    // Render Plane
    prog.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 500.0f);
    prog.setUniform("Material.texChoice", 4);
    model = mat4(1.0f);
    setmatrices();
    plane.render();

    // Render Bowl
    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Shininess", 50.0f);
    prog.setUniform("Material.texChoice", 0);

    
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 6.1f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    setmatrices();
    meshBowl->render();

    // Render Fish
    prog.setUniform("Material.Kd", vec3(1.0f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Shininess", 500.1f);
    prog.setUniform("Material.texChoice", 6);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.4f, 6.05f, 1.2f));
    model = glm::scale(model, vec3(0.2f));
    model = glm::rotate(model, glm::radians(spinAngle), vec3(0.0f, 1.0f, 0.0f));
    setmatrices();
    meshFish->render();


    // Render Table
    prog.setUniform("Material.Kd", vec3(1.0f)); 
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Shininess", 500.1f);
    prog.setUniform("Material.texChoice", 1);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 3.0f, 0.0f));
    setmatrices();
    meshTable->render();
  
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setmatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}
