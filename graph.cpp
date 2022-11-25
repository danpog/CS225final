#include "graph.h"
#include "song.h"
#include "playlist.h"
#include "node.h"

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <json/json.h>

using namespace std;


Graph::Graph(string file) {
    Json::Value all_nodes_json;
    std::ifstream graph_file(file);
    graph_file >> all_nodes_json;
    Json::StreamWriterBuilder builder;

    string artist;
    for (Json::Value node: all_nodes_json["nodes"]) {
        artist = Json::writeString(builder, node["artist"]);
        _graph.insert(pair<string, Node>(artist, Node(artist)));
    }

    Node* artist2;
    int frequency;
    string name;
    string album;
    string artist3;
    string uri;
    int frequency2;
    for (Json::Value node: all_nodes_json["nodes"]) {
        artist = Json::writeString(builder, node["artist"]);
        for (Json::Value neighbor: node["neighbors"]) {
            artist2 = &_graph[Json::writeString(builder, neighbor["artist"])];
            frequency = neighbor["frequency"].asInt();
            _graph[artist].AddNeighborPair(pair<Node*, int>(artist2, frequency));
        }
        for (Json::Value song: node["songs"]) {
            name = Json::writeString(builder, song["name"]);
            album = Json::writeString(builder, song["album"]);
            artist3 = Json::writeString(builder, song["artist"]);
            uri = Json::writeString(builder, song["uri"]);
            frequency2 = song["frequency"].asInt();
            Song s = Song(name, album, artist3, uri);
            _graph[artist].AddSongPair(s, frequency2);
        }
    }
}

Graph::Graph(vector<Playlist>& playlists)    {
    analyze_all_playlists(playlists);
}

void Graph::analyze_all_playlists(vector<Playlist>& playlists) {
    /*for(Playlist playlist: playlists) {
        analyze_playlist(playlist);
    }*/
    while (!playlists.empty()) {
        Playlist& playlist = playlists[0];
        analyze_playlist(playlist);
        playlists.erase(playlists.begin());
    }
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

unordered_map<Node*, int>& Graph::FindNeighbors(string artist) {
    return _graph[artist].GetNeighbors();
}

void Graph::save_graph(string file) {
    ofstream saved_graph;

    saved_graph.open(file);
    saved_graph << "{";
    saved_graph << "\"nodes\": [";

    unordered_map<Node*, int> neighbors;
    unordered_map<Node*, int>::iterator it_neighbors;
    unordered_map<string, Node>::iterator it;
    int neighbor_counter;
    int song_counter;

    for (it = _graph.begin(); it != _graph.end(); it++) {
        neighbors = _graph[it->first].GetNeighbors();
        saved_graph << "{";
        saved_graph << "\"artist\": " << it->first << ",";
        saved_graph << "\"neighbors\": [";
        neighbor_counter = 1;
        for (it_neighbors = neighbors.begin(); it_neighbors != neighbors.end(); it_neighbors++) {
            saved_graph << "{";
            saved_graph << "\"artist\": " << it_neighbors->first->GetArtist() << ", ";
            saved_graph << "\"frequency\": " << it_neighbors->second << "}";
            if (neighbor_counter < int(neighbors.size())) {
                saved_graph << ",";
            }
            neighbor_counter++;
        }
        saved_graph << "],";
        saved_graph << "\"songs\": [";
        song_counter = 1;
        vector<pair<Song,int>> songs = _graph[it->first].GetAllSongs();
        for (pair<Song,int> song: songs) {
                saved_graph << "{";
                saved_graph << "\"name\": " << song.first._name << ", ";
                saved_graph << "\"album\": " << song.first._album << ", ";
                saved_graph << "\"artist\": " << song.first._artist << ", ";
                saved_graph << "\"uri\": " << song.first._uri << ", ";
                saved_graph << "\"frequency\": " << song.second << "}";
            if (song_counter < int(songs.size())) {
                saved_graph << ",";
            }
            song_counter++;
        }
        saved_graph << "]";
        saved_graph << "},";
    }
    saved_graph << "{}";
    saved_graph << "]";
    saved_graph << "}";
    saved_graph.close();

    /*Json::Value graph_json; 
    std::ifstream graph_file(file);
    graph_file >> graph_json;
    Json::StreamWriterBuilder builder;
    std::string artist = Json::writeString(builder, graph_json["nodes"][0]["artist"]);
    std::string neighbor = Json::writeString(builder, graph_json["nodes"][0]["neighbors"][0]["artist"]);
    int frequency = graph_json["nodes"][0]["neighbors"][0]["frequency"].asInt();
    cout << endl << "artist: " << artist << " neighbor: " << neighbor << " frequency: " << frequency<< endl;*/
}