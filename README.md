# OpenGL Tutorial (APU_SSC)

Code for the Introduction to Graphics with OpenGL session. Each folder is its own small program, and each one builds on the concepts from the one before it.

## Build

Requires CMake, Ninja, clang (with lld) and vcpkg (with `VCPKG_ROOT` set), and a compiler new enough for C++23 `std::println`

`run.sh` and the linker flags in `CMakeLists.txt` are set up for Windows with MSYS2 clang, you may modify it based on your setup

```bash
./run.sh 07_loader
```

Pass the chapter folder name to build and run it like above. Dependencies (glfw, glad, glm) should be installed by vcpkg on the first build.

## Chapters

- `00_hello` - use this to check your setup and that build works
- `01_window` - glfw window and opengl context
- `02_input` - keyboard callback and mouse polling
- `03_triangle` - shaders, vbo and vao
- `03b_colors` - a second vertex attribute
- `04_shader` - shader class, loading shaders from files, uniforms
- `05_ebo` - the index buffer
- `06_transforms` - model, view and projection matrices
- `07_loader` - advanced model loading
- `08_lighting` - advanced shaders and normals attribute
- `09_viewer` - everything all together

## Resources

- [learnopengl.com](https://learnopengl.com)
- [songho.ca](https://www.songho.ca/opengl/index.html)
