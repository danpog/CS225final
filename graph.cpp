#include "graph.h"
#include "song.h"
#include "playlist.h"
#include "node.h"

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>

using namespace std;

Graph::Graph(vector<Playlist>& playlists)    {
    analyze_all_playlists(playlists);
}

void Graph::analyze_all_playlists(vector<Playlist>& playlists) {
    for(Playlist playlist: playlists) {
        analyze_playlist(playlist);
    }
    /*while (!playlists.empty()) {
        Playlist& playlist = playlists[0];
        analyze_playlist(playlist);
        playlists.erase(playlists.begin());
    }*/
}

// Adds a playlist to a graph 
//      -> Updates the artists popular songs
//      -> Adds all the neighbors (undirected)
void Graph::analyze_playlist(Playlist& playlist) {
    update_graph(playlist);
    for (int i = 0; i < playlist.SongCount(); i++) {
        Song key_song = playlist.GetSong(i);
        _graph[key_song._artist].AddSong(key_song);
        for (int j = i; j < playlist.SongCount(); j++) {
            Song value_song = playlist.GetSong(j);
            if (key_song._artist != value_song._artist) {
                _graph[key_song._artist].AddNeighbor(&_graph[value_song._artist]);
            }
        }
    }
}

// Ensures that there is a node for each artist in playlist
void Graph::update_graph(Playlist& playlist) {
    string artist;
    for (int i = 0; i < playlist.SongCount(); i++) {
        artist = playlist.GetSong(i)._artist;
        _graph.insert(pair<string, Node>(artist, Node(artist)));
    }
}

Node* Graph::GetNode(string artist) {
    return &_graph[artist];
}

unordered_map<Node*, double>& Graph::FindNeighbors(string artist) {
    return _graph[artist].GetNeighbors();
}