# Ray tracer framework in C++ for Computer Graphics

This is a C++ framework for a ray tracer. It was created for the Computing Science course "Computer Graphics" taught at the University of Groningen.

This framework is inspired by and uses (some) code of the ray tracer framework of Bert Freudenberg which unfortunately is no longer available.

## New Features

The following features have been added to the framework:

- Width and height of the output image are setable
- Shift and rotate the camera in 3d space
- Changeable FOV of the camera

Another feature we were eager about is the ability to render animations. For this we made a python script that changes the json config of the scene, run the framework and saves each frame. After that we used ffmpeg to create a gif from the frames.

## Examples

<p align="center">
    <img src="./scenes/1_animation/out.gif" loop></img><br/>
    <caption>
        Animation of rotating in a pond around multiple transpanrent spheres.
    </caption>
</p>
<p align="center">
    <img src="./scenes/2_animation/out.gif" loop></img><br/>
    <caption>
        Animation of a transparent sphere levitating over the pond.
    </caption>
</p>

## Thoughts

This was an interesting project to work and to discover.

A few interesting things we learned:

- If you place a texture on a sphere, you can see it from the inside. This way you can use a 360 panorama image and make it look like you are observing the panorama.

- Shifting the camera and rotating it was a bit tricky. We used quatrenions to rotate the camera. We also had to change the ray direction to make it work.

- We couldn't manage to place objects inside the glass spheres and make them look like they are inside. Our initial idea was to have marbles with colored glass inside, rather than just glass spheres. We suspect this is because of the way the light reflect off the object when it is inside the sphere.

## Compiling the code

We provided a [CMakeLists.txt](CMakeLists.txt) file to be used with `CMake`.
This can be used in the following way (on the command line/shell):

```
# cd into the main directory of the framework
mkdir build   # create a new build directory
cd build      # cd into the directory
cmake ..      # creates a Makefile
# Compiling the code can then be done with:
make
# or
make -j4      # replacing 4 with the number of cores of your computer
```

A simple build script has also been provided in `build.sh`.

**Note!** After adding new `.cpp` files, `cmake ..` needs to be called again or you might get linker errors.

## Running the Ray tracer
After compilation you should have the `./build/ray` executable.
This can be used like this:
```
./build/ray <path to .json file> [output .png file]
# when in the build directory:
./build/ray ../scenes/other/scene01.json
```
Specifying an output is optional and by default an image will be created in
the same directory as the source scene file with the `.json` extension replaced
by `.png`.

## Running the python script

For this you need to have python3 and ffmpeg installed.

Copy of python files for each scene are in the respective scene folder. The python script has to placed into the root of the repository and can be run like this:

```
python3 anim.py
```

It will generate a folder for each scene with the frames and a gif.

## Description of the included files

### Scene files
* `scenes/*.json`: Scene files are structured in JSON. If you have never
    worked with JSON, please see [here](https://en.wikipedia.org/wiki/JSON#Data_types_and_syntax) or [here](https://www.json.org).

    Take a look at the provided example scenes for the general structure.
    You are encouraged to define your own scene files for testing your
    application and for participating in the competition.

### The ray tracer source files

* `main.cpp`: Contains main(), starting point. Responsible for parsing
    command-line arguments.

* `raytracer.cpp/.h`: Ray tracer class. Responsible for reading the scene description, starting the ray tracer and writing the result to an image file.

* `scene.cpp/.h`: Scene class. Contains code for the actual ray tracing.

* `image.cpp/.h`: Image class, includes code for reading from and writing to PNG files.

* `light.h`: Light class. Plain Old Data (POD) class. A colored light at a position in the scene.

* `ray.h`: Ray class. POD class. Ray from an origin point in a direction.

* `hit.h`: Hit class. POD class. Intersection between an `Ray` and an `Object`.

* `object.h`: virtual `Object` class. Represents an object in the scene.
    All your shapes should derive from this class.

* `shapes (directory/folder)`: Folder containing all your shapes.

* `sphere.cpp/.h (inside shapes)`: Sphere class, which is a subclass of the `Object` class. Represents a sphere in the scene.

* `triple.cpp/.h`: Triple class. Represents a three-dimensional vector which is used for colors, points and vectors.
    Includes a number of useful functions and operators, see the comments in `triple.h`.
    Classes of `Color`, `Vector`, `Point` are all aliases of `Triple`.

### Supporting source files

* `lode/*`: Code for reading from and writing to PNG files,
    used by the `Image` class.
     lodepng is created by Lode Vandevenne and can be found on [github](https://github.com/lvandeve/lodepng).

* `json/*`: Code for parsing JSON documents.
    Created by Niels Lohmann and available under the MIT license on  [github](https://github.com/nlohmann/json).
    **Recommended:** Especially take a look at their README for more info on how to work with JSON files.
