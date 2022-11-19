#pragma once
#include <vector>
#include <iostream>
#include "song.h"
using namespace std;
class Playlist  {
    public:
        Playlist() = default;
        Playlist(std::string name): _name(name){};
        Playlist(vector<Song>& songs) : _songs(songs){};
        vector<Song>& GetSongs();
        void AddSong(Song& song);
        int SongCount() const;
        int GetID() const;
        void SetID(int id);
        friend ostream & operator <<(ostream &out, const Playlist& p);
    private:
        string _name;
        vector<Song> _songs;
        int _id = -1;
};

