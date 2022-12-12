#include "../src/graph.h"
#include "../src/utils.h"

#include <fstream>
#include <ctime>
#include <chrono>
#include <sstream>
#include <cmath>
#include <cstring>
using namespace std;

/**
* Print a Song.
*/
void PrintSong(Song& s) {
    std::cout << s._name << endl << s._album << endl << s._artist << endl << endl;
}

/**
* If user misstyped a command, this method will try to predict what command they attempted
* to type. If none is found, it will return the command options.
*/
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
"make playlist    -> generates a playlist from three song preferences, optionally push to spotify\n"
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
            if (playlists <= 0) {
                cout << "                            Invalid Parameter" << endl;
                continue;
            }
            if (playlists > 10) {
                cout << "                            Too Many Playlists" << endl;
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
            cout << "Similarity: ";
            getline(cin, param);
            stringstream value(param);
            int distance = 0;
            value >> distance;
            if (distance <= 0) {
                cout << "         Invalid Parameter" << endl;
                continue;
            }
            if (distance > 10) {
                cout << "         Depth Too Far" << endl;
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
            cout << "Source Artist: ";
            getline(cin, param);
            source = "\"" + param + "\"";
            if (user_graph.getGraph().count(source) == 0) {
                cout << "               Artist Not Found" << endl;
                continue;
            }
            cout << "Destination artist: ";
            getline(cin, param);
            dest = "\"" + param + "\"";
            if (user_graph.getGraph().count(dest) == 0) {
                cout << "                    Artist Not Found" << endl;
                continue;
            }
            vector<string> artists = user_graph.Dijkstras(source, dest);
            if(artists.size() == 0) {
                cout << "Path not Found" << endl;
                continue;
            }
            for (string artist: artists) {
                cout << artist << endl;
            }
            continue;

        }
        if (command == "carlevans") { //THE GOAT
            cout << "THE GOAT" << endl;
            continue;
        }
        if (command == "similarity") {
            if (graph_generated == false) {
                cout << "         No Graph Generated" << endl;
                continue;
            }
            string source = "";
            string dest = "";
            cout << "Source Artist: ";
            getline(cin, param);
            source = "\"" + param + "\"";
            if (user_graph.getGraph().count(source) == 0) {
                cout << "        Artist Not Found" << endl;
                continue;
            }
            cout << "Destination Artist: ";
            getline(cin, param);
            dest = "\"" + param + "\"";
            if (user_graph.getGraph().count(dest) == 0) {
                cout << "        Artist Not Found" << endl;
                continue;
            }
            cout << "Distance: ";
            getline(cin, param);
            stringstream value(param);
            int distance = 0;
            value >> distance;
            if (distance <= 0) {
                cout << "          Invalid Parameter" << endl;
                continue;
            }
            if (distance >= 20) {
                cout << "          Depth Too Far" << endl;
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
            cout << "Artist: ";
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
            if (num_songs <= 0) {
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

            cout << "Would you like to save this playlist to spotify? (Check readme BEFRORE saying yes)" << endl;
            cout << "(y/n): ";
            string confirmation;
            getline(cin, confirmation);

            for (size_t i = 0; i < confirmation.length(); i++)  {
                confirmation[i] = tolower(confirmation[i]);
                if (confirmation.at(i) == ' ' || !isalpha(confirmation.at(i)))   {
                    confirmation.erase(confirmation.begin() + i);
                    i--;
                }
            }

            if (confirmation != "y" && confirmation != "yes") {
                continue;
            }

            cout << "User ID (check readme for instructions): ";
            string user_id;
            getline(cin, user_id);

            cout << "Token (check readme for instructions): ";
            string token;
            getline(cin, token);

            string playlist_id = user_graph.CreateSpotifyPlaylist(user_id, token, playlist_name, "", false);
            if (playlist_id == "error") {
                cout << "An Error Has Occured - Could Not Create Playlist" << endl;
                continue;
            }

            user_graph.SendPlaylistToSpotify(playlist, token, playlist_id);
            std::cout <<"Your Playlist: " << "https://open.spotify.com/playlist/" + playlist_id << std::endl;
            continue;
        }
        cout << "         Invalid Command. Did you mean " << CompareCommand(command) << endl;
    }
    return 0;
}




