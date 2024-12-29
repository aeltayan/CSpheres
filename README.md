# Raycasting Renderer in C

## Project Description

This project implements a raycasting renderer in C that takes an input file to define a scene with 3D spheres, a light source, and rendering parameters. The program simulates a camera, computes ray-sphere intersections, applies lighting and shading, and outputs a rendered image in the PPM format. The resulting image displays a 3D scene with shaded spheres.

## Input File Format

The input file must follow this specific format:

1. **Image Dimensions:**

   - `<image width (int)> <image height (int)>`
     - Specifies the resolution of the output image in pixels.

2. **Viewport and Cam Fera Parameters:**

   - `<viewport height (float)>`
     - Defines the height of the viewport in world coordinates.
   - `<focal length (float)>`
     - Distance from the camera to the viewport.

3. **Lighting Parameters:**

   - `<light position (3float)> <light brightness (float)>`
     - Position of the light source in world coordinates and its brightness factor.

4. **Color Palette:**

   - `<number of colors m (int)>`
     - Number of colors in the palette.
   - `<color 1 (HEX)> <color 2 (HEX)> ... <color m (HEX)>`
     - List of `m` colors in hexadecimal format.

5. **Background Color:**

   - `<background color index (int)>`
     - Index of the background color in the color palette.

6. **Spheres in the Scene:**

   - `<number of spheres n (int)>`
     - Number of spheres in the scene.
   - `<sphere 1 position (3float)> <sphere 1 radius (float)> <sphere 1 color index (int)>`
   - `<sphere 2 position (3float)> <sphere 2 radius (float)> <sphere 2 color index (int)>`
   - ...
   - `<sphere n position (3float)> <sphere n radius (float)> <sphere n color index (int)>`
     - For each sphere: position in world coordinates, radius, and color index from the color palette.

### Example Input File

```
800 600
2.0
1.0
0.0 10.0 -10.0 1.5
3
#FF0000 #00FF00 #0000FF
0
2
0.0 0.0 -5.0 1.0 0
2.0 1.0 -6.0 0.5 1
```

## Output

The program generates an image in the PPM format with the filename `render.ppm`. You can view this file using any image viewer that supports PPM files. I reccomend using the website called "PPM Viewer-Rhodes College".

## How to Run the Program

### Step 1: Install GCC

Ensure that GCC is installed on your system.

### Step 2: Install Make

Ensure that `make` is installed on your system. For Linux and macOS, it is usually pre-installed. For Windows, you have to install it.


### Step 3: Compile the Program

A Makefile is provided to simplify the build process. Run the following command in the terminal:

```bash
make
```

This will compile the program and create an executable named `FS_assg`.

### Step 4: Run the Program

Run the program by specifying the input file:

```bash
./FS_assg <input_file> <render.ppm>
```
The rendered image will be saved as `render.ppm` in the same directory.


## Dependencies

- GCC compiler 
- Make utility.
- No external libraries are required; the program uses standard C.

## Notes

- Ensure the input file strictly adheres to the specified format.
- If the input file is malformed, the program may terminate unexpectedly or produce incorrect results.
- Adjust the Makefile as needed if using a different compiler or setup.

## Contact

For any issues or questions, please open a ticket in the repository or contact the developer directly.

