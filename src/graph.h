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
        void save_graph(string file);

        unordered_map<string, Node>& getGraph() { return _graph; };
        Node* GetNode(string artist);
        unordered_map<Node*, int>& FindNeighbors(string artist);
        
        Playlist CreatePlaylist(int num_songs, vector<Song>& preferences);
        bool SendPlaylistToSpotify(Playlist& playlist, string uid, string playlist_id);
        string CreateSpotifyPlaylist(string user_id, string token, string name, string description, bool pub);

        vector<string> Dijkstras(string source, string dest);
        
        string somethingNew(string artist1, int distance);
        bool similarity(string artist1, string artist2, int distance);

    private:
        void analyze_playlist(Playlist& playlist);
        bool similarity(string artist1, string artist2, int distance, vector<string>& path);
        void update_graph(Playlist& playlist);
        unordered_map<string, Node> _graph;
        bool RecurseDFS(Node* source, int num_songs, int depth, vector<Song>& playlist, unordered_map<string, bool>& visited, int song_tier);
};