#include "song.h"
#include "playlist.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

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
        Playlist p = Playlist(name);
        for (Json::Value track: playlist["tracks"]) {
            std::string name = Json::writeString(builder, track["track_name"]);
            std::string artist = Json::writeString(builder, track["artist_name"]);
            std::string album_name = Json::writeString(builder, track["album_name"]);
            Song s = Song(name, album_name, artist);
            p.AddSong(s);
        }
        std::cout << p << std::endl;
        all_playlists.push_back(p);
    }

    return all_playlists;
}

int main()  {
    std::cout << "Ran" << std::endl;
    vector<Playlist> a = parse("SongsLimit.json");
    return 0;
}




