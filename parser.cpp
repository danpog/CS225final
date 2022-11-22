#include "song.h"
#include "playlist.h"
#include "node.h"
#include "graph.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

#include <json/json.h>

using namespace std;

void PrintSong(Song& s) {
    cout << s._name << endl << s._album << endl << s._artist << endl << endl;
}

vector<Playlist> parse(string filename)    {
    if (filename.empty()) {
        throw std::invalid_argument("No file name");
    }
    
    vector<Playlist> all_playlists = vector<Playlist>();
    Json::Value all_playlists_json; 

    std::ifstream playlist_file(filename);
    playlist_file >> all_playlists_json;

    // cout << all_playlists_json; // This will print the entire json object.
    cout << all_playlists_json["info"]; 
    Json::StreamWriterBuilder builder;
    // builder["indentation"] = ""; // If you want whitespace-less output

    for(Json::Value playlist: all_playlists_json["playlists"]) {
        const std::string name = Json::writeString(builder, playlist["name"]);
        Json::Value pid = playlist["pid"];
        int id = pid.asInt();
        Playlist p = Playlist(name);
        p.SetID(id);
        for (Json::Value track: playlist["tracks"]) {
            std::string name = Json::writeString(builder, track["track_name"]);
            std::string artist = Json::writeString(builder, track["artist_name"]);
            std::string album_name = Json::writeString(builder, track["album_name"]);
            std::string uri = Json::writeString(builder, track["track_uri"]);
            Song s = Song(name, album_name, artist, uri);
            p.AddSong(s);
        }
        all_playlists.push_back(p);
    }

    return all_playlists;
}

int main()  {
    chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    Graph graph = Graph();
    vector<Playlist> a;

    // Creating the graph
    for (int i = 0; i < 000; i += 1000) {
        a = parse("./mpdslices/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        graph.analyze_all_playlists(a);
    }
    
    chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t = ((t_final-t_start)/1000.0).count();


    string artist1 = a[0].GetSong(0)._artist;
    cout << endl << artist1 << endl;

    unordered_map<Node*, double> neighbors = graph.FindNeighbors(artist1);
    unordered_map<Node*, double>::iterator it;
    /*Node* n = graph.GetNode(artist1);
    for (int i = 0; i < n->SongCount(); i++)    {
        cout << n->RequestSong() << endl;
    }*/
    for (it = neighbors.begin(); it != neighbors.end(); it++) {
        if (it->second > 20) {
            cout << it->first->GetArtist() << " " << it->second << endl;
        }
    }
    /*for (Song& b : a[0].GetSongs()) {
        cout << std::hash<Song&>{}(b) << endl;
    }*/
    cout << endl << "Graph generated in "<< t << "s" << endl;
    return 0;
}




