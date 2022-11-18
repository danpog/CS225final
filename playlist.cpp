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