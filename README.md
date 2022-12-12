# Spotify Recommendation Algorithm

Derived from https://www.aicrowd.com/challenges/spotify-million-playlist-dataset-challenge

Group: Edan Elazar, Eli Gidron, & Daniel Pogrebinsky

Link to Video: https://mediaspace.illinois.edu/media/t/1_6thg4dro

## Class Organization

|  Folder Name   | Description                         
|----------------|-------------------------------|
|entry| Entry point for the code |
|src             | Contains the structure of project: Node, Graph, Playlist, and Song   classes, along with utility         |
|tests| Contains the files for testing various functions of the code            
|csvdata| Contains the data of 10,000 playlists in csv format
|JSON| Support for JSON configs

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

Run the following commands:

`make main`

`./main`

This will open a command-line interface. The following commands can be used with the varying effects:
|  Command   | Description                         
|----------------|-------------------------------|
|break|ends the program |
|generate|generates a new graph |
|find neighbors|prints the neighbors of a given artist|
|path between|uses Dijkstra's algorithm to find the shortest weighted path between two artists |
|similarity|uses DFS to check if an artist is within a specified distance of another artist |
|something new|recommends a new artist at a specified distance (1-10, 1 = most similar, 10 = least similar)|
|make playlist|generates a playlist from three song preferences, optionally push to spotify |
|help|show command list|

Note that some of these commands will prompt you for additional parameters. Additionally, the commands listed above are case and whitespace insensitive, and will ignore non-alphabetic characters. 

All output will be printed to the terminal. 

Run the command `generate` in order to generate a graph, no other graph-related commands will function until this is done. When prompted to choose a number of playlists, choose a number between 1-10 (inclusive), as there are 10,000 playlists included in the repository.

**Testing**

Run the following commands:

`make test`

`./test`

## Spotify Implementation
We provide the optional functionality of sending a generated playlist to your Spotify account. Unfortunately, this cannot be done in the default CS225 container, and requires running two extra commands to enbale curl commands:

`apt update`

`apt install curl`

Without curl installed, the Spotify implementation will unfortunately not work and will cause the program to freeze, requiring it to be stopped by the user. 

Once these commands have been run, follow the following instructions to find your User ID and OAuth Token (or "Token") as they will be needed to grant permission to create and modify a playlist on your account.

**Obtaining User ID**

Go to spotify.com, and sign in if you are not signed in already. Then, navigate to your username in the top-right corner, click it, and select "Account" (see image). 

<img width="239" alt="Screen Shot 2022-12-11 at 18 58 17" src="https://user-images.githubusercontent.com/32343221/206940271-fdcd0d1c-d1ae-4c24-9b16-205e353fbadd.png">

From there, in the "Account overview" section, your User ID is the string in the "Username" field of the "Profile" section (see image).

![Screen Shot 2022-12-11 at 19 01 53](https://user-images.githubusercontent.com/32343221/206940700-aaa84767-f43d-423b-83c6-0cebed439df9.png)


**Obtaining Token**

To obtain your Token, go to the following website https://developer.spotify.com/console/post-playlists/. There will be a green button labeled "GET TOKEN"- select this button (see image).

<img width="1426" alt="Screen Shot 2022-12-11 at 19 08 35" src="https://user-images.githubusercontent.com/32343221/206940952-804f7ba8-7138-4e55-a20e-665065f896da.png">

Then, select the "playlist-modify-private" scope, and click "REQUEST TOKEN" at the bottom (see image).

<img width="595" alt="Screen Shot 2022-12-11 at 19 09 35" src="https://user-images.githubusercontent.com/32343221/206941067-bec6db99-97d0-4b1f-bad5-285c7b3b7d1d.png">

You may be prompted to sign in or grant certain permissions. Please do so. Finally, it will return you back, but with the "OAuth Token" field filled. The entire string in this field is your Token. It should look something like

"BQBXKvzPUVcWTbvhbMqZXFDU8vX_GvapUlgyDRUm0GyEdzC3XIH46az7XqPQnYVbzGtkYWjGPhhByximl_bP9cNixHiloN9Q2TCoiLSpnxRN4bc9THTLbZgC9DgN2RmQYpUaz8ave2-qzaVTxHIiLYguH7pifFj6K54P240vh0hgm3CnYdRwZVL8SmbwkMVorOP6dPuskCtikO29lh15g6thV5E_-1yOiqg"

(without the quotes). Note that this Token expires every hour, and you can get a new one by repeating these steps.

**Using The Interface**

Finally, after generating a playlist using the command `make playlist` in the command-line interface, you will be asked if you want to send the playlist to Spotify. It will then prompt you for your User ID and Token, which you simply input, then hit return. After creating the playlist, upon success, the program will print a link to your new playlist in the terminal. Again, the Token expires every hour, so be sure to refresh it. 
