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

#include <json/json.h>

using namespace std;

vector<Playlist> parse(string filename)    {
    if (filename.empty()) {
        throw std::invalid_argument("No file name");
    }
    
    vector<Playlist> all_playlists = vector<Playlist>();
   
    Json::Value all_playlists_json; 

    std::ifstream playlist_file(filename);
    playlist_file >> all_playlists_json;

    // cout << all_playlists_json; //This will print the entire json object.
    cout<<all_playlists_json["info"]; //Prints the value for "Anna"
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
            Song s = Song(name, album_name, artist);
            p.AddSong(s);
        }
        all_playlists.push_back(p);
    }

    return all_playlists;
}

int main()  {
    vector<Playlist> a = parse("Songs0-600.json");
    /*
    //print first playlist
    vector<Song> first = a[0].GetSongs();
    for (Song i : first) {
        std::cout << i << std::endl;
    }
    */
    
    //test graph class
    time_t t = time(0);
    Graph graph = Graph();
    for(Playlist playlist: a) {
        graph.analyze_playlist(playlist);
    }
    t = time(0) - t;
    string artist1 = a[0].GetSong(0).GetArtist();
    cout << endl << artist1 << endl;
    vector<std::pair<Node*, double>> neighbors = graph.FindNeighbors(artist1);
    for (std::pair<Node*, double> neighbor: neighbors) {
        if (neighbor.second > 3) {
            cout << neighbor.first->GetArtist() << "  " << neighbor.second << endl;
        }
    }
    cout << endl << "Graph generated in "<< t << "s" << endl;
    return 0;
}




