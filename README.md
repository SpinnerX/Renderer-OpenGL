# OpenGL Renderer

Renderer that I refactored using OpenGL, to utilize Conan.

## Build Starter

Run this conan command to build the project with `-b missing` on the first build, so conan can install any missing dependencies.

```
conan build . -b missing
```

## Running the Starter

Running the executable, `./build/Release/Starter`

# Progress Showcase

This image shows applying bit of shadowmapping and lighting sources to the surface of the mesh along with the skybox.

![mesh_skybox_image](https://github.com/user-attachments/assets/ca286c27-365e-4221-be7f-55a7a63cd1a8)

Showing binding multiple material sources onto the mesh applying normal, specular, and diffuse mappings onto the two meshes.

![mesh_image](https://github.com/user-attachments/assets/1f359756-d473-4926-ba96-2757820ec34d)

# Video Posting

This was some of the earlier works that I did on the renderer, adding basic blinn-phong lighting model. That calculates ambient, specular, and diffuse for the lighting calculation in the shaders.

https://github.com/user-attachments/assets/cb25b4a6-df62-495a-8be4-fc913a4e6d6c
