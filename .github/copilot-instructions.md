# cub3D - AI Coding Agent Instructions

## Project Overview
This is a **cub3D** raycasting engine (42 school project) implementing a Wolfenstein 3D-style first-person view using the **DDA (Digital Differential Analysis)** algorithm. Written in C with MinilibX graphics library.

## Critical: 42 Norm Compliance (v4.1)

**ALL code MUST follow these strict rules:**
- **FORBIDDEN:** `for`, `do...while`, `switch`, `case`, `goto`, ternary operator `?:`, VLA
- Max 25 lines per function (excluding braces)
- Max 4 parameters per function
- Max 5 functions per `.c` file
- Max 5 variables per function scope
- All variables declared at function start (C89 style)
- 80 characters max per line
- TAB indentation (real tab character, width 4)
- snake_case only (no camelCase)
- Use `while` loops instead of `for`

**Example of compliant loop:**
```c
int i;

i = 0;
while (i < WIDTH)
{
    // code here
    i++;
}
```

## Architecture & Key Files

### Core Structure
- `inc/game.h` - Main header with all structs, defines, and function prototypes
- `srcs/main.c` - Game initialization and main loop
- `srcs/raycast.c` - **DDA raycasting algorithm** (critical file)
- `srcs/player.c` - Player movement and collision detection
- `srcs/minimap.c` - Minimap rendering (scaled version of main view)
- `srcs/map/` - Map parsing and texture loading

### Critical Constants (game.h)
- `BLOCK 64` - Grid size (all textures MUST be 64x64 pixels)
- `WIDTH 1280`, `HEIGHT 720` - Window dimensions
- `DEBUG 0/1` - Toggle debug visualization (draws rays, map grid)

### Data Flow
1. **Parse map** (`map_read.c`) → validates `.cub` file format
2. **Load textures** (`load_texture.c`) → XPM files via `mlx_xpm_file_to_image()`
3. **Main loop** (`draw_loop()`) → calls `render_frame()` every frame
4. **Raycasting** (`render_frame()`) → DDA algorithm projects 3D view
5. **Minimap overlay** (`draw_minimap()`) → scaled top-down view

## Raycasting Implementation Details

### DDA Algorithm (raycast.c)
The engine uses **grid-based DDA** to avoid pixel-by-pixel ray stepping:

```c
// Key concept: Step through grid cells, not pixels
if (side_x < side_y)  // Which grid boundary is closer?
{
    side_x += delta_x;  // Move to next vertical grid line
    map_x += step_x;    // Advance grid X
    side = 0;           // Hit vertical wall (EAST/WEST)
}
else
{
    side_y += delta_y;  // Move to next horizontal grid line
    map_y += step_y;    // Advance grid Y
    side = 1;           // Hit horizontal wall (NORTH/SOUTH)
}
```

### Texture Mapping
- `side == 0` → Vertical wall → Use EAST/WEST texture
- `side == 1` → Horizontal wall → Use NORTH/SOUTH texture
- Texture selection based on ray direction sign (`dir_x > 0` = facing east)

### Common Pitfalls
1. **Texture bleeding between walls:** Caused by using `fmod()` directly on world coords. Instead, calculate position relative to grid block:
   ```c
   wall_x = hit_y - (map_y * BLOCK);  // Position within block
   ```

2. **Fisheye effect:** Always apply perpendicular distance correction:
   ```c
   dist = dist * cos(angle - player.angle);
   ```

3. **Uninitialized variables:** In raycasting, always initialize `prev_x`, `prev_y` before loops

## Build & Development

### Build Commands
```bash
make           # Compile project
make clean     # Remove objects
make fclean    # Remove objects + executable
make re        # Recompile everything
./cub3D maps/maps.cub  # Run with map file
```

### Dependencies
- **libft** (`lib/libft/`) - Custom C standard library
- **get_next_line** (`lib/get_next_line/`) - Line reading utility
- **MinilibX** (`minilibx-linux/`) - Graphics library (X11)

### Allowed External Functions
```c
// Graphics
mlx_init, mlx_new_window, mlx_new_image, mlx_pixel_put, mlx_put_image_to_window
mlx_xpm_file_to_image, mlx_get_data_addr, mlx_destroy_image, mlx_destroy_window

// Standard
open, close, read, write, printf, malloc, free, perror, strerror, exit

// Math
All functions from -lm (math.h): cos, sin, sqrt, fabs, floor, fmod, pow, atan2
gettimeofday
```

## Map File Format (.cub)

```
NO assets/map/wall-64x64.xpm   # North texture (must be 64x64 XPM)
SO assets/map/wall4.xpm         # South texture
WE assets/map/wall3.xpm         # West texture
EA assets/map/wall2.xpm         # East texture

F 220,100,0                     # Floor RGB color
C 225,30,0                      # Ceiling RGB color

111111111111111
100000000000001
100N00100000001  # N/S/E/W = player start position
111111111111111
```

## Global Game Singleton Pattern

This project uses a **static singleton** to avoid global variables (Norm compliant):

```c
t_game *global_game(void)
{
    static t_game *game;
    
    if (!game)
        game = (t_game *)ft_calloc(1, sizeof(t_game));
    return (game);
}
```

**Usage:** Call `global_game()` in any file to access game state.

## Debugging

### Enable Debug Mode
Set `#define DEBUG 1` in `inc/game.h` to visualize:
- Red rays shooting from player
- Blue grid outlines of map blocks
- Green player square

### Common Debug Patterns
```c
if (DEBUG)
    printf("Player: x=%f y=%f angle=%f\n", player->x, player->y, player->angle);
```

## Testing & Validation

### Map Validation Rules
- Map must be surrounded by walls (`1`)
- Only valid characters: `0` (floor), `1` (wall), `N/S/E/W` (player start)
- Exactly one player spawn point
- Rectangular shape (no gaps)

### Performance Expectations
- Target: 60 FPS at 1280x720
- DDA is O(n) per ray where n = grid cells crossed
- Each frame casts `WIDTH` (1280) rays

## Project-Specific Conventions

### File Header
All `.c` and `.h` files start with 42 standard header:
```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename.c                                         :+:      :+:    :+:   */
```

### Memory Management
- Always free allocated memory (check with `valgrind --leak-check=full`)
- MLX cleanup order: image → window → display → mlx pointer

### Error Handling
- Return `0` on success, `1` on error (or use `exit()`)
- Use `perror()` or custom error messages to stderr

## When Editing Code

1. **Check function limits:** Max 25 lines, 4 params, 5 variables
2. **Verify Norm:** Run `norminette` on modified files
3. **Test compilation:** `make re` after changes
4. **Test with map:** `./cub3D maps/maps.cub`
5. **Check leaks:** `valgrind ./cub3D maps/maps.cub`

## Reference Implementation Notes

The current `render_frame()` uses optimized DDA that:
- Avoids pixel-by-pixel iteration (too slow)
- Calculates exact grid intersections
- Properly handles texture coordinate mapping
- Prevents texture bleeding between adjacent walls

When modifying raycasting, maintain these properties or performance will degrade significantly.
