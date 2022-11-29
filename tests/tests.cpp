#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif

#include <catch/catch.hpp>
#include "../node.cpp"
#include "../playlist.cpp"
#include "../song.cpp"
#include "../graph.cpp"
#include <json/json.h>


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

TEST_CASE("Check Song Equality") { 
    Song s = Song("a", "b", "c", "d");
    Song t = Song("a", "b", "c", "e");
    Song u = Song("a", "b", "c", "e");
    
    REQUIRE(t == u); 
    REQUIRE_FALSE(s == t);
}

TEST_CASE("Check Neighbors") { 
    vector<Playlist> a;
    
    a = parse("./old_testing/SongsLimit.json");
    Graph graph = Graph(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(3, true);
    }

    Node* source = graph.GetNode("\"Missy Elliott\"");
    Node* top = graph.GetNode("\"Jesse McCartney\"");
    Node* second = graph.GetNode("\"Chris Brown\"");
    Node* third = graph.GetNode("\"Justin Bieber\"");
    Node* bieber3 = graph.GetNode("\"Destiny's Child\"");
    
    REQUIRE(NeighborsContain(source, top, 4)); 
    REQUIRE(NeighborsContain(source, second, 3));
    REQUIRE(NeighborsContain(source, third, 3));
    REQUIRE(NeighborsContain(third, top, 12));
    REQUIRE(NeighborsContain(third, second, 9));
    REQUIRE(NeighborsContain(third, bieber3, 6));
}

TEST_CASE("Check Playlist") { 
    vector<Playlist> a;
    
    a = parse("./old_testing/SongsLimit.json");
    Graph graph = Graph(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(3, true);
    }
    vector<Song> song_prefs;
    song_prefs.push_back(graph.GetNode("\"Missy Elliott\"") -> FindSong("\"Lose Control (feat. Ciara & Fat Man Scoop)\""));
    Playlist p = graph.CreatePlaylist(9, song_prefs);

    /*Node* source = graph.GetNode("\"Missy Elliott\"");
    Node* top = graph.GetNode("\"Jesse McCartney\"");
    Node* second = graph.GetNode("\"Chris Brown\"");
    Node* third = graph.GetNode("\"Justin Bieber\"");
    Node* bieber3 = graph.GetNode("\"Destiny's Child\"");*/
    //7-9
    REQUIRE(p.GetSongs().size() == 9);
    for (auto i : p.GetSongs()) {
        std::cout << i._name << std::endl;
    }
    REQUIRE(p.ContainsSongByName("\"Lose Control (feat. Ciara & Fat Man Scoop)\"")); 
    REQUIRE(p.ContainsSongByName("\"One Less Lonely Girl\""));
    REQUIRE(p.ContainsSongByName("\"Run It!\""));

    REQUIRE(p.ContainsSongByName("\"Yo (Excuse Me Miss)\""));
    REQUIRE(p.ContainsSongByName("\"Say My Name\""));
    REQUIRE(p.ContainsSongByName("\"Somebody To Love\""));
}