#include "song.h"
using namespace std;

Song::Song(std::string name, std::string album, std::string artist): 
    _name(name), _album(album), _artist(artist) {};

std::string Song::toString() const {
    std::string to_return = "name: " + _name + "\n" 
        + "album: " + _album + "\n" + "artist: " + _artist + "\n";
    return to_return;
}

std::string Song::GetArtist() const {
    return _artist;
}

bool operator==(const Song& LHS, const Song& RHS)    {
    return LHS._name == RHS._name && LHS._album == RHS._album && LHS._artist == RHS._artist;
}

ostream& operator<<(ostream &out, const Song &s)
{
    out << s.toString();
    return out;
}

bool operator!=(const Song& LHS, const Song& RHS)   {
    return !(LHS == RHS);
}