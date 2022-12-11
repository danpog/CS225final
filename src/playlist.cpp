#include "playlist.h"
#include "song.h"
#include <stdexcept>
using namespace std;

std::vector<Song>& Playlist::GetSongs() {
    return _songs;
}
void Playlist::AddSong(Song& song)  {
    _songs.push_back(song);
}
int Playlist::SongCount() const {
    return (int)(_songs.size());
}
int Playlist::GetID() const {
    return _id;
}
void Playlist::SetID(int id)  {
    _id = id;
}

void Playlist::SetName(string name) {
    _name = name;
}

Song& Playlist::GetSong(int i) {
    if (i < 0 || i >= SongCount())  {
        throw invalid_argument("Get song of index: " + to_string(i) + " is out of bounds.");
    }
    return _songs[i];
}

bool Playlist::ContainsSongByName(string name) {
    for (Song& i : _songs)  {
        if (i._name == name)    {
            return true;
        }
    }
    return false;
}

ostream& operator<<(ostream &out, const Playlist &p)
{
    out << "Name of Playlist: " << p._name << endl;
    out << "Song count: " << p.SongCount() << endl;
    for (Song s: p._songs) {
        out << s._name << " | " << s._album << " | " << s._artist << endl;
    }
    out << "---------------------" << endl;
    return out;
}
//
