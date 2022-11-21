#pragma once
#include "song.h"
#include "playlist.h"
#include "node.h"

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>

class Graph {
    public: 
        Graph() = default;
        Graph(vector<Playlist>& playlists);
        void analyze_all_playlists(vector<Playlist>& playlists);
        void analyze_playlist(Playlist& playlist);
        Node* GetNode(string artist);
        map<Node*, double>& FindNeighbors(string artist);
    private:
        void update_graph(Playlist& playlist);
        unordered_map<string, Node> _graph;
};