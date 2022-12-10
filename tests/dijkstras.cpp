#include "../lib/song.h"
#include "../lib/playlist.h"
#include "../lib/node.h"
#include "../src/graph.h"

#include <fstream>
#include <chrono>

#include <json/json.h>

using namespace std;

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
int main2(int argc, char *argv[])  {
    bool make_playlist = false;
    /*
    if (argc != 6)  {
        cout << "If you want to create a playlist, use these arguments in this order: " << endl;
        cout << "(string) user_id" << endl;
        cout << "(string) OAuth Token" << endl;
        cout << "(string) Playlist Name" << endl;
        cout << "(string) Playlist Description" << endl;
        cout << "(int) Number of Songs" << endl;
    }
    */
    string user_id = "";
    string token = "";
    string playlist_name = "";
    string playlist_desc = "";
    //int num_songs = 0;
    if (argc == 6)  {
        make_playlist = true;
        user_id = argv[1];
        token = argv[2];
        playlist_name = argv[3];
        playlist_desc = argv[4];
        // num_songs = stoi(argv[5]);
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
    //for (int i = 0; i < 4000; i += 1000) {
        //a = parse("mpdslices/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        //a = parse("mpdslices/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        a = parse("./tests/old_testing/Songs0-600.json");
        graph.analyze_all_playlists(a);
    //}
    
    chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t = ((t_final-t_start)/1000.0).count();
    cout << "made graph in: " << t << "s" << endl;
    std::cout << "Trimming Neighbors:" << endl;
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(3, true);
    }

    chrono::milliseconds t_final_2 = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    double t2 = ((t_final_2-t_final)/1000.0).count();
    cout << "trimmed neighbors in: " << t2 << "s" << endl;

    string artist = "\"Missy Elliott\"";
    Node n1 = graph.getGraph()[artist];

    for (auto x : n1.GetNeighbors()) {
        cout << x.first->GetArtist() << " " << x.second << endl;
    }
    cout << endl;

    string artist1 = "\"Rihanna\"";
    Node n2 = graph.getGraph()[artist1];

    for (auto x : n2.GetNeighbors()) {
        cout << x.first->GetArtist() << " " << x.second << endl;
    }
    cout << endl;

    string artist2 = "\"Drake\"";
    Node n3 = graph.getGraph()[artist2];
    for (auto x : n3.GetNeighbors()) {
        cout << x.first->GetArtist() << " " << x.second << endl;
    }
    cout << endl;
    string artist3 = "\"Kanye West\"";

    string artist4 = "\"Maroon 5\"";
    
    auto t4 = graph.Dijkstras(artist, artist3);

    cout << "traversing result --------------" << endl;
    std::cout << t4.size() << std::endl;
    for (size_t i = 0; i < t4.size(); ++i) {
        std::cout << t4[i] << std::endl;
    }
    return 0;
}



