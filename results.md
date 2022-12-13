# Results

Derived from https://www.aicrowd.com/challenges/spotify-million-playlist-dataset-challenge

Group: Edan Elazar, Eli Gidron, & Daniel Pogrebinsky

Link to Presentation Video: https://mediaspace.illinois.edu/media/t/1_6thg4dro

Link to Demo Video: https://mediaspace.illinois.edu/media/t/1_x5s62xas


## Leading Question

Given a dataset of playlists are we able to accurately construct our own playlist? 

Yes, we were able to accomplish this. The dataset that we are given is a list of playlists. Each of these playlists contains a set of songs along with some other information such as unique id, duration, album name. We constructed a relevant graph from the playlist data and then using various traversal methods to build a new playlist.

## Dijsktas Algorithm 

The purpose of Dijkstra's algorithm is to find the minimum path between two nodes. In this particular implementation we return the path that would be taken for the shortest path. This effectively models a link between two artists with their most correlated artists.
The structure of the graph is a directed, weighted graph. The weight of each node is correlated to the number of playlists that they appear with each other. However, because the weight increases with correlation (the path that we want to take) when computing the weights I use its inverse.

**Testing**

In the first section the graph down so that each node only has one connection. Then I check if it is able to find a path when a path exists and is unable to find a path when the path doesn’t exist. 
In the second section there is a larger graph where there are ties that need to be broken. For the provided test cases that are multiple routes that can be taken to get to the node. Each “correct” vector has the fastest path between the two nodes. 


## Modified Iterative Deepening Depth-First Search 

The purpose of the IDDFS was to generate a playlist by visiting neighboring nodes of a starting preference node and adding songs from the nodes we traverse. In this particular implementation, we accumulate these songs into a vector of songs until we reach the desired amount. As the graph is undirected, we maintain a map from string to boolean representing whether or not each node has been visited within that specific DFS. Furthermore, since we have multiple starting preference nodes, the DFS is run at depth k on each node before iterating to depth k+1. This ensures that nodes close to all of the preferencecs are visited roughly equally.  

**Testing**

We have two methods of testing this algorithm. The first is to use a smaller playlist dataset that we can parse manually and ensure that the expected results are present. We also built a graph by hand (instead of parsing a dataset), and ensured the the results were as expected. Finally, although qualititative, we ran the algorithm on a large dataset and noticed that the artists and songs chosen do indeed make sense, and inputted artists appear at the expected frequencies.

## DFS

The purpose of the DFS is to check whether two nodes are within a specified distance. It starts at a specific artist and recurses through the nodes until it either visited all the nodes or found the second artist. The max recursion depth is the specified distance to ensure that the artist must be found within that given distance.

**Testing**

The first test case for the DFS has a graph that we mannually build so that we can test the output of the function and veryify that it worked properly. The second test case uses the algorithm on a large dataset and verifies that the output makes sense for what we consider to be similar artists as well as very different ones.