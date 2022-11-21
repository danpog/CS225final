#include "graph.h"
#include "song.h"
#include "playlist.h"
#include "node.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>

using namespace std;

Graph::Graph(vector<Playlist>& playlists)    {
    for(Playlist playlist: playlists) {
        analyze_playlist(playlist);
    }
}

void Graph::analyze_all_playlists(vector<Playlist>& playlists) {
    for(Playlist playlist: playlists) {
        analyze_playlist(playlist);
    }
}

void Graph::analyze_playlist(Playlist& playlist) {
    update_graph(playlist);
    for (int i = 0; i < playlist.SongCount(); i++) {
        Song key_song = playlist.GetSong(i);
        _graph[key_song.GetArtist()].AddSong(key_song);
        for (int j = i; j < playlist.SongCount(); j++) {
            Song value_song = playlist.GetSong(j);
            if (key_song.GetArtist() != value_song.GetArtist()) {
                _graph[key_song.GetArtist()].AddNeighbor(&_graph[value_song.GetArtist()]);
            }
        }
    }
}

void Graph::update_graph(Playlist& playlist) {
    string artist;
    for (int i = 0; i < playlist.SongCount(); i++) {
        artist = playlist.GetSong(i).GetArtist();
        _graph.insert(pair<string, Node>(artist, Node(artist)));
    }
}

Node* Graph::GetNode(string artist) {
    return &_graph[artist];
}

map<Node*, double>& Graph::FindNeighbors(string artist) {
    return _graph[artist].GetNeighbors();
}