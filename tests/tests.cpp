#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif


/////////////////////////////////////////////////////////////////////////////////////////////
//                                 Includes                                                //
/////////////////////////////////////////////////////////////////////////////////////////////

#include <catch2/catch_test_macros.hpp>
#include "../src/graph.cpp"
#include "../src/utils.h"
//#include <json/json.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helpers/Constants                                           //
/////////////////////////////////////////////////////////////////////////////////////////////


bool NeighborsContain(Node* node, Node* target, double count)   {
    return node->GetNeighbors()[target] == count;
}

Graph CustomGraph(std::vector<Song> input_songs)    {
    Song q = input_songs[0];
    Song r = input_songs[1];
    Song s = input_songs[2];
    Song t = input_songs[3];
    Song u = input_songs[4];
    Song v = input_songs[5];
    std::vector<Song> songs = {q};
    Graph g;  
    unordered_map<string, Node>& _graph = g.getGraph();
    _graph.insert(pair<string, Node>("1", Node("1")));
    _graph.insert(pair<string, Node>("2", Node("2")));
    _graph.insert(pair<string, Node>("3", Node("3")));
    _graph.insert(pair<string, Node>("4", Node("4")));
    _graph.insert(pair<string, Node>("5", Node("5")));
    _graph.insert(pair<string, Node>("6", Node("6")));


    _graph["1"].AddNeighbor(&_graph["2"]);
    _graph["2"].AddNeighbor(&_graph["3"]);
    _graph["3"].AddNeighbor(&_graph["4"]);
    _graph["4"].AddNeighbor(&_graph["5"]);
    _graph["4"].AddNeighbor(&_graph["5"]);
    _graph["4"].AddNeighbor(&_graph["6"]);

    g.GetNode("1")->AddSongPair(q, 1);
    g.GetNode("2")->AddSongPair(r, 1);
    g.GetNode("3")->AddSongPair(s, 1);
    g.GetNode("4")->AddSongPair(t, 1);
    g.GetNode("5")->AddSongPair(u, 1);
    g.GetNode("6")->AddSongPair(v, 1);
    return g;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                                Test Cases                                               //
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Song Equality") { 
    Song s = Song("a", "b", "c", "d");
    Song t = Song("a", "b", "c", "e");
    Song u = Song("a", "b", "c", "e");
    
    REQUIRE(t == u); 
    REQUIRE_FALSE(s == t);
}

TEST_CASE("Request Song") {
    Node n("artist");
    Song q = Song("a1", "b", "artist", "A");
    Song r = Song("a2", "b", "artist", "B");
    Song s = Song("a3", "b", "artist", "C");
    Song t = Song("a4", "b", "artist", "D");
    Song u = Song("a5", "b", "artist", "E");
    Song v = Song("a6", "b", "artist", "F");
    n.AddSongPair(q, 6);
    n.AddSongPair(r, 5);
    n.AddSongPair(s, 4);
    n.AddSongPair(t, 3);
    n.AddSongPair(u, 2);
    n.AddSongPair(v, 1);
    REQUIRE(n.RequestSong(50) == t);
    REQUIRE(n.RequestSong(50) == u);
    REQUIRE(n.RequestSong(50) == s);
    REQUIRE(n.RequestSong(50) == v);
    REQUIRE(n.RequestSong(50) == r);
    REQUIRE(n.RequestSong(50) == q);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                            DJISKRAS Test Cases                                          //
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Can find path between nodes")  {

  vector<Playlist> a;
    
    Graph graph;
    a = ParseCSV("../tests/test_playlist.csv");

    graph.analyze_all_playlists(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(1, true);
    }


    // Uncommented to visualize the connections within the graph
    /*for (auto y: graph.getGraph()) 
       for (auto x : y.second.GetNeighbors()) 
               cout << y.first << "-"  << x.first->GetArtist() << " " << x.second << endl;
    */

    // There exists the following path between each of the following
    vector<string> correct1 = {"\"Shaggy\"", "\"OutKast\"", "\"Usher\""};
    vector<string> output1 = graph.Dijkstras("\"Shaggy\"", "\"Usher\"");
    REQUIRE(correct1 == output1);

    vector<string> correct2 = {"\"Usher\"", "\"OutKast\""};
    vector<string> output2 = graph.Dijkstras("\"Usher\"", "\"OutKast\"");
    REQUIRE(correct2 == output2);

    // There are no paths between the following nodes
    vector<string> output3 = graph.Dijkstras("\"Shaggy\"", "\"Justin Timberlake\"");
    vector<string> output4 = graph.Dijkstras("\"Shaggy\"", "\"Destiny's Child\"");
    REQUIRE(output3.empty());
    REQUIRE(output4.empty());
}

TEST_CASE("Can choose the minimum path")  {
  vector<Playlist> a;
    
    Graph graph2;
    a = ParseCSV("../tests/test_playlist_large.csv");

    graph2.analyze_all_playlists(a);
    for (auto x: graph2.getGraph()) {
        graph2.GetNode(x.first)->TrimNeighbors(3, true);
    }
    
    /*
    Uncomment to visualize the connections within the graph
    vector<string> c = {"\"Soul Asylum\"", "\"The Smashing Pumpkins\"", "\"Nirvana\"", "\"The Goo Goo Dolls\"", "\"Incubus\""};
    for (auto y: graph.getGraph()) {
       for (auto x : y.second.GetNeighbors()) {
            if (y.first == c[0] || y.first == c[1] || y.first == c[2] || y.first == c[3] || y.first == c[4])
                cout << y.first << "-"  << x.first->GetArtist() << " " << x.second << endl;
        }      
        // cout << endl;
    }
    */
      
    // There exists the following path between these two nodes
    // "Soul Asylum" "The Smashing Pumpkins"  "Niravana" "Incubus" with more cost
    vector<string> correct1 = {"\"Soul Asylum\"", "\"The Smashing Pumpkins\"", "\"Nirvana\""};
    vector<string> output1 = graph2.Dijkstras("\"Soul Asylum\"",  "\"Nirvana\"");
    REQUIRE(correct1 == output1);

     // There exists the following path between these two nodes
    // "Soul Asylum" "The Smashing Pumpkins" "Incubus" "The Goo Goo Dolls" with more cost
    vector<string> correct2 = {"\"Soul Asylum\"", "\"The Smashing Pumpkins\"", "\"Nirvana\"","\"The Goo Goo Dolls\"" };
    vector<string> output2 = graph2.Dijkstras("\"Soul Asylum\"", "\"The Goo Goo Dolls\"");
    REQUIRE(correct2 == output2);

    vector<string> correct3 = graph2.Dijkstras("\"Tee Grizzley\"", "\"blackbear\"");
    vector<string> output3 = graph2.Dijkstras("\"Tee Grizzley\"", "\"blackbear\"");
    REQUIRE(correct3 == output3);
}

// Tests if the trimmed neighbors function runs correctly
TEST_CASE("Trimmed Neighbors") { 
    vector<Playlist> a;
    
    //a = parse("./tests/old_testing/SongsLimit.json");
    Graph graph;
    a = ParseCSV("../tests/test_playlist.csv");
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

// Checks that a small playlist can be created
TEST_CASE("Small Playlist") { 
    vector<Playlist> a;
    
    Graph graph;
    a = ParseCSV("../tests/test_playlist.csv");
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

// Checks that a large playlist can be created
TEST_CASE("Large Playlist") {
    vector<Playlist> a;
    
    Graph graph;
    a = ParseCSV("../tests/test_playlist_large.csv");
    graph.analyze_all_playlists(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(5, true);
    }
    
    vector<Song> song_prefs;
    song_prefs.push_back(graph.GetNode("\"Missy Elliott\"") -> FindSong("\"Lose Control (feat. Ciara & Fat Man Scoop)\""));
    Playlist p = graph.CreatePlaylist(9, song_prefs);

    REQUIRE(p.GetSongs().size() == 9);

    REQUIRE(p.ContainsSongByName("\"One Less Lonely Girl\"")); 
    REQUIRE(p.ContainsSongByName("\"Run It!\""));
    REQUIRE(p.ContainsSongByName("\"Forever\""));
}   


TEST_CASE("Constructed Graph Playlist") {
    Song q = Song("a1", "b", "1", "A");
    Song r = Song("a2", "b", "2", "B");
    Song s = Song("a3", "b", "3", "C");
    Song t = Song("a4", "b", "4", "D");
    Song u = Song("a5", "b", "5", "E");
    Song v = Song("a6", "b", "6", "F");
    std::vector<Song> input_songs = {q, r, s, t, u, v};
    Graph g = CustomGraph(input_songs);
    std::vector<Song> songs = {q};

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

TEST_CASE("Constructed Graph Similarity")   {
    Song q = Song("a1", "b", "1", "A");
    Song r = Song("a2", "b", "2", "B");
    Song s = Song("a3", "b", "3", "C");
    Song t = Song("a4", "b", "4", "D");
    Song u = Song("a5", "b", "5", "E");
    Song v = Song("a6", "b", "6", "F");
    std::vector<Song> input_songs = {q, r, s, t, u, v};
    Graph g = CustomGraph(input_songs);

    REQUIRE(g.similarity("1", "2", 1));
    REQUIRE(g.similarity("1", "2", 2));
    REQUIRE(g.similarity("1", "4", 3));
    REQUIRE(g.similarity("1", "5", 4));
    REQUIRE(g.similarity("1", "6", 4));
    REQUIRE(g.similarity("2", "6", 3));
    REQUIRE(g.similarity("3", "6", 2));
    REQUIRE_FALSE(g.similarity("1", "4", 2));
    REQUIRE_FALSE(g.similarity("2", "6", 2));
    REQUIRE_FALSE(g.similarity("1", "2", 0));
}

TEST_CASE("Graph Similarity")   {
   vector<Playlist> a;
    
    Graph graph;
    a = ParseCSV("../tests/test_playlist_large.csv");
    graph.analyze_all_playlists(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(3, true);
    }
    
    REQUIRE(graph.similarity("\"Missy Elliott\"", "\"Justin Bieber\"", 1));
    REQUIRE(graph.similarity("\"Missy Elliott\"", "\"The Black Eyed Peas\"", 2));
    REQUIRE_FALSE(graph.similarity("\"Missy Elliott\"", "\"The Black Eyed Peas\"", 1));
    REQUIRE_FALSE(graph.similarity("\"Missy Elliott\"", "\"Flo Rida\"", 1));
}

TEST_CASE("Constructed Graph Give Me Something New")  {
    Song q = Song("a1", "b", "1", "A");
    Song r = Song("a2", "b", "2", "B");
    Song s = Song("a3", "b", "3", "C");
    Song t = Song("a4", "b", "4", "D");
    Song u = Song("a5", "b", "5", "E");
    Song v = Song("a6", "b", "6", "F");
    std::vector<Song> input_songs = {q, r, s, t, u, v};
    Graph g = CustomGraph(input_songs);

    REQUIRE(g.somethingNew("1", 3) == "4");
    REQUIRE(g.somethingNew("1", 4) == "5");
    REQUIRE((g.somethingNew("2", 1) == "1" || g.somethingNew("2", 1) == "3"));
}

TEST_CASE("Give Me Something New")  {
    vector<Playlist> a;
    Graph graph;
    a = ParseCSV("../csvdata/mpd.slice.0-999.csv");
    graph.analyze_all_playlists(a);
    for (auto x: graph.getGraph()) {
        graph.GetNode(x.first)->TrimNeighbors(5, true);
    }
    REQUIRE(graph.somethingNew("\"Missy Elliott\"", 1) == "\"Anthony Hamilton\"");
    REQUIRE(graph.somethingNew("\"Missy Elliott\"", 2) == "\"Chrisette Michele\"");
    REQUIRE(graph.somethingNew("\"Missy Elliott\"", 3) == "\"Damian Marley\"");
    REQUIRE(graph.somethingNew("\"Missy Elliott\"", 4) == "Not Enough Connections");
}


