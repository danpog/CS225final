#include "song.h"
#include <iostream>
using namespace std;

Song::Song()    {
    std::size_t h1 = std::hash<std::string>{}(_name);
    std::size_t h2 = std::hash<std::string>{}(_artist);
    std::size_t h3 = std::hash<std::string>{}(_album);
    _hash = h1 ^ h2 ^ h3;
}
Song::Song(std::string name, std::string album, std::string artist): 
    _name(name), _album(album), _artist(artist) {
        std::size_t h1 = std::hash<std::string>{}(_name);
        std::size_t h2 = std::hash<std::string>{}(_artist);
        std::size_t h3 = std::hash<std::string>{}(_album);
        _hash = h1 ^ h2 ^ h3;
    };

std::string Song::toString() const {
    std::string to_return = "name: " + _name + "\n" 
        + "album: " + _album + "\n" + "artist: " + _artist + "\n";
    return to_return;
}

bool operator==(const Song& LHS, const Song& RHS)    {
    return hash<Song&>{}(LHS) == hash<Song&>{}(RHS);
    //return LHS._name == RHS._name && LHS._album == RHS._album && LHS._artist == RHS._artist;
}

ostream& operator<<(ostream &out, const Song &s)
{
    out << s.toString();
    return out;
}

bool operator!=(const Song& LHS, const Song& RHS)   {
    return !(LHS == RHS);
}