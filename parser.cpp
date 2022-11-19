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
    Playlist p = Playlist();
    vector<Playlist> to_return = vector<Playlist>();
    to_return.push_back(p);
    
    Json::Value playlist; 

    std::ifstream playlist_file("SongsLimit.json");
    playlist_file >> playlist;

    cout << playlist; //This will print the entire json object.

    //The following lines will let you access the indexed objects.
    cout<<playlist["info"]; //Prints the value for "Anna"
    //cout<<people["ben"]; //Prints the value for "Ben"
    //cout<<people["Anna"]["profession"]; //Prints the value corresponding to "profession" in the json for "Anna"

    //cout<<people["profession"];


    return to_return;
    /*fstream file;
    while (file.good()) {
        string n;
        cin >> n; // takes in all the other inputs
    } 
    */
}

int main()  {
    std::cout << "Ran" << std::endl;
    vector<Playlist> a = parse("a");
    return 0;
}




