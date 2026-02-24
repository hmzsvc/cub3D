# Cub3D - A Raycasting Engine

*This project has been created as part of the 42 curriculum by hasivaci, huozturk.

## Description
Cub3D is a C-based graphic engine that mimics the visual style of early 90s first-person shooters like Wolfenstein 3D. The project's core objective is to translate a 2D map into a pseudo-3D perspective using the Raycasting technique. 

Key features include:
- DDA (Digital Differential Analyzer) algorithm for fast and precise wall intersection detection.
- Dynamic Perspective: Implementation of a 60-degree Field of View (FOV) with fisheye distortion correction.
- Texture Mapping: Applying specific textures (North, South, East, West) based on the ray's impact side.
- Robust Parsing: A custom parser to validate .cub maps, ensuring they are closed by walls and contain valid RGB colors.

## Instructions

### Compilation
The project uses a standard Makefile. To build the executable, run:

make

### Execution
Launch the game by passing a valid configuration file:
./cub3D maps/map.cub

### Controls
- W / S: Move Forward / Backward
- A / D: Strafe Left / Right
- Left / Right Arrows: Rotate Camera
- ESC / Window Close Button: Exit Game

## Resources

### Tutorials and Articles
- https://stackoverflow.com/questions/1668029/confused-in-dda-algorithm-need-some-help
- https://matematiksel.site/tag/trigonometri/
- https://www.geeksforgeeks.org/computer-graphics/dda-line-generation-algorithm-computer-graphics/
- https://ismailassil.medium.com/ray-casting-c-8bfae2c2fc13
- 

### AI Usage
Artificial Intelligence was utilized during this project for the following purposes:
- Conceptual Visualization: Explaining the DDA stepping logic and the relationship between delta distances and side distances.
- Debugging and Standards: Identifying Norminette violations (specifically indentation and tab issues) and refining the collision detection logic.
- Documentation: Structuring the technical explanations and formatting this README file to meet 42 curriculum requirements.
