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
        void AddSong(Song& song);
        int SongCount() const {return _popular_songs.size();};
        unordered_map<Node*, double>& GetNeighbors() {return _neighbors;};
        void AddNeighbor(Node* node);
        void TrimNeighbors(size_t size, bool brute);
        double GetWeight(Node* node);
        double GetWeight(string artist);
        friend bool operator<(Node& LHS, Node& RHS);
        friend bool operator==(Node& LHS, Node& RHS);
        friend bool operator!=(Node& LHS, Node& RHS);
        friend bool operator<=(Node& LHS, Node& RHS);

    private:
        string _artist;
        vector<pair<Song,int>> _popular_songs;
        size_t _position = 0;
        unordered_map<Node*, double> _neighbors;
};