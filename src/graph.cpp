#include "graph.h"
#include "utils.h"

#include <unordered_set>
#include <stack>
#include <fstream>
#include <cstring>
#include <chrono>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

//generates a graph given a csv file of playlists from the dataset
Graph::Graph(string file)   {
    vector<Playlist> a = ParseCSV(file);
    analyze_all_playlists(a);
}

//generates a graph from a vector of playlists that have already been parsed
Graph::Graph(vector<Playlist>& playlists)    {
    analyze_all_playlists(playlists);
}

// Loops through all the playlists so that multiple playlists can be added to the graph at once
void Graph::analyze_all_playlists(vector<Playlist>& playlists) {
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

// Returns the node related to a given artist
Node* Graph::GetNode(string artist) {
    return &_graph[artist];
}

// Returns the neighbors of a given artist
unordered_map<Node*, int>& Graph::FindNeighbors(string artist) {
    return _graph[artist].GetNeighbors();
}

/**
* Create a playlist using IDDFS with num_songs. This accepts a vector of Songs used as starting
* nodes. Ideally, this vector should contain 3-5 Songs. Returns the Playlist.
*/
Playlist Graph::CreatePlaylist(int num_songs, vector<Song>& preferences)    {
    int sum = 0;
    //Find average popularity (or song_tier) of the songs.
    for (Song& s : preferences) {
        int temp = 100*GetNode(s._artist) -> FindSongPlacement(s._name);
        int temp2 = GetNode(s._artist) -> SongCount();
        sum += temp / temp2;
    }
    int average = sum/preferences.size();
    vector<Song> playlist;
    //Iteratively increase depth.
    for (int depth = 0; depth < num_songs; depth++) {
        //Repeat for each song in preferences.
        for (Song& s : preferences) {
            unordered_map<string, bool> visited;
            Node* node = GetNode(s._artist);
            if (node -> SongCount() == 0) {
                continue;
            }
            //Recrusive step
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
    dist[source] = 0;                                                              

    // Min Heap
    typedef pair<double, string> pi;
    std::priority_queue<pi, vector<pi>, greater<pi> > pq;
    
    for (auto const& vertex : _graph) {
        if (vertex.first != source) {
            dist[vertex.first] = INT_MAX;     // Unknown distance from source to v
            prev[vertex.first] = "";          // Predecessor of v
            pq.push(make_pair(dist[vertex.first], vertex.first));                
        } else {

            pq.push(make_pair(dist[source], source));
        }
    }

    while (!pq.empty())   {                    // The main loop
        pi u = pq.top();                       // Best Vertex
        pq.pop();                              // Remove best vertex
        if (dist[u.second] == INT_MAX) {  return vector<string>();  }
        for (auto const& neighbor: _graph[u.second].GetNeighbors()) {       // Go through all v neighbors of u
            auto vertex = neighbor.first -> GetArtist();                
            if (vertex == dest) {            
                // If we have reached our goal
                string temp = u.second;
                vector<string> to_return;
                to_return.push_back(vertex);
                while(temp != "") {
                    to_return.push_back(temp);
                    temp = prev[temp];
                }
                // Directions are backwards
                reverse(to_return.begin(), to_return.end());
                return to_return;
            }
            
            // Adding neighbors to graph 
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

/**
* Recursive component of the IDDFS. Runs until playlist contains num_songs, or depth < 0. 
* Ensures that nodes are not visited twice within the same DFS. 
*/
bool Graph::RecurseDFS(Node* source, int num_songs, int depth, vector<Song>& playlist, unordered_map<string, bool>& visited, int song_tier)  {
    //Check if finished
    if ((int)playlist.size() >= num_songs)   {
        return true;
    }
    //Check depth
    if (depth < 0)  {
        return false;
    }
    //Add a song from the source node
    playlist.push_back(source -> RequestSong(song_tier));
    visited[source -> GetArtist()] = true;
    unordered_map<Node*, int>& neighbors =  source -> GetNeighbors();

    unordered_map<Node*, int>::iterator it;
    if (neighbors.size() > 3)   {
        //Use only the top 3 neighbors for better results, but do so without trimming neighbors.
        Node* top3[3];
        for (int i = 0; i < 3; i++) {
            top3[i] = NULL;
        }
        for (it = neighbors.begin(); it != neighbors.end(); it++)  {
            if (top3[0] == NULL || it -> second > neighbors[top3[0]])    {
                top3[2] = top3[1];
                top3[1] = top3[0];
                top3[0] = it -> first;
            }
            else if (top3[1] == NULL || it -> second > neighbors[top3[1]])  {
                top3[2] = top3[1];
                top3[1] = it -> first;
            }
            else if (top3[2] == NULL || it -> second > neighbors[top3[2]])  {
                top3[2] = it -> first;
            }
        }
        for (Node* n : top3)    {
            if (n == NULL)  {
                continue;
            }
            
            if (visited[n -> GetArtist()])   {
                continue;
            }
            //Recurse
            if (RecurseDFS(n, num_songs, depth - 1, playlist, visited, song_tier))   {
                return true;
            }
        }
    }
    else    {
        //Run DFS on each neighbor, same as above just without choosing top 3 neighbors.
        for (it = neighbors.begin(); it != neighbors.end(); it++)  {
        if (visited[it -> first -> GetArtist()])   {
            continue;
        }
        if (RecurseDFS(it -> first, num_songs, depth - 1, playlist, visited, song_tier))   {
            return true;
        }
    }
    }
    return false;
}

/**
* Send a Playlist to Spotify using curl commands, which must be installed. Requires a 
* user token.
*/
bool Graph::SendPlaylistToSpotify(Playlist& playlist, string uid, string playlist_id) {
    //Spotify only accepts requests of up to 100 songs, so run this for each 100 to handle larger playlists.
    for (int hundredcount = 0; hundredcount < (playlist.SongCount() - 1)/100 + 1; hundredcount++) {
        //std::ifstream myfile ("../terminal_output.txt");
        chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
        );


        chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
        );
        //Wait for curl command to be complete. It has a chance of failure.
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
        //Build command by reading from a file and inserting user input where necessary.
        string command = "";
        fstream file("../src/add_to_playlist_command.txt");
        string s;
        for (int i = 0; i < PLAYLIST_ID - 1; i++)   {
            std::getline(file, s);
            command += s;
        }
        std::getline(file,s); //this is just the placeholder playlist id
        command += playlist_id;
        for (int i = PLAYLIST_ID; i < URI_START - 1; i++)   {
            std::getline(file, s);
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
        std::getline(file, s);
        for (int i = URI_START; i < USER_TOKEN - 1; i++)    {
            std::getline(file, s);
            command += s;
        }
        std::getline(file, s); //temp uid
        command += uid;
        while (std::getline(file,s)) {
            command += s;
        }
        command += " > ../terminal_output.txt";
        char* char_array = new char[command.length() + 1];
        strcpy(char_array, command.c_str());
        //Run the command
        system(char_array);
        delete[] char_array;
    }
    return true;
}
/**
* Create a Playlist on Spotify using curl commands, which must be installed. Requires a 
* user token and user id.
*/
string Graph::CreateSpotifyPlaylist(string user_id, string token, string name, string description, bool pub) {
    std::ofstream ofs;
    ofs.open("../terminal_output.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    const int USER_ID = 3;
    const int PLAYLIST_NAME = 7;
    const int PLAYLIST_DESCRIPTION = 12;
    const int PUBLIC = 16;
    const int USER_TOKEN = 19;
    //Build command by reading from a file and inserting user input where necessary. 
    string command = "";
    fstream file("../src/create_playlist_command.txt");
    string s;

    for (int i = 0; i < USER_ID - 1; i++)   {
        std::getline(file, s);
        command += s;
    }
    std::getline(file,s); //this is just the placeholder user id
    command += user_id;
    for (int i = USER_ID; i < PLAYLIST_NAME - 1; i++)   {
        std::getline(file, s);
        command += s;
    }
    std::getline(file,s); //this is just the placeholder playlist name
    command += name;

     for (int i = PLAYLIST_NAME; i < PLAYLIST_DESCRIPTION - 1; i++)   {
       std::getline(file, s);
        command += s;
    }
    std::getline(file,s); //this is just the placeholder playlist description
    command += description;

     for (int i = PLAYLIST_DESCRIPTION; i < PUBLIC - 1; i++)   {
        std::getline(file, s);
        command += s;
    }
    std::getline(file,s); //this is just the placeholder public bool
    command += pub ? "true" : "false";

     for (int i = PUBLIC; i < USER_TOKEN - 1; i++)   {
        std::getline(file, s);
        command += s;
    }
    std::getline(file,s); //this is just the placeholder user token
    command += token;
    while (std::getline(file,s)) {
        command += s;
    }
    command += " > ../terminal_output.txt";
    //cout << command << endl;
    char* char_array = new char[command.length() + 1];
    strcpy(char_array, command.c_str());
    system(char_array);
    delete[] char_array;
    std::ifstream myfile ("../terminal_output.txt");
    chrono::milliseconds t_start = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );


    chrono::milliseconds t_final = chrono::duration_cast< chrono::milliseconds >(
    chrono::system_clock::now().time_since_epoch()
    );
    //Wait so that the command has time to complete
    while (((t_final-t_start)/1000.0).count() < 2)    {
        t_final = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
        );
    }
    //Wait for response
    while (myfile.get() != '{')    {

    }
    string line;
    //Read response and either return "error" or the playlist id.
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

//recommends a new artist given a starting artist and a distance
string Graph::somethingNew(string artist, int distance) {
    vector<string> visited;
    visited.push_back(artist);
    //the only artist within 0 nodes is itself
    if (distance == 0) {
        return artist;
    }

    //store the neighbors in a priority queue to iterate through the most related neighbors
    auto cmp = [](const pair<Node*, int>& lhs, const pair<Node*, int>& rhs)
    { return lhs.second < rhs.second;};
    priority_queue<pair<Node*, int>, vector<pair<Node*, int>>, decltype(cmp) > pQ(cmp);
    for(auto const& x : this->FindNeighbors(artist)) {
        pQ.push(x);
    }

    while(!pQ.empty()) {
        if (find(visited.begin(), visited.end(), pQ.top().first->GetArtist()) == visited.end()) {
            //if the neighbor was not visited yet, recurse through it to find a new artist
            visited.push_back(pQ.top().first->GetArtist());
            string returned = somethingNew(pQ.top().first->GetArtist(), distance - 1, visited, artist, distance);
            //if the returned value is not an empty string, an artist was found
            if (returned != "") {
                return returned;
            }
            visited.pop_back();
        }
        pQ.pop();
    }
    //if there is no artist found after recursing through all the neighbors, then there is not a path long enough for the given distance
    return "Not Enough Connections";
}

//helper function for somethingNew
//almost the same thing but with extra inputs
string Graph::somethingNew(string artist, int distance, vector<string>& visited, string og_artist, int og_distance) {
    if (distance == 0) {
        //if the max distance was reached and the artist is within a certain threshold, return the artist
        if ((og_distance - og_distance * 0.2 - 1) <= 0 || !similarity(og_artist, artist, og_distance - og_distance * 0.4 - 1)) {
            return artist;
        }
        //if the max distance was reached but the artist is not within the threshold, return an empty string
        return "";
    }

    //store the neighbors in a priority queue to iterate through the most related neighbors
    auto cmp = [](const pair<Node*, int>& lhs, const pair<Node*, int>& rhs)
    { return lhs.second < rhs.second;};
    priority_queue<pair<Node*, int>, vector<pair<Node*, int>>, decltype(cmp) > pQ(cmp);
    for(auto const& x : this->FindNeighbors(artist)) {
        pQ.push(x);
    }

    while(!pQ.empty()) {
        if (find(visited.begin(), visited.end(), pQ.top().first->GetArtist()) == visited.end()) {
            //if the neighbor was not visited yet, recurse through it to find a new artist
            visited.push_back(pQ.top().first->GetArtist());
            string returned = somethingNew(pQ.top().first->GetArtist(), distance - 1, visited, og_artist, og_distance);
            //if the returned value is not an empty string, an artist was found
            if (returned != "") {
                return returned;
            }
            visited.pop_back();
        }
        pQ.pop();
    }
    //if there is no artist found after recursing through all the neighbors, return the empty string
    return "";
}

//returns a boolean for if an artist is within a given distance of another artist
bool Graph::similarity(string artist1, string artist2, int distance) {
    //Two artists cannot be within 0 nodes of each other
    if (distance <= 0) {
        return false;
    }
    if (artist1 == artist2) {
        return true;
    }

    vector<string> path;
    path.push_back(artist1);

    for (auto neighbor: this->FindNeighbors(artist1)) {
        //if one of the first artists neighbors is the second artist, we found the second artist
        if (neighbor.first->GetArtist() == artist2) {
            return true;
        }
        //otherwise, recurse to keep looking for the second artist
        path.push_back(neighbor.first->GetArtist());
        if(similarity(neighbor.first->GetArtist(), artist2, distance-1, path)) {
            return true;
        } else {
            path.pop_back();
        }
    }
    //if the artist was not found, return false
    return false;
}

//helper function for similarity
//almost the same thing but takes the path as input
bool Graph::similarity(string artist1, string artist2, int distance, vector<string>& path) {
    //if the distance is 0, we recursed too far
    if (distance == 0) {
        return false;
    }
    for (auto neighbor: this->FindNeighbors(artist1)) {
        //if one of the first artists neighbors is the second artist, we found the second artist
        if (neighbor.first->GetArtist() == artist2) {
            return true;
        }
        if(find(path.begin(), path.end(), neighbor.first->GetArtist()) == path.end()) {
            //if the neighbor is not in the visited path, recurse through the neighbor to find the second artist
            path.push_back(neighbor.first->GetArtist());
            if(similarity(neighbor.first->GetArtist(), artist2, distance-1, path)) {
                return true;
            } else {
                path.pop_back();
            }
        }
    }
    //if the second artist was not found, return false
    return false;
}
