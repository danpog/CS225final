# Spotify Recommendation Algorithm

Derived from https://www.aicrowd.com/challenges/spotify-million-playlist-dataset-challenge

Group: Edan Elazar, Eli Gidron, & Daniel Pogrebinsky


# Running instructions

## Configuration (Cmake)

For this project we are using CMake rather than just make. To do this you need to run the following in the base directory.  (CS225Final)

`mkdir build`

`cd build`

This first makes a new directory in your assignment directory called build. This is where you will actually build the assignment and then moves to that directory. This is not included in the provided code since we are following industry standard practices and you would normally exclude the build directory from any source control system.

Now you need to actually run CMake as follows.


`cmake ..`

This runs CMake to initialize the current directory which is the build directory you just made as the location to build the assignment. 

## Execution 

**Main**

Run `make main`

This will execute the bulk of the project, it will build the graphs. The following terminal commands can be used with the varying effects:

**Testing**

Run the following commands:

`make test`

`./test`
