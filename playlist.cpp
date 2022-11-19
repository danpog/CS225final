#include "playlist.h"
using namespace std;

vector<Song>& Playlist::GetSongs() {
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

ostream& operator<<(ostream &out, const Playlist &p)
{
    out << "Name of Playlist: " << p._name << endl;
    out << "Song count: " << p.SongCount() << endl;
    for (Song s: p._songs) {
        out << s.toString() << endl;
    }
    out << "---------------------" << endl;
    return out;
}
//
