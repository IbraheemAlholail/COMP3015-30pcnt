# COMP3015 30pcnt
 
This is an OpenGL scene made by Ibraheem Alholail

The scene depicts a fish that has fallen out of the fish bowl and is flailing on the table.
The models were made by me in blender and exported as waveObjects (.obj).

This project was created in Windows 11 using OpenGL with the stb library.
It was coded using Visual Studio 2022 with the GLSL integration extension.
A large portion of the code can be found in the Scenebasic_unform.cpp file where our first few items are loaded, this includes the plane, skybox, and meshes
next, in the initScene function, the scene is initialized. The initialization starts by making the camera view and aiming it at the subject. next, the 
lighting is initialized, both the ambient and spotlight. after that, the textures are loaded from the media folder and then activated and bound. Once that is complete the shader files are called and compiled. All animations are done in the update function, in my case that's the spinning lights and flailing fish. After that, the Render function renders the skybox, plane, and meshes into the scene. 
The other portion of the code is in the shader files, with most of that being in the file basic_uniform.frag for the lighting. Here, our textures, and lighting structures are declared for use in the CPP file. The frag contains two functions, one for the blinnphong ambient basic lighting, which handles the regular ambient lighting as well as the textures for the meshes, plane, and skybox, and the second function handles the spotlight.

Youtube Link: https://youtu.be/cT4Yy6hVH8E


