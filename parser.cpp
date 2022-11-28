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
    string s;
    string command;
    fstream file("../copy");
    string s2;
    while (getline(file, s2))    {
        command += s2;
    }
    char char_array[command.length() + 1];
    strcpy(char_array, command.c_str());
    char* temp = char_array;
    system(temp);
    //
    /*do 
    {
    getline(cin, s);
    
    cout << s << endl;
    } while (getchar() == 'f');*/

    cout << "done" << endl;
    return 0;
    /*string s;
    string s2;
    fstream file("../command_test");
    while (getline(file, s2))    {
        s += s2;
    }
    char char_array[s.length() + 1];
    strcpy(char_array, s.c_str());
    char* temp = char_array;
    system(temp);
    return 0;*/

    chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    Graph graph = Graph();
    vector<Playlist> a;

    // Creating the graph
    for (int i = 0; i < 12000; i += 1000) {
        
        a = parse("../spotify_million_playlist_dataset/data/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        //a = parse("mpdslices/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        graph.analyze_all_playlists(a);
    }
    
    chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t = ((t_final-t_start)/1000.0).count();

    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(5, true);
    }

    chrono::milliseconds t_final_2 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t2 = ((t_final_2-t_final)/1000.0).count();

    vector<string> artists;
    //artists.push_back("\"Count Basie\"");
    artists.push_back("\"Crosby, Stills, Nash & Young\"");
    artists.push_back("\"Pink Floyd\"");
    artists.push_back("\"Dire Straits\"");
    artists.push_back("\"Peter Gabriel\"");
    artists.push_back("\"Jimi Hendrix\"");

    vector<Song> song_prefs;
    song_prefs.push_back(graph.GetNode("\"Crosby, Stills, Nash & Young\"") -> FindSong("\"Almost Cut My Hair\""));
    song_prefs.push_back(graph.GetNode("\"Pink Floyd\"") -> FindSong("\"Wish You Were Here\""));
    song_prefs.push_back(graph.GetNode("\"Dire Straits\"") -> FindSong("\"Walk Of Life\""));
    song_prefs.push_back(graph.GetNode("\"Peter Gabriel\"") -> FindSong("\"Solsbury Hill - 2002 Remaster\""));
    song_prefs.push_back(graph.GetNode("\"Jimi Hendrix\"") -> FindSong("\"Voodoo Child (Slight Return)\""));
    //Almost Cut My Hair Song by Crosby, Stills, Nash & Young
    //pink flloyd
    //dire straits
    //peter gabriel
    //jimmy hendricks

    Playlist p = graph.CreatePlaylist(100, song_prefs);
    graph.SendPlaylistToSpotify(p, "BQB_n6hwB9YZKll7kZFAUjgT2VOS_wdXgOY2NeWhX2lGEJZgQWzRpBqnaXHAeuC5FeRgbA4pJEGEApK53nDU4GwciMqbZVJBl1U3T7irW6Jdu49WOqx7SzwjDAATPGNybPWpfSPNY3EG2ZtscbMa1k-Nb1zqJAZHwHlRQsTx_GsoNXm7B2tNDKAinIL1euSUoo3oM5_jHrEyHWXfUDEIFmTmD-MGjoBlZxo"
    , "7J2dhcHVao5mJronBUmHIy");
    cout << p << endl;
    for (string artist1 : artists) {
        std::cout << artist1 << std::endl;
        unordered_map<Node*, double> neighbors = graph.FindNeighbors(artist1);
        unordered_map<Node*, double>::iterator it;
        Node* n = graph.GetNode(artist1);
        cout << n -> SongCount() << endl;
        /*for (int i = 0; i < n->SongCount(); i++)    {
            cout << "song: " << endl;
            cout << n->RequestSong()._name << endl;
        }*/
        for (it = neighbors.begin(); it != neighbors.end(); it++) {
            //if (it->second > 20) {
                cout << it->first->GetArtist() << " " << it->second << endl;
            //}
        } //
    }
    /*for (Song& b : a[0].GetSongs()) {
        cout << std::hash<Song&>{}(b) << endl;
    }*/
    cout << endl << "Graph generated in "<< t << "s" << endl;
    cout << endl << "Neighbor trimmed in "<< t2 << "s" << endl;
    return 0;
}




