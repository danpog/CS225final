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
    //print first playlist
    std::cout << "Ran" << std::endl;
    vector<Playlist> a = parse("Songs0-600.json");
    vector<Song> first = a[0].GetSongs();
    for (Song i : first) {
        std::cout << i << std::endl;
    }
    return 0;
}




