## Winston's Shield from Overwatch

<img src="assets/demo.gif" width="700px">

This project emulates Winston's shield from overwatch using OpenGL. Some key features are:
- Intersection shader using a depth pass and smooth edges
- Variable rim lighting emulating the Fresnel effect
- Depth linearization for position-agnostic VFX

I made a video explaining all of the techniques and theory used to create this, which can be found [here!](https://youtu.be/SiCNFMhDZ1o)

If you want to run it yourself, you should be able to just run the executable in <code>/bin</code>

### Controls
- <code>WASD</code>: Move camera
- <code>LEFT, RIGHT</code> Move bubble through the scene

### Dependencies

- GLFW
- GLAD
- GLM
- Assimp
