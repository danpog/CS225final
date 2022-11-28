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
        unordered_map<Node*, double>& FindNeighbors(string artist);
        unordered_map<string, Node> getGraph() { return _graph; };
        Playlist CreatePlaylist(int num_songs, vector<Song>& preferences);
        bool SendPlaylistToSpotify(Playlist& playlist, string uid, string playlist_id);
        string CreateNewPlayList(string user_id, string token, string name, string description, bool pub);
    private:
        void update_graph(Playlist& playlist);
        unordered_map<string, Node> _graph;
        bool RecurseDFS(Node* source, int num_songs, int depth, vector<Song>& playlist, unordered_map<string, bool>& visited);
};