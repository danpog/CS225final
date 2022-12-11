#include "../src/graph.h"
#include "../src/utils.h"

#include <fstream>
#include <ctime>
#include <chrono>
#include <sstream>
#include <cmath>
#include <cstring>
using namespace std;

void PrintSong(Song& s) {
    std::cout << s._name << endl << s._album << endl << s._artist << endl << endl;
}
int min3(int x, int y, int z){
    if(x <= y && x <= z)
        return x;
    if(y <=x && y <= z)
        return y;
    return z;
}

void clear2D(int** arr, int rows){
  for(int i = 0; i < rows; i++){
    delete[] arr[i];
  }
  
  delete[] arr;
}
//this algorithm was very much found on the internet and slightly adapted
int LevenshteinDistance(string a, string b){
  int len_a = a.size();
  int len_b = b.size();
  int** matrix = new int*[len_a + 1];

  for(int i = 0; i < len_a + 1; i++)    {
    matrix[i] = new int[len_b + 1];
  }

  for(int i = 0; i < len_a + 1; i++)    {
    matrix[i][0] = i;
  }

  for(int j = 0; j < len_b + 1; j++)    {
    matrix[0][j] = j;
  }
    
  int ins;
  int del;
  int rep;

  for(int i = 1; i < len_a + 1; i++){
    for(int j = 1; j < len_b + 1; j++){
      if(a[i - 1] == b[j - 1]){
        matrix[i][j] = matrix[i - 1][j - 1];
      }
      else{
        ins = matrix[i][j - 1];
        del = matrix[i - 1][j];
        rep = matrix[i - 1][j - 1];
        matrix[i][j] = 1 + min3(ins, del, rep);
      }
    }
  }
  int to_return = matrix[len_a][len_b];
  clear2D(matrix, len_a + 1);
  return to_return;
}
string CompareCommand(string comm)  {
    vector<string> legal_commands = {"find neighbors", "break", "generate", "similarity", "something new", "path between", "make playlist", "help"};
    if (comm == "help")   {
        string to_return;
        to_return = "Use one of these commands: ";
        for (string i : legal_commands) {
            to_return += i + ", ";
        }
        
        to_return += "or use 'help' to get more information.";
        return to_return;
    }
    vector<int> dist;
    int max_string = 0;
    for (string str : legal_commands)  {
        if ((int)str.size() > max_string)    {
            max_string = str.size();
        }
        for (size_t i = 0; i < str.length(); i++)  {
            str[i] = tolower(str[i]);
            if (str.at(i) == ' ' || !isalpha(str.at(i)))   {
                str.erase(str.begin() + i);
                i--;
            }
        }
        dist.push_back(LevenshteinDistance(str, comm));
        
    }
    int min = dist[0];
    int idx = 0;
    for (size_t i = 0; i < dist.size(); i++)   {
        if (dist[i] < min) {
            min = dist[i];
            idx = i;
        }
    }
    string to_return;
    if (min > (int)(comm.length() - 1) || min > max_string)    {
        to_return = "one of these commands: ";
        for (string i : legal_commands) {
            to_return += i + ", ";
        }
        to_return.erase(to_return.begin() + to_return.size() - 1);
        to_return.erase(to_return.begin() + to_return.size() - 1);
        to_return += "?";
        return to_return;
    }
    return legal_commands[idx] + "?";
}
/*
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
*/
int main(int argc, char *argv[])  {
    cout << "Enter 'help' for a list of commands" << endl;
    string command;
    string param;
    Graph user_graph = Graph();
    bool graph_generated = false;
    while(true) {
        cout << "Command: ";
        command = "";
        getline(cin, command);
        //remove whitespace and make everything lowercase
        for (size_t i = 0; i < command.length(); i++)  {
            command[i] = tolower(command[i]);
            if (command.at(i) == ' ' || !isalpha(command.at(i)))   {
                command.erase(command.begin() + i);
                i--;
            }
        }
        if (command == "break") {
            break;
        }
        if (command == "help")  {
cout <<
"\nList of Commands:\n"
"break            -> ends the program\n"
"generate         -> generates a new graph\n"
"find neighbors   -> prints the neighbors of a given artist\n"
"path between     -> uses Dijkstra's algorithm to find the shortest weighted path between two artists\n"
"similarity       -> uses DFS to check if an artist is within a specified distance of another artist\n"
"something new    -> recommends a new artist at a specified distance (1-10, 1 = most similar, 10 = least similar)\n"
"make playlist    -> generates a playlist from three song preferences\n"
"help             -> show command list\n"
"Some of these commands will prompt you for additional parameters\n\n";
            continue;
        }
        if (command == "generate") {
            if (graph_generated == true) {
                cout << "         Graph Already Generated" << endl;
                continue;
            }
            cout << "# of playlists (thousands): ";
            param = "";
            getline(cin, param);
            stringstream value(param);
            int playlists = 0;
            value >> playlists;
            if (playlists == 0) {
                cout << "                            Invalid Parameter" << endl;
                continue;
            }

            cout << "Generating Graph..." << endl;
            vector<Playlist> a;
            for (int i = 0; i < playlists * 1000; i += 1000) {
                a = ParseCSV("../csvdata/mpd.slice."  + to_string(i) + "-" + to_string(i + 999) + ".csv");
                user_graph.analyze_all_playlists(a);
            }

            cout << "Trimming Neighbors..." << endl;
            for (auto x: user_graph.getGraph()) {
                user_graph.GetNode(x.first)->TrimNeighbors(5, true);
            }
            cout << "Graph Generation Complete" << endl;
            graph_generated = true;
            continue;
        }
        if (command == "somethingnew") {
            if (graph_generated == false) {
                cout << "         No Graph Generated" << endl;
                continue;
            }
            cout << "Artist: ";
            param = "";
            getline(cin, param);
            string artist = "\"" + param + "\"";
            if (user_graph.getGraph().count(artist) == 0) {
                cout << "        Artist Not Found" << endl;
                continue;
            }
            param = "";
            cout << "Disance: ";
            getline(cin, param);
            stringstream value(param);
            int distance = 0;
            value >> distance;
            if (distance == 0) {
                cout << "         Invalid Parameter" << endl;
                continue;
            }

            cout << user_graph.somethingNew(artist, distance) << endl;
            
            continue;
        }
        if (command == "pathbetween") {
            if (graph_generated == false) {
                cout << "         No Graph Generated" << endl;
                continue;
            }
            string source = "";
            string dest = "";
            cout << "Source_Artist: ";
            getline(cin, param);
            source = "\"" + param + "\"";
            if (user_graph.getGraph().count(source) == 0) {
                cout << "        Artist Not Found" << endl;
                continue;
            }
            cout << "Destination_artist: ";
            getline(cin, param);
            dest = "\"" + param + "\"";
            if (user_graph.getGraph().count(dest) == 0) {
                cout << "        Artist Not Found" << endl;
                continue;
            }
            for (string artist: user_graph.Dijkstras(source, dest)) {
                cout << artist << endl;
            }
            continue;

        }
        if (command == "similarity") {
            if (graph_generated == false) {
                cout << "         No Graph Generated" << endl;
                continue;
            }
            string source = "";
            string dest = "";
            cout << "Source_Artist: ";
            getline(cin, param);
            source = "\"" + param + "\"";
            if (user_graph.getGraph().count(source) == 0) {
                cout << "        Artist Not Found" << endl;
                continue;
            }
            cout << "Destination_Artist: ";
            getline(cin, param);
            dest = "\"" + param + "\"";
            if (user_graph.getGraph().count(dest) == 0) {
                cout << "        Artist Not Found" << endl;
                continue;
            }
            cout << "Distance: " << endl;
            getline(cin, param);
            stringstream value(param);
            int distance = 0;
            value >> distance;
            if (distance == 0) {
                cout << "         Invalid Parameter" << endl;
                continue;
            }
            cout << (user_graph.similarity(source, dest, distance) ? "true" : "false") << endl;
            continue;
        }
        if (command == "findneighbors") {
            if (graph_generated == false) {
                cout << "         No Graph Generated" << endl;
                continue;
            }
            cout << "Artist: " << endl;
            getline(cin, param);
            string artist = "\"" + param + "\"";
            if (user_graph.getGraph().count(artist) == 0) {
                cout << "        Artist Not Found" << endl;
                continue;
            }
            unordered_map<Node*, int> neighbors = user_graph.FindNeighbors(artist);
            unordered_map<Node*, int>::iterator it;
            for (it = neighbors.begin(); it != neighbors.end(); it++) {
                cout << it->first->GetArtist() << " " << it->second << endl;
            }
            continue;
        }
        if (command == "makeplaylist")  {
            if (graph_generated == false) {
                cout << "         No Graph Generated" << endl;
                continue;
            }

            string playlist_name;
            cout << "Name of Playlist: ";
            getline(cin, playlist_name);

            cout << "# of songs: ";
            getline(cin, param);
            stringstream value(param);
            int num_songs = 0;
            value >> num_songs;
            if (num_songs == 0) {
                cout << "            Invalid Parameter" << endl;
                continue;
            }
            if (num_songs > 1000) {
                cout << "            Too Many Songs" << endl;
                continue;
            }
            vector<Song> songs;
            cout << "Artist (1/3): ";
            getline(cin, param);
            string artist1 = "\"" + param + "\"";
            if (user_graph.getGraph().count(artist1) == 0) {
                cout << "              Artist Not Found" << endl;
                continue;
            }
            cout << "Song (1/3): ";
            getline(cin, param);
            string song1 = "\"" + param + "\"";
            songs.push_back(user_graph.GetNode(artist1)->FindSong(song1));

            cout << "Artist (2/3): ";
            getline(cin, param);
            string artist2 = "\"" + param + "\"";
            if (user_graph.getGraph().count(artist2) == 0) {
                cout << "              Artist Not Found" << endl;
                continue;
            }
            
            cout << "Song (2/3): ";
            getline(cin, param);
            string song2 = "\"" + param + "\"";
            songs.push_back(user_graph.GetNode(artist2)->FindSong(song2));

            cout << "Artist (3/3): ";
            getline(cin, param);
            string artist3 = "\"" + param + "\"";
            if (user_graph.getGraph().count(artist3) == 0) {
                cout << "              Artist Not Found" << endl;
                continue;
            }
            cout << "Song (3/3): ";
            getline(cin, param);
            string song3 = "\"" + param + "\"";
            songs.push_back(user_graph.GetNode(artist3)->FindSong(song3));

            Playlist playlist = user_graph.CreatePlaylist(num_songs, songs);
            playlist.SetName(playlist_name);

            cout << endl << playlist << endl;
            continue;
        }
        cout << "         Invalid Command. Did you mean " << CompareCommand(command) << endl;
    }
    return 0;
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
    */
    
    Graph graph = Graph();
    chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    
    vector<Playlist> a;

    // Creating the graph
    for (int i = 0; i < 10000; i += 1000) {
        //a = graph.ParseJSON("/workspaces/cs225env/spotify_million_playlist_dataset/data/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        //a = parse("mpdslices/mpd.slice." + to_string(i) + "-" + to_string(i + 999) + ".json");
        a = ParseCSV("/workspaces/cs225env/CS225final/csvdata/mpd.slice."  + to_string(i) + "-" + to_string(i + 999) + ".csv");
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
    //song_prefs.push_back(n -> FindSong("\"Almost Cut My Hair\""));
    
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
    /*
    if (make_playlist)  {
        Playlist p = graph.CreatePlaylist(num_songs, song_prefs);
        graph.SendPlaylistToSpotify(p, token, playlist_link);
    }
    */
    //else    { }
        //Playlist p = graph.CreatePlaylist(100, song_prefs);
        //cout << p << endl;
    
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
    // if (make_playlist)  { std::cout <<"Your Playlist: " << "https://open.spotify.com/playlist/" + playlist_link << std::endl; }
    
    return 0;
}




