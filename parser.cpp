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

void split_file(string file) {
    Json::Value all_nodes_json;
    std::ifstream graph_file(file);
    graph_file >> all_nodes_json;
    Json::StreamWriterBuilder builder;


    ofstream saved_graph;


    string artist;
    int frequency;
    string artist2;
    string name;
    string album;
    string artist3;
    string uri;
    int frequency2;
    int neighbor_counter;
    int song_counter;
    int node_counter = 0;
    saved_graph.open("test_nodes/500k_nodes_" + to_string(node_counter) + "-" + to_string(node_counter + 29999) + ".json");
    saved_graph << "{";
    saved_graph << "\"nodes\": [";
    for (Json::Value node: all_nodes_json["nodes"]) {
        artist = Json::writeString(builder, node["artist"]);
        saved_graph << "{";
        saved_graph << "\"artist\": " << artist << ",";
        saved_graph << "\"neighbors\": [";
        neighbor_counter = 1;
        for (Json::Value neighbor: node["neighbors"]) {
            artist2 = Json::writeString(builder, neighbor["artist"]);
            frequency = neighbor["frequency"].asInt();
            saved_graph << "{";
            saved_graph << "\"artist\": " << artist2 << ", ";
            saved_graph << "\"frequency\": " << frequency << "}";
            if (neighbor_counter < int(node["neighbors"].size())) {
                saved_graph << ",";
            }
            neighbor_counter++;
        }
        saved_graph << "],";
        saved_graph << "\"songs\": [";
        song_counter = 1;
        for (Json::Value song: node["songs"]) {
            name = Json::writeString(builder, song["name"]);
            album = Json::writeString(builder, song["album"]);
            artist3 = Json::writeString(builder, song["artist"]);
            uri = Json::writeString(builder, song["uri"]);
            frequency2 = song["frequency"].asInt();
            saved_graph << "{";
            saved_graph << "\"name\": " << name << ", ";
            saved_graph << "\"album\": " << album << ", ";
            saved_graph << "\"artist\": " << artist3 << ", ";
            saved_graph << "\"uri\": " << uri << ", ";
            saved_graph << "\"frequency\": " << frequency2 << "}";
            if (song_counter < int(node["songs"].size())) {
                saved_graph << ",";
            }
            song_counter++;
        }
        saved_graph << "]";
        saved_graph << "},";
        node_counter++;
        if (node_counter % 30000 == 0) {
            saved_graph << "{}";
            saved_graph << "]";
            saved_graph << "}";
            saved_graph.close();
            saved_graph.open("test_nodes/500k_nodes_" + to_string(node_counter) + "-" + to_string(node_counter + 29999) + ".json");
            saved_graph << "{";
            saved_graph << "\"nodes\": [";
        }
    }
    saved_graph << "{}";
    saved_graph << "]";
    saved_graph << "}";
    saved_graph.close();
}

void combine_files(vector<string> files) {
    Json::Value all_nodes_json;
    Json::StreamWriterBuilder builder;
    ofstream saved_graph;
    string artist;
    string artist2;
    int frequency;
    string name;
    string album;
    string artist3;
    string uri;
    int frequency2;
    int neighbor_counter;
    int song_counter;
    saved_graph.open("500k_graph.json");
    saved_graph << "{";
    saved_graph << "\"nodes\": [";
    for (string file: files) {
        std::ifstream graph_file(file);
        graph_file >> all_nodes_json;

        for (Json::Value node: all_nodes_json["nodes"]) {
            artist = Json::writeString(builder, node["artist"]);
            saved_graph << "{";
            saved_graph << "\"artist\": " << artist << ",";
            saved_graph << "\"neighbors\": [";
            neighbor_counter = 1;
            for (Json::Value neighbor: node["neighbors"]) {
                artist2 = Json::writeString(builder, neighbor["artist"]);
                frequency = neighbor["frequency"].asInt();
                saved_graph << "{";
                saved_graph << "\"artist\": " << artist2 << ", ";
                saved_graph << "\"frequency\": " << frequency << "}";
                if (neighbor_counter < int(node["neighbors"].size())) {
                    saved_graph << ",";
                }
                neighbor_counter++;
            }
            saved_graph << "],";
            saved_graph << "\"songs\": [";
            song_counter = 1;
            for (Json::Value song: node["songs"]) {
                name = Json::writeString(builder, song["name"]);
                album = Json::writeString(builder, song["album"]);
                artist3 = Json::writeString(builder, song["artist"]);
                uri = Json::writeString(builder, song["uri"]);
                frequency2 = song["frequency"].asInt();
                saved_graph << "{";
                saved_graph << "\"name\": " << name << ", ";
                saved_graph << "\"album\": " << album << ", ";
                saved_graph << "\"artist\": " << artist3 << ", ";
                saved_graph << "\"uri\": " << uri << ", ";
                saved_graph << "\"frequency\": " << frequency2 << "}";
                if (song_counter < int(node["songs"].size())) {
                    saved_graph << ",";
                }
                song_counter++;
            }
            saved_graph << "]";
            saved_graph << "},";
        }
    }
    saved_graph << "{}";
    saved_graph << "]";
    saved_graph << "}";
    saved_graph.close();

}

void PrintSong(Song& s) {
    std::cout << s._name << endl << s._album << endl << s._artist << endl << endl;
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
    std::cout << all_playlists_json["info"]; 
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
bool NeighborsContain(Node* node, Node* target, double count)   {
    return node->GetNeighbors()[target] == count;
}

int main()  {
    /*chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    Graph graph = Graph();
    vector<Playlist> a;

    // Creating the graph
    a = parse("./old_testing/SongsLimit.json");
    graph.analyze_all_playlists(a);*/
    /*for (int i = 0; i < 12000; i += 1000) {
        
        a = parse("../spotify_million_playlist_dataset/data/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        //a = parse("mpdslices/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        graph.analyze_all_playlists(a);
    }*/
    
    /*chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t = ((t_final-t_start)/1000.0).count();
    std::cout << "Trimming Neighbors:" << endl;
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(3, true);
    }

    chrono::milliseconds t_final_2 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t2 = ((t_final_2-t_final)/1000.0).count();*/

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
    //vector<string> artists;
    //artists.push_back("\"Count Basie\"");
    /*artists.push_back("\"Crosby, Stills, Nash & Young\"");
    artists.push_back("\"Pink Floyd\"");
    artists.push_back("\"Dire Straits\"");
    artists.push_back("\"Peter Gabriel\"");
    artists.push_back("\"Jimi Hendrix\"");*/

    /*vector<Song> song_prefs;
    song_prefs.push_back(graph.GetNode("\"Crosby, Stills, Nash & Young\"") -> FindSong("\"Almost Cut My Hair\""));
    song_prefs.push_back(graph.GetNode("\"Pink Floyd\"") -> FindSong("\"Wish You Were Here\""));
    song_prefs.push_back(graph.GetNode("\"Dire Straits\"") -> FindSong("\"Walk Of Life\""));
    song_prefs.push_back(graph.GetNode("\"Peter Gabriel\"") -> FindSong("\"Solsbury Hill - 2002 Remaster\""));
    song_prefs.push_back(graph.GetNode("\"Jimi Hendrix\"") -> FindSong("\"Voodoo Child (Slight Return)\""));*/
    //Almost Cut My Hair Song by Crosby, Stills, Nash & Young
    //pink flloyd
    //dire straits
    //peter gabriel
    //jimmy hendricks

    
    //graph.SendPlaylistToSpotify(p, "BQB_n6hwB9YZKll7kZFAUjgT2VOS_wdXgOY2NeWhX2lGEJZgQWzRpBqnaXHAeuC5FeRgbA4pJEGEApK53nDU4GwciMqbZVJBl1U3T7irW6Jdu49WOqx7SzwjDAATPGNybPWpfSPNY3EG2ZtscbMa1k-Nb1zqJAZHwHlRQsTx_GsoNXm7B2tNDKAinIL1euSUoo3oM5_jHrEyHWXfUDEIFmTmD-MGjoBlZxo"
    //, "7J2dhcHVao5mJronBUmHIy");
    /*for (string artist1 : artists) {
        std::cout << artist1 << std::endl;
        unordered_map<Node*, int> neighbors = graph.FindNeighbors(artist1);
        unordered_map<Node*, int>::iterator it;
        Node* n = graph.GetNode(artist1);
        std::cout << n -> SongCount() << endl;*/
        /*for (int i = 0; i < n->SongCount(); i++)    {
            cout << "song: " << endl;
            cout << n->RequestSong()._name << endl;
        }*/
        //for (it = neighbors.begin(); it != neighbors.end(); it++) {
            //if (it->second > 20) {
                //std::cout << it->first->GetArtist() << " " << it->second << endl;
            //}
        //} //
    //}
    /*for (Song& b : a[0].GetSongs()) {
        cout << std::hash<Song&>{}(b) << endl;
    }*/
    /*vector<Song> song_prefs;
    std::cout << endl << "Graph generated in "<< t << "s" << endl;
    std::cout << endl << "Neighbor trimmed in "<< t2 << "s" << endl;
    Node* source = graph.GetNode("\"Missy Elliott\"");
    Node* top = graph.GetNode("\"Jesse McCartney\"");
    Node* second = graph.GetNode("\"Chris Brown\"");
    Node* third = graph.GetNode("\"Justin Bieber\"");
    Node* bieber3 = graph.GetNode("\"Destiny's Child\"");
    cout << NeighborsContain(source, top, 4) << endl;
    cout << NeighborsContain(source, second, 3) << endl;
    cout << NeighborsContain(source, third, 3) << endl;
    cout << NeighborsContain(third, top, 12) << endl;
    cout << NeighborsContain(third, second, 9) << endl;
    cout << NeighborsContain(third, bieber3, 6) << endl;
    song_prefs.push_back(graph.GetNode("\"Missy Elliott\"") -> FindSong("\"Lose Control (feat. Ciara & Fat Man Scoop)\""));
    Playlist p = graph.CreatePlaylist(9, song_prefs);
    //1, 3-5
    cout << p.ContainsSongByName("\"Lose Control (feat. Ciara & Fat Man Scoop)\"") << endl;
    cout << p.ContainsSongByName("\"One Less Lonely Girl\"") << endl;
    cout << p.ContainsSongByName("\"Right Where You Want Me - Radio Edit Version\"") << endl;
    cout << p.ContainsSongByName("\"Run It!\"") << endl;
    //7-9
    cout << p.ContainsSongByName("\"Yo (Excuse Me Miss)\"") << endl;
    cout << p.ContainsSongByName("\"Say My Name\"") << endl;
    cout << p.ContainsSongByName("\"Somebody To Love\"") << endl;
    cout << p;*/

    
    vector<string> files;
    for (int i = 0; i < 8; i++) {
        files.push_back("graph_nodes/500k_nodes_" + to_string(i * 30000) + "-" + to_string((i+1) * 30000 - 1) + ".json");
    }
    combine_files(files);

    return 0;
}




