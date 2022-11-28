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
#include <cstring>

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

Playlist Graph::CreatePlaylist(int num_songs, vector<Song>& preferences)    {
    vector<Song> playlist;
    for (int depth = 0; depth < num_songs; depth++) {
        for (Song& s : preferences) {
            unordered_map<string, bool> visited;
            Node* node = GetNode(s._artist);
            if (node -> SongCount() == 0) {
                continue;
            }
            if (RecurseDFS(node, num_songs, depth, playlist, visited))  {
                return Playlist(playlist);
            }
        }
    }
    return Playlist(playlist);
}

bool Graph::RecurseDFS(Node* source, int num_songs, int depth, vector<Song>& playlist, unordered_map<string, bool>& visited)  {
    if ((int)playlist.size() >= num_songs)   {
        return true;
    }
    if (depth < 0)  {
        return false;
    }
    playlist.push_back(source -> RequestSong());
    visited[source -> GetArtist()] = true;
    unordered_map<Node*, double>& neighbors =  source -> GetNeighbors();

    unordered_map<Node*, double>::iterator it;
    for (it = neighbors.begin(); it != neighbors.end(); it++)  {
        if (visited[it -> first -> GetArtist()])   {
            continue;
        }
        if (RecurseDFS(it -> first, num_songs, depth - 1, playlist, visited))   {
            return true;
        }
    }
    return false;
}

bool Graph::SendPlaylistToSpotify(Playlist& playlist, string uid, string playlist_id) {
    const int PLAYLIST_ID = 3;
    const int URI_START = 6; 
    const int USER_TOKEN = 9;

    string command = "";
    fstream file("../command_test");
    string s;
    for (int i = 0; i < PLAYLIST_ID - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder playlist id
    command += playlist_id;
    for (int i = PLAYLIST_ID; i < URI_START - 1; i++)   {
        getline(file, s);
        command += s;
    }
    int count = 0;
    for (Song& song : playlist.GetSongs())  {
        string uri = song._uri.substr(1, song._uri.size() - 2);
        if (count == 0) {
            command += uri;
        }
        else    {
            string uri2 = "%2C" + uri;
            command += uri2;
        }
        count++;
    }
    getline(file, s);
    for (int i = URI_START; i < USER_TOKEN - 1; i++)    {
        getline(file, s);
        command += s;
    }
    getline(file, s); //temp uid
    command += uid;
    while (getline(file,s)) {
        command += s;
    }
    char char_array[command.length() + 1];
    strcpy(char_array, command.c_str());
    char* temp = char_array;
    system(temp);
    return true;
}

string Graph::CreateNewPlayList(string user_id, string token, string name, string description, bool pub) {
    const int USER_ID = 3;
    const int PLAYLIST_NAME = 7;
    const int PLAYLIST_DESCRIPTION = 12;
    const int PUBLIC = 16;
    const int USER_TOKEN = 19;

    string command = "";
    fstream file("../create_playlist");
    string s;

    for (int i = 0; i < USER_ID - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder user id
    command += user_id;
    for (int i = USER_ID; i < PLAYLIST_NAME - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder playlist name
    command += name;

     for (int i = PLAYLIST_NAME; i < PLAYLIST_DESCRIPTION - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder playlist description
    command += description;

     for (int i = PLAYLIST_DESCRIPTION; i < PUBLIC - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder public bool
    command += pub ? "true" : "false";

     for (int i = PUBLIC; i < USER_TOKEN - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder user token
    command += token;
    while (getline(file,s)) {
        command += s;
    }
    char char_array[command.length() + 1];
    strcpy(char_array, command.c_str());
    char* temp = char_array;
    system(temp);
    
    /*while (cin.get() != "{")    {

    }*/
    return "f";
}