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
        Graph(string file);
        void analyze_all_playlists(vector<Playlist>& playlists);
        void analyze_playlist(Playlist& playlist);
        Node* GetNode(string artist);
        unordered_map<Node*, int>& FindNeighbors(string artist);
        unordered_map<string, Node> getGraph() { return _graph; };
        void save_graph(string file);
    private:
        void update_graph(Playlist& playlist);
        unordered_map<string, Node> _graph;
};