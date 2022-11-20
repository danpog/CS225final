#pragma once
#include "song.h"
#include "playlist.h"
#include "node.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>

class Graph {
    public: 
        Graph() = default;
        void analyze_playlist(Playlist playlist);
        Node* GetNode(string artist);
        map<Node*, double>& FindNeighbors(string artist);
    private:
        void update_graph(Playlist playlist);
        map<string, Node> _graph;
};