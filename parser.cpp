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
    for (int i = 0; i < 10000; i += 1000) {
        a = parse("../spotify_million_playlist_dataset/data/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        graph.analyze_all_playlists(a);
    }
    
    chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t = ((t_final-t_start)/1000.0).count();

    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(20, false);
    }

    chrono::milliseconds t_final_2 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t2 = ((t_final_2-t_final)/1000.0).count();


    /*for (Song& b : a[0].GetSongs()) {
        cout << std::hash<Song&>{}(b) << endl;
    }*/
    chrono::milliseconds t_start2 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    graph.save_graph("saved_graph.json");
    chrono::milliseconds t_final3 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t3 = ((t_final3-t_start2)/1000.0).count();
    chrono::milliseconds t_start3 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    Graph graph2 = Graph("saved_graph.json");
    chrono::milliseconds t_final4 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t4 = ((t_final4-t_start3)/1000.0).count();
    cout << endl << "Graph generated in "<< t << "s" << endl;
    cout << endl << "Neighbor trimmed in "<< t2 << "s" << endl;
    cout << endl << "Graph saved in "<< t3 << "s" << endl;
    cout << endl << "New graph generated in "<< t4 << "s" << endl;
    vector<string> artists;
    artists.push_back("\"Lost Frequencies\"");
    artists.push_back("\"Michael Jackson\"");
    artists.push_back("\"Kygo\"");
    for (string artist1 : artists) {
        std::cout << artist1 << std::endl;
        unordered_map<Node*, double> neighbors = graph.FindNeighbors(artist1);
        unordered_map<Node*, double> neighbors2 = graph2.FindNeighbors(artist1);
        unordered_map<Node*, double>::iterator it;
        Node* n = graph.GetNode(artist1);
        for (int i = 0; i < n->SongCount(); i++)    {
            cout << n->RequestSong()._name << endl;
        }
        cout << endl << endl;
        n = graph2.GetNode(artist1);
        for (int i = 0; i < n->SongCount(); i++)    {
            cout << n->RequestSong()._name << endl;
        }
        cout << endl << endl;
        for (it = neighbors.begin(); it != neighbors.end(); it++) {
            cout << it->first->GetArtist() << " " << it->second << endl;
        }
        cout << endl << endl;
        for (it = neighbors.begin(); it != neighbors.end(); it++) {
            cout << it->first->GetArtist() << " " << it->second << endl;
        }
        cout << endl << endl;
    }
    return 0;
}




