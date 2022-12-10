#pragma once
#include <string>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include "song.h"
using namespace std;

class Node  {
    public:
        Node() = default;
        Node(string artist);
        Node(string artist, vector<pair<Song,int>>& songs);
        string GetArtist() const {return _artist;};
        Song& RequestSong();
        Song& RequestSong(int song_tier);
        vector<pair<Song,int>> GetAllSongs();
        Song FindSong(string title);
        int FindSongPlacement(string title);
        void AddSong(Song& song);
        void AddSongPair(Song& song, int frequency);
        int SongCount() const {return _popular_songs.size();};
        unordered_map<Node*, int>& GetNeighbors() {return _neighbors;};
        void AddNeighbor(Node* node);
        void AddNeighborPair(pair<Node*, int> neighbor);
        void TrimNeighbors(size_t size, bool brute);
        int GetWeight(Node* node);
        int GetWeight(string artist);
        friend bool operator<(Node& LHS, Node& RHS);
        friend bool operator==(Node& LHS, Node& RHS);
        friend bool operator!=(Node& LHS, Node& RHS);
        friend bool operator<=(Node& LHS, Node& RHS);

    private:
        string _artist;
        vector<pair<Song,int>> _popular_songs;
        int _position = -1;
        unordered_map<Node*, int> _neighbors;
};