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
    std::cout << s._name << endl << s._album << endl << s._artist << endl << endl;
}

void JSONtoCSV(string json, string csv)  {
    if (json.empty()) {
        throw std::invalid_argument("No file name");
    }
    
    vector<Playlist> all_playlists = vector<Playlist>();
    Json::Value all_playlists_json; 

    std::ifstream playlist_file(json);
    std::ofstream csv_file(csv);
    playlist_file >> all_playlists_json;

    // cout << all_playlists_json; // This will print the entire json object.
    //std::cout << all_playlists_json["info"]; 
    Json::StreamWriterBuilder builder;
    // builder["indentation"] = ""; // If you want whitespace-less output

    for(Json::Value playlist: all_playlists_json["playlists"]) {
        const std::string name = Json::writeString(builder, playlist["name"]);
        Json::Value pid = playlist["pid"];
        int id = pid.asInt();
        Playlist p = Playlist(name);
        p.SetID(id);
        csv_file << "Playlist:" << "," << name << "," << id << endl;
        for (Json::Value track: playlist["tracks"]) {
            std::string name = Json::writeString(builder, track["track_name"]);
            std::string artist = Json::writeString(builder, track["artist_name"]);
            std::string album_name = Json::writeString(builder, track["album_name"]);
            std::string uri = Json::writeString(builder, track["track_uri"]);
            Song s = Song(name, album_name, artist, uri);
            p.AddSong(s);
            csv_file << s._name << "," << s._album << "," << s._artist << "," << s._uri << endl;
        }
        all_playlists.push_back(p);
    }
    csv_file.close();
}
int main(int argc, char *argv[])  {
    bool make_playlist = false;
    if (argc != 6)  {
        cout << "If you want to create a playlist, use these arguments in this order: " << endl;
        cout << "(string) user_id" << endl;
        cout << "(string) OAuth Token" << endl;
        cout << "(string) Playlist Name" << endl;
        cout << "(string) Playlist Description" << endl;
        cout << "(int) Number of Songs" << endl;
    }
    string user_id = "";
    string token = "";
    string playlist_name = "";
    string playlist_desc = "";
    int num_songs = 0;
    if (argc == 6)  {
        make_playlist = true;
        user_id = argv[1];
        token = argv[2];
        playlist_name = argv[3];
        playlist_desc = argv[4];
        num_songs = stoi(argv[5]);
    }
    string playlist_link = "";
    Graph graph = Graph();
    if (make_playlist)  {
        playlist_link = graph.CreateSpotifyPlaylist(user_id,token, playlist_name, playlist_desc, false);
        if (playlist_link == "error")  {
            cout << "Could Not Create Playlist!" << endl;
            return 0;
        }
    }
    
    

    chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    
    vector<Playlist> a;

    // Creating the graph
    for (int i = 0; i < 15000; i += 1000) {
        
        a = graph.ParseJSON("../spotify_million_playlist_dataset/data/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        //a = parse("mpdslices/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        
        graph.analyze_all_playlists(a);
    }
    
    chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t = ((t_final-t_start)/1000.0).count();
    std::cout << "Trimming Neighbors:" << endl;
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(5, true);
    }

    chrono::milliseconds t_final_2 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t2 = ((t_final_2-t_final)/1000.0).count();

//EDAN'S TESTING
    /*cout << "Saving Graph:" << endl;
    chrono::milliseconds t_start2 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    graph.save_graph("saved_graph.json");
    chrono::milliseconds t_final3 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t3 = ((t_final3-t_start2)/1000.0).count();
    cout << "Generating New Graph:" << endl;
    chrono::milliseconds t_start3 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    Graph graph2 = Graph("500k_playlists_graph.json");
    chrono::milliseconds t_final4 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t4 = ((t_final4-t_start3)/1000.0).count();
    //cout << endl << "Graph generated in "<< t << "s" << endl;
    //cout << "Neighbor trimmed in "<< t2 << "s" << endl;
    //cout << "Graph saved in "<< t3 << "s" << endl;
    cout << "New graph generated in "<< t4 << "s" << endl;*/

//ELI'S TESTING
    vector<string> artists;
    //artists.push_back("\"Count Basie\"");
    artists.push_back("\"Crosby, Stills, Nash & Young\"");
    artists.push_back("\"Pink Floyd\"");
    artists.push_back("\"Dire Straits\"");
    artists.push_back("\"Peter Gabriel\"");
    artists.push_back("\"Jimi Hendrix\"");

    vector<Song> song_prefs;
    Node* n = graph.GetNode("\"Crosby, Stills, Nash & Young\"");
    song_prefs.push_back(n -> FindSong("\"Almost Cut My Hair\""));
    
    n = graph.GetNode("\"Pink Floyd\"");
    auto s = n -> GetAllSongs();
    song_prefs.push_back(s[s.size()/2].first);

    n = graph.GetNode("\"Dire Straits\"");
    s = n -> GetAllSongs();
    song_prefs.push_back(s[s.size()/2].first);
    
    n = graph.GetNode("\"Peter Gabriel\"");
    s = n -> GetAllSongs();
    song_prefs.push_back(s[s.size()/2].first);

    n = graph.GetNode("\"Jimi Hendrix\"");
    s = n -> GetAllSongs();
    song_prefs.push_back(s[s.size()/2].first);
    //Almost Cut My Hair Song by Crosby, Stills, Nash & Young
    //pink flloyd
    //dire straits
    //peter gabriel
    //jimmy hendricks
    if (make_playlist)  {
        Playlist p = graph.CreatePlaylist(num_songs, song_prefs);
        graph.SendPlaylistToSpotify(p, token, playlist_link);
    }
    else    {
        Playlist p = graph.CreatePlaylist(100, song_prefs);
        cout << p << endl;
    }
    
    /*for (string artist1 : artists) {
        std::cout << artist1 << std::endl;
        unordered_map<Node*, int> neighbors = graph.FindNeighbors(artist1);
        unordered_map<Node*, int>::iterator it;
        Node* n = graph.GetNode(artist1);
        std::cout << n -> SongCount() << endl;
        //for (int i = 0; i < n->SongCount(); i++)    {
          //  cout << "song: " << endl;
           // cout << n->RequestSong()._name << endl;
        //}
        for (it = neighbors.begin(); it != neighbors.end(); it++) {
            //if (it->second > 20) {
                std::cout << it->first->GetArtist() << " " << it->second << endl;
            //}
        } //
    }*/
    cout << "graph generated in " << t << " seconds" << endl;
    cout << "neighbors trimmed in " << t2 << "seconds" << endl;
    if (make_playlist)  {
        std::cout <<"Your Playlist: " << "https://open.spotify.com/playlist/" + playlist_link << std::endl;
    }
    
    return 0;
}




