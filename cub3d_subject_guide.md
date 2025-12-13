# CUB3D - Ray-Casting Game Project Guide

## 📋 Project Overview
**Goal:** Create a 3D graphical representation of a maze from first-person perspective using ray-casting principles, inspired by Wolfenstein 3D.

**Version:** 11.0

---

## 🎯 Learning Objectives
- Master ray-casting algorithms
- Work with miniLibX graphics library
- Develop 3D rendering techniques
- Practice C programming with graphics
- Explore practical mathematics applications

---

## 📚 Mandatory Requirements

### Program Specifications
- **Program name:** `cub3D`
- **Arguments:** A map file with `.cub` extension
- **Language:** C
- **Library:** miniLibX
- **Compilation:** Makefile with `-Wall -Wextra -Werror`

### Key Features Required

#### 🎮 Controls
- **WASD keys:** Move through the maze
- **Left/Right arrows:** Look left and right
- **ESC key:** Close window and exit
- **Red cross (X):** Close window and exit

#### 🖼️ Visual Elements
- **Wall textures:** Different textures for North, South, East, West walls
- **Floor/Ceiling colors:** Two different configurable colors
- **Smooth rendering:** No crashes, proper window management

#### 🗺️ Map Requirements
Map characters:
- `0` - Empty space
- `1` - Wall
- `N/S/E/W` - Player starting position and orientation

**Example valid map:**
```
111111
100101
101001
1100N1
111111
```

**Map rules:**
- Must be surrounded by walls
- Must be the last element in .cub file
- Spaces are valid parts of the map

### 📄 Scene File Format (.cub)

#### Texture Identifiers
```
NO ./path_to_north_texture    # North wall texture
SO ./path_to_south_texture    # South wall texture  
WE ./path_to_west_texture     # West wall texture
EA ./path_to_east_texture     # East wall texture
```

#### Color Configuration
```
F 220,100,0    # Floor color (RGB: 0-255)
C 225,30,0     # Ceiling color (RGB: 0-255)
```

#### Complete Example
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10000000000000001100000010001
10000000000000001101010010001
11000001110101011111011110N0111
11110111 1110101 101111010001
11111111 1111111 111111111111
```

---

## 🌟 Bonus Features (Optional)
*Note: Bonuses only evaluated if mandatory part is perfect*

- **Wall collisions** - Prevent walking through walls
- **Minimap system** - Show overhead view
- **Interactive doors** - Doors that open/close
- **Animated sprites** - Moving objects in game
- **Mouse rotation** - Look around with mouse

---

## 🛠️ Technical Requirements

### Authorized Functions
```c
// System functions
open, close, read, write, printf, malloc, free, perror, strerror, exit

// Time function
gettimeofday()

// Math library (-lm)
All math library functions

// miniLibX library
All miniLibX functions
```

### Makefile Rules Required
- `$(NAME)` - Compile the program
- `all` - Same as $(NAME)
- `clean` - Remove object files
- `fclean` - Remove object files and executable
- `re` - Recompile everything
- `bonus` - Compile with bonus features

### Error Handling
- Must handle all misconfiguration cases
- Return `"Error\n"` followed by explicit error message
- No crashes (segfault, bus error, double free)
- Proper memory management (no leaks)

---

## 🚀 Development Tips

### Ray-Casting Basics
1. Cast rays from player position at different angles
2. Find wall intersections
3. Calculate wall height based on distance
4. Apply textures based on wall orientation
5. Handle fish-eye effect correction

### Common Challenges
- **Memory management** - Always free allocated memory
- **Parsing** - Handle various .cub file formats correctly  
- **Texture mapping** - Apply correct textures to walls
- **Movement** - Smooth player movement and rotation
- **Error handling** - Comprehensive input validation

### Testing Strategy
- Test various map configurations
- Verify all controls work properly
- Check error cases and edge conditions
- Test window management (minimize, resize, etc.)
- Validate memory usage with valgrind

---

## 📝 Evaluation Points

### Must Work Perfectly
- All mandatory features implemented
- No crashes or undefined behavior
- Proper error handling
- Clean compilation with required flags
- Follows coding standards (Norm)

### Common Mistakes to Avoid
- Memory leaks
- Segmentation faults
- Incorrect file parsing
- Missing error messages
- Non-smooth window management
- Improper texture mapping

---

## 🎓 Educational Value
This project teaches:
- **Graphics programming** - Working with pixels and rendering
- **Mathematics application** - Trigonometry, geometry, vectors
- **Algorithm optimization** - Efficient ray-casting
- **File parsing** - Structured data handling
- **Event handling** - Keyboard/mouse input processing

---

*Remember: This project is about understanding ray-casting fundamentals, not creating a commercial game. Focus on solid implementation of core concepts!*
