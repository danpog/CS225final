#include "graph.h"
#include "song.h"
#include "playlist.h"
#include "node.h"

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <cstring>
#include <chrono>
#include <queue>

using namespace std;


Graph::Graph(string file) {
    Json::Value all_nodes_json;
    std::ifstream graph_file(file);
    graph_file >> all_nodes_json;
    Json::StreamWriterBuilder builder;

    string artist;
    for (Json::Value node: all_nodes_json["nodes"]) {
        artist = Json::writeString(builder, node["artist"]);
        _graph.insert(pair<string, Node>(artist, Node(artist)));
    }

    Node* artist2;
    int frequency;
    string name;
    string album;
    string artist3;
    string uri;
    int frequency2;
    for (Json::Value node: all_nodes_json["nodes"]) {
        artist = Json::writeString(builder, node["artist"]);
        for (Json::Value neighbor: node["neighbors"]) {
            artist2 = &_graph[Json::writeString(builder, neighbor["artist"])];
            frequency = neighbor["frequency"].asInt();
            _graph[artist].AddNeighborPair(pair<Node*, int>(artist2, frequency));
        }
        for (Json::Value song: node["songs"]) {
            name = Json::writeString(builder, song["name"]);
            album = Json::writeString(builder, song["album"]);
            artist3 = Json::writeString(builder, song["artist"]);
            uri = Json::writeString(builder, song["uri"]);
            frequency2 = song["frequency"].asInt();
            Song s = Song(name, album, artist3, uri);
            _graph[artist].AddSongPair(s, frequency2);
        }
    }
}

Graph::Graph(vector<Playlist>& playlists)    {
    analyze_all_playlists(playlists);
}

void Graph::analyze_all_playlists(vector<Playlist>& playlists) {
    /*for(Playlist playlist: playlists) {
        analyze_playlist(playlist);
    }*/
    while (!playlists.empty()) {
        Playlist& playlist = playlists[0];
        analyze_playlist(playlist);
        playlists.erase(playlists.begin());
    }
}

// Adds a playlist to a graph 
//      -> Updates the artists popular songs
//      -> Adds all the neighbors (undirected)
void Graph::analyze_playlist(Playlist& playlist) {
    update_graph(playlist);
    for (int i = 0; i < playlist.SongCount(); i++) {
        Song key_song = playlist.GetSong(i);
        _graph[key_song._artist].AddSong(key_song);
        for (int j = i; j < playlist.SongCount(); j++) {
            Song value_song = playlist.GetSong(j);
            if (key_song._artist != value_song._artist) {
                _graph[key_song._artist].AddNeighbor(&_graph[value_song._artist]);
            }
        }
    }
}

// Ensures that there is a node for each artist in playlist
void Graph::update_graph(Playlist& playlist) {
    string artist;
    for (int i = 0; i < playlist.SongCount(); i++) {
        artist = playlist.GetSong(i)._artist;
        _graph.insert(pair<string, Node>(artist, Node(artist)));
    }
}

Node* Graph::GetNode(string artist) {
    return &_graph[artist];
}

unordered_map<Node*, int>& Graph::FindNeighbors(string artist) {
    return _graph[artist].GetNeighbors();
}

void Graph::save_graph(string file) {
    ofstream saved_graph;

    saved_graph.open(file);
    saved_graph << "{";
    saved_graph << "\"nodes\": [";

    unordered_map<Node*, int> neighbors;
    unordered_map<Node*, int>::iterator it_neighbors;
    unordered_map<string, Node>::iterator it;
    int neighbor_counter;
    int song_counter;

    for (it = _graph.begin(); it != _graph.end(); it++) {
        neighbors = _graph[it->first].GetNeighbors();
        saved_graph << "{";
        saved_graph << "\"artist\": " << it->first << ",";
        saved_graph << "\"neighbors\": [";
        neighbor_counter = 1;
        for (it_neighbors = neighbors.begin(); it_neighbors != neighbors.end(); it_neighbors++) {
            saved_graph << "{";
            saved_graph << "\"artist\": " << it_neighbors->first->GetArtist() << ", ";
            saved_graph << "\"frequency\": " << it_neighbors->second << "}";
            if (neighbor_counter < int(neighbors.size())) {
                saved_graph << ",";
            }
            neighbor_counter++;
        }
        saved_graph << "],";
        saved_graph << "\"songs\": [";
        song_counter = 1;
        vector<pair<Song,int>> songs = _graph[it->first].GetAllSongs();
        for (pair<Song,int> song: songs) {
                saved_graph << "{";
                saved_graph << "\"name\": " << song.first._name << ", ";
                saved_graph << "\"album\": " << song.first._album << ", ";
                saved_graph << "\"artist\": " << song.first._artist << ", ";
                saved_graph << "\"uri\": " << song.first._uri << ", ";
                saved_graph << "\"frequency\": " << song.second << "}";
            if (song_counter < int(songs.size())) {
                saved_graph << ",";
            }
            song_counter++;
        }
        saved_graph << "]";
        saved_graph << "},";
    }
    saved_graph << "{}";
    saved_graph << "]";
    saved_graph << "}";
    saved_graph.close();

    /*Json::Value graph_json; 
    std::ifstream graph_file(file);
    graph_file >> graph_json;
    Json::StreamWriterBuilder builder;
    std::string artist = Json::writeString(builder, graph_json["nodes"][0]["artist"]);
    std::string neighbor = Json::writeString(builder, graph_json["nodes"][0]["neighbors"][0]["artist"]);
    int frequency = graph_json["nodes"][0]["neighbors"][0]["frequency"].asInt();
    cout << endl << "artist: " << artist << " neighbor: " << neighbor << " frequency: " << frequency<< endl;*/
}
Playlist Graph::CreatePlaylist(int num_songs, vector<Song>& preferences)    {
    int sum = 0;
    for (Song& s : preferences) {
        int temp = 100*GetNode(s._artist) -> FindSongPlacement(s._name);
        int temp2 = GetNode(s._artist) -> SongCount();
        sum += temp / temp2;
    }
    int average = sum/preferences.size();
    vector<Song> playlist;
    for (int depth = 0; depth < num_songs; depth++) {
        for (Song& s : preferences) {
            unordered_map<string, bool> visited;
            Node* node = GetNode(s._artist);
            if (node -> SongCount() == 0) {
                continue;
            }
            if (RecurseDFS(node, num_songs, depth, playlist, visited, average))  {
                return Playlist(playlist);
            }
        }
    }
    return Playlist(playlist);
}
vector<string> Graph::Dijkstras(string source, string dest) {
    map<string, double> dist;
    map<string, string> prev;
    set<string> removed;
    dist[source] = 0;                           // Initialization

    // Min Heap
    typedef pair<double, string> pi;
    std::priority_queue<pi, vector<pi>, greater<pi> > pq;
    
    for (auto const& vertex : _graph) {
        if (vertex.first != source) {
            dist[vertex.first] = INT_MAX;                       // Unknown distance from source to v
            prev[vertex.first] = "";   
            pq.push(make_pair(dist[vertex.first], vertex.first));                // Predecessor of v
        } else {
            pq.push(make_pair(dist[source], source));
        }
    }

    while (!pq.empty())   {                    // The main loop
        pi u = pq.top();                       // Best Vertex
        pq.pop();                              // Remove best vertex
        if (dist[u.second] == INT_MAX) {  return vector<string>();  }
        for (auto const& neighbor: _graph[u.second].GetNeighbors()) {           // Go through all v neighbors of u
            auto vertex = neighbor.first -> GetArtist();
            if (vertex == dest) {
                string temp = u.second;
                vector<string> to_return;
                to_return.push_back(vertex);
                while(temp != "") {
                    to_return.push_back(temp);
                    temp = prev[temp];
                }
                reverse(to_return.begin(), to_return.end());
                return to_return;
            }
            
            double distance = dist[u.second] == INT_MAX ? 0 : dist[u.second];
            auto temp_dist = distance + (1.0 / _graph[u.second].GetWeight(vertex));
            if (temp_dist < dist[vertex]) {
                dist[vertex] = temp_dist;
                prev[vertex] = u.second;
                pq.push(make_pair(dist[vertex], vertex));
            }
        }
    }
    return vector<string>();
}

bool Graph::RecurseDFS(Node* source, int num_songs, int depth, vector<Song>& playlist, unordered_map<string, bool>& visited, int song_tier)  {
    if ((int)playlist.size() >= num_songs)   {
        return true;
    }
    if (depth < 0)  {
        return false;
    }
    playlist.push_back(source -> RequestSong(song_tier));
    visited[source -> GetArtist()] = true;
    unordered_map<Node*, int>& neighbors =  source -> GetNeighbors();

    unordered_map<Node*, int>::iterator it;
    for (it = neighbors.begin(); it != neighbors.end(); it++)  {
        if (visited[it -> first -> GetArtist()])   {
            continue;
        }
        if (RecurseDFS(it -> first, num_songs, depth - 1, playlist, visited, song_tier))   {
            return true;
        }
    }
    return false;
}

bool Graph::SendPlaylistToSpotify(Playlist& playlist, string uid, string playlist_id) {
    for (int hundredcount = 0; hundredcount < (playlist.SongCount() - 1)/100 + 1; hundredcount++) {
        std::ifstream myfile ("./terminal_output.txt");
        chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
        );


        chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
        );
        if (hundredcount != 0) {
            while (((t_final-t_start)/1000.0).count() < 2)    {
                t_final = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch()
                );
            }
        
        }
        const int PLAYLIST_ID = 3;
        const int URI_START = 6; 
        const int USER_TOKEN = 9;

        string command = "";
        fstream file("./add_to_playlist_command.txt");
        string s;
        for (int i = 0; i < PLAYLIST_ID - 1; i++)   {
            getline(file, s);
            command += s;
        }
        getline(file,s); //this is just the placeholder playlist id
        command += playlist_id;
        for (int i = PLAYLIST_ID; i < URI_START - 1; i++)   {
            getline(file, s);
            command += s;
        }
        int count = 0;
        for (int i = 100*hundredcount; i < std::min(playlist.SongCount(), 100*hundredcount + 100); i++)  {
            Song& song = playlist.GetSong(i);
            string uri = song._uri.substr(1, song._uri.size() - 2);
            if (count == 0) {
                command += uri;
            }
            else    {
                string uri2 = "%2C" + uri;
                command += uri2;
            }
            count++;
        }
        getline(file, s);
        for (int i = URI_START; i < USER_TOKEN - 1; i++)    {
            getline(file, s);
            command += s;
        }
        getline(file, s); //temp uid
        command += uid;
        while (getline(file,s)) {
            command += s;
        }
        command += " > terminal_output.txt";
        char char_array[command.length() + 1];
        strcpy(char_array, command.c_str());
        char* temp = char_array;
        system(temp);
    }
    return true;
}

string Graph::CreateSpotifyPlaylist(string user_id, string token, string name, string description, bool pub) {
    std::ofstream ofs;
    ofs.open("terminal_output.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    const int USER_ID = 3;
    const int PLAYLIST_NAME = 7;
    const int PLAYLIST_DESCRIPTION = 12;
    const int PUBLIC = 16;
    const int USER_TOKEN = 19;

    string command = "";
    fstream file("./create_playlist_command.txt");
    string s;

    for (int i = 0; i < USER_ID - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder user id
    command += user_id;
    for (int i = USER_ID; i < PLAYLIST_NAME - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder playlist name
    command += name;

     for (int i = PLAYLIST_NAME; i < PLAYLIST_DESCRIPTION - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder playlist description
    command += description;

     for (int i = PLAYLIST_DESCRIPTION; i < PUBLIC - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder public bool
    command += pub ? "true" : "false";

     for (int i = PUBLIC; i < USER_TOKEN - 1; i++)   {
        getline(file, s);
        command += s;
    }
    getline(file,s); //this is just the placeholder user token
    command += token;
    while (getline(file,s)) {
        command += s;
    }
    command += " > terminal_output.txt";
    char char_array[command.length() + 1];
    strcpy(char_array, command.c_str());
    char* temp = char_array;
    system(temp);
    std::ifstream myfile ("./terminal_output.txt");
    chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );


    chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    while (((t_final-t_start)/1000.0).count() < 2)    {
        t_final = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
        );
    }
    while (myfile.get() != '{')    {

    }
    string line;
    while (myfile.good())   {
        std::getline(myfile, line);
        if (line.size() < 8)    {
            continue;
        }
        if (line.substr(3, 5) == "error")   {
            return "error";
        }
        if (line.substr(2, 26) == "\"uri\" : \"spotify:playlist:")  {
            line = line.substr(28, line.size() - 29);
            break;
        }
        line = "";

    }
    return line;
}

string Graph::somethingNew(string artist1, int distance) {
    unordered_set <string> visited;
    vector<string> visited_ordered;
    int max_weight;
    Node* next_node;
    string cur_artist = artist1;
    visited.insert(cur_artist);
    for (int i = 0; i < distance; i++) {
        visited_ordered.push_back(cur_artist);
        max_weight = 0;
        for (auto neighbor: this->FindNeighbors(cur_artist)) {
            if(neighbor.second > max_weight && visited.find(neighbor.first->GetArtist()) == visited.end()) {
                max_weight = neighbor.second;
                next_node = neighbor.first;
            }
            visited.insert(neighbor.first->GetArtist());
        }
        cur_artist = next_node->GetArtist();
    }
    for (string artist: visited_ordered) {
        cout << artist << endl;
    }
    return cur_artist;
}

bool Graph::similarity(string artist1, string artist2, int distance) {
    if (distance == 0) {
        return false;
    }
    vector<string> path;
    path.push_back(artist1);
    for (auto neighbor: this->FindNeighbors(artist1)) {
        if (neighbor.first->GetArtist() == artist2) {
            cout << neighbor.first->GetArtist() << endl;
            return true;
        }
        path.push_back(neighbor.first->GetArtist());
        if(similarity(neighbor.first->GetArtist(), artist2, distance-1, path)) {
            cout << neighbor.first->GetArtist() << endl;
            return true;
        } else {
            path.pop_back();
        }
    }
    return false;
}

bool Graph::similarity(string artist1, string artist2, int distance, vector<string>& path) {
    if (distance == 0) {
        return false;
    }
    for (auto neighbor: this->FindNeighbors(artist1)) {
        if (neighbor.first->GetArtist() == artist2) {
            cout << neighbor.first->GetArtist() << endl;
            return true;
        }
        if(find(path.begin(), path.end(), neighbor.first->GetArtist()) == path.end()) {
            path.push_back(neighbor.first->GetArtist());
            if(similarity(neighbor.first->GetArtist(), artist2, distance-1, path)) {
                cout << neighbor.first->GetArtist() << endl;
                return true;
            } else {
                path.pop_back();
            }
        }
    }
    return false;
}