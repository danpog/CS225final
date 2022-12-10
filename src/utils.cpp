#include "utils.h"
#include <fstream>
using namespace std;

/*
vector<Playlist> Graph::ParseJSON(string filename) {
    if (filename.empty()) {
        throw std::invalid_argument("No file name");
    }
    
    vector<Playlist> all_playlists = vector<Playlist>();
    Json::Value all_playlists_json; 

    std::ifstream playlist_file(filename);
    playlist_file >> all_playlists_json;

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
*/

vector<Playlist> ParseCSV(string filename)  {
    if (filename.empty()) {
        throw std::invalid_argument("No file name");
    }

    vector<Playlist> all_playlists = vector<Playlist>();
    std::ifstream playlist_file(filename);
    string line;
    std::getline(playlist_file,line);
    while (playlist_file.good())    {
        if (line.substr(0,9) == "Playlist:") {
            vector<string> parts;
            int count = SplitString(line, ',', parts);
            if (count > 3) {
                string s = parts[1];
                for (int i = 2; i < count - 1; i++) {
                    s += parts[i];
                }
                parts[1] = s;
                parts[2] = parts[count - 1];
            }
            Playlist p(parts[1]);
            p.SetID(std::stoi(parts[2]));
            if (std::stoi(parts[2]) % 1000 == 0)    {
                cout << "Parsing playlist " << parts[2]<< endl;
            }
            if (!playlist_file.good())  {
                break;
            }
            std::getline(playlist_file,line);
            while (line.substr(0,9) != "Playlist:") {
                vector<string> song;
                int length = SplitString(line, ',', song);
                if (length  == 0)   {
                    break;
                }
                if (length != 4)    {
                    int temp = 0;
                    for (int i = 0; i < 4; i++){
                        string curr;
                        curr += song[temp];
                        while (song[temp].at(song[temp].length() - 1) != '\"')  {
                            temp++;
                            curr += "," + song[temp];
                        }
                        temp++;
                        song[i] = curr;
                    }
                }
                Song s = Song(song[0], song[1], song[2], song[3]);
                p.AddSong(s);
                if (!playlist_file.good())  {
                    break;
                }
                std::getline(playlist_file,line);
                if (line.size() == 0)   {
                    break;
                }
            }
            all_playlists.push_back(p);
        }
    }
    return all_playlists;
}
int SplitString(const std::string & str1, char sep, std::vector<std::string> &fields) {
    std::string str = str1;
    std::string::size_type pos;
    while((pos=str.find(sep)) != std::string::npos) {
        fields.push_back(str.substr(0,pos));
        str.erase(0,pos+1);  
    }
    fields.push_back(str);
    return fields.size();
}