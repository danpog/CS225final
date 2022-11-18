#pragma once
#include <vector>
#include "song.h"
using namespace std;
class Playlist  {
    public:
        Playlist() = default;
        Playlist(vector<Song>& songs) : _songs(songs){};
        vector<Song>& GetSongs();
        void AddSong(Song& song);
        int SongCount() const;
        int GetID() const;
        void SetID(int id);
    private:
        vector<Song> _songs;
        int _id = -1;
};