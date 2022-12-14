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
        void SetName(string name);
        vector<Song>& GetSongs();
        Song& GetSong(int i);
        void AddSong(Song& song);
        int SongCount() const;
        int GetID() const;
        void SetID(int id);
        bool ContainsSongByName(string name);
        friend ostream & operator <<(ostream &out, const Playlist& p);
    private:
        string _name;
        vector<Song> _songs;
        int _id = -1;
};

