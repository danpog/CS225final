#include "song.h"
#include "playlist.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main()  {
    std::cout << "Ran" << std::endl;
    return 0;
}

vector<Playlist> parse(string filename)    {
    if (filename.empty()) {
        throw std::invalid_argument("No file name");
    }
    Playlist p = Playlist();
    vector<Playlist> to_return = vector<Playlist>();
    to_return.push_back(p);
    return to_return;
    /*fstream file;
    while (file.good()) {
        string n;
        cin >> n; // takes in all the other inputs
    } 
    */
}

/*
std::ifstream people_file("people.json", std::ifstream::binary);
    people_file >> people;

    cout<<people; //This will print the entire json object.

    //The following lines will let you access the indexed objects.
    cout<<people["Anna"]; //Prints the value for "Anna"
    cout<<people["ben"]; //Prints the value for "Ben"
    cout<<people["Anna"]["profession"]; //Prints the value corresponding to "profession" in the json for "Anna"

    cout<<people["profession"];
*/
