#include "song.h"
using namespace std;

Song::Song(std::string name, std::string album, std::string artist): 
    _name(name), _album(album), _artist(artist) {};

std::string Song::toString() {
    std::string to_return = "name: " + _name + "\n" 
        + "album: " + _album + "\n" + "artist: " + _artist + "\n";
    return to_return;
}