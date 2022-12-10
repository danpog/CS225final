#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif


/////////////////////////////////////////////////////////////////////////////////////////////
//                                 Includes //
/////////////////////////////////////////////////////////////////////////////////////////////

#include <catch2/catch_test_macros.hpp>
#include "../src/graph.cpp"
//#include <json/json.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helpers/Constants //
/////////////////////////////////////////////////////////////////////////////////////////////


bool NeighborsContain(Node* node, Node* target, double count)   {
    return node->GetNeighbors()[target] == count;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                                Test Cases //
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Check Song Equality") { 
    Song s = Song("a", "b", "c", "d");
    Song t = Song("a", "b", "c", "e");
    Song u = Song("a", "b", "c", "e");
    
    REQUIRE(t == u); 
    REQUIRE_FALSE(s == t);
}

TEST_CASE("Check Neighbors") { 
    vector<Playlist> a;
    
    //a = parse("./tests/old_testing/SongsLimit.json");
    Graph graph;
    a = graph.ParseCSV("../tests/test_playlist.csv");
    graph.analyze_all_playlists(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(3, true);
    }
    
    //cout << a[0].SongCount() << endl;

    Node* source = graph.GetNode("\"Missy Elliott\"");
    Node* top = graph.GetNode("\"Britney Spears\"");
    Node* second = graph.GetNode("\"OutKast\"");
    Node* third = graph.GetNode("\"Usher\"");
    
    REQUIRE(NeighborsContain(source, top, 2)); 
    REQUIRE(NeighborsContain(source, second, 2));
    REQUIRE(NeighborsContain(source, third, 2));
    REQUIRE(NeighborsContain(second, third, 4));
    REQUIRE(NeighborsContain(third, second, 4));
}

TEST_CASE("Check Playlist") { 
    vector<Playlist> a;
    
    Graph graph;
    a = graph.ParseCSV("../tests/test_playlist.csv");
    graph.analyze_all_playlists(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(3, true);
    }
    
    vector<Song> song_prefs;
    song_prefs.push_back(graph.GetNode("\"Missy Elliott\"") -> FindSong("\"Lose Control (feat. Ciara & Fat Man Scoop)\""));
    Playlist p = graph.CreatePlaylist(9, song_prefs);

    REQUIRE(p.GetSongs().size() == 9);

    REQUIRE(p.ContainsSongByName("\"Lose Control (feat. Ciara & Fat Man Scoop)\"")); 

    REQUIRE(p.ContainsSongByName("\"Hey Ya! - Radio Mix / Club Mix\""));
    REQUIRE(p.ContainsSongByName("\"Yeah!\""));
    REQUIRE(p.ContainsSongByName("\"Toxic\""));

    REQUIRE(p.ContainsSongByName("\"Lucky\""));

}

TEST_CASE("Check playlist larger") {
    vector<Playlist> a;
    
    Graph graph;
    a = graph.ParseCSV("../tests/test_playlist_large.csv");
    graph.analyze_all_playlists(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(3, true);
    }
    
    vector<Song> song_prefs;
    song_prefs.push_back(graph.GetNode("\"Missy Elliott\"") -> FindSong("\"Lose Control (feat. Ciara & Fat Man Scoop)\""));
    Playlist p = graph.CreatePlaylist(9, song_prefs);

    REQUIRE(p.GetSongs().size() == 9);

    REQUIRE(p.ContainsSongByName("\"One Less Lonely Girl\"")); 
    REQUIRE(p.ContainsSongByName("\"Run It!\""));
    REQUIRE(p.ContainsSongByName("\"Forever\""));
}   

TEST_CASE("Check playlist constructed") {
    Song q = Song("a1", "b", "1", "A");
    Song r = Song("a2", "b", "2", "B");
    Song s = Song("a3", "b", "3", "C");
    Song t = Song("a4", "b", "4", "D");
    Song u = Song("a5", "b", "5", "E");
    Song v = Song("a6", "b", "6", "F");
    std::vector<Song> songs = {q};
    Graph g;  
    unordered_map<string, Node>& _graph = g.getGraph();
    _graph.insert(pair<string, Node>("1", Node("1")));
    _graph.insert(pair<string, Node>("2", Node("2")));
    _graph.insert(pair<string, Node>("3", Node("3")));
    _graph.insert(pair<string, Node>("4", Node("4")));
    _graph.insert(pair<string, Node>("5", Node("5")));
    _graph.insert(pair<string, Node>("6", Node("6")));

    _graph["1"].AddNeighborPair(std::pair<Node*, int>(&_graph["2"], 1));
    _graph["2"].AddNeighborPair(std::pair<Node*, int>(&_graph["3"], 1));
    _graph["3"].AddNeighborPair(std::pair<Node*, int>(&_graph["4"], 1));
    _graph["4"].AddNeighborPair(std::pair<Node*, int>(&_graph["5"], 1));
    _graph["4"].AddNeighborPair(std::pair<Node*, int>(&_graph["6"], 1));

    g.GetNode("1")->AddSongPair(q, 1);
    g.GetNode("2")->AddSongPair(r, 1);
    g.GetNode("3")->AddSongPair(s, 1);
    g.GetNode("4")->AddSongPair(t, 1);
    g.GetNode("5")->AddSongPair(u, 1);
    g.GetNode("6")->AddSongPair(v, 1);

    Playlist p = g.CreatePlaylist(1, songs);
    REQUIRE(p.GetSongs().size() == 1);
    REQUIRE(p.ContainsSongByName("a1")); 

    Playlist p2 = g.CreatePlaylist(3, songs);
    REQUIRE(p2.GetSongs().size() == 3);
    REQUIRE(p2.ContainsSongByName("a1"));
    REQUIRE(p2.ContainsSongByName("a2"));

    Playlist p3 = g.CreatePlaylist(6, songs);
    REQUIRE(p3.GetSongs().size() == 6);
    REQUIRE(p3.ContainsSongByName("a1"));
    REQUIRE(p3.ContainsSongByName("a2"));
    REQUIRE(p3.ContainsSongByName("a3"));

    Playlist p4 = g.CreatePlaylist(10, songs);
    REQUIRE(p4.GetSongs().size() == 10);
    REQUIRE(p4.ContainsSongByName("a1"));
    REQUIRE(p4.ContainsSongByName("a2"));
    REQUIRE(p4.ContainsSongByName("a3"));
    REQUIRE(p4.ContainsSongByName("a4"));

    Playlist p5 = g.CreatePlaylist(16, songs);
    REQUIRE(p5.GetSongs().size() == 16);
    REQUIRE(p5.ContainsSongByName("a1"));
    REQUIRE(p5.ContainsSongByName("a2"));
    REQUIRE(p5.ContainsSongByName("a3"));
    REQUIRE(p5.ContainsSongByName("a4"));
    REQUIRE(p5.ContainsSongByName("a5"));
    REQUIRE(p5.ContainsSongByName("a6"));   
}
