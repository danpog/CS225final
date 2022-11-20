#pragma once
#include <string>
#include <set>
#include <vector>
#include "song.h"
using namespace std;

class Node  {
    public:
        Node() = default;
        Node(string artist);
        Node(string artist, vector<Song>& songs);
        string GetArtist() const {return _artist;};
        Song& RequestSong();
        void AddSong(Song& song);
        int SongCount() const {return _neighbors.size();};
        vector<std::pair<Node*, double>>& GetNeighbors() {return _neighbors;};
        void AddNeighbor(Node* node);
        double GetWeight(Node* node);
        double GetWeight(string artist);
        friend bool operator<(Node& LHS, Node& RHS);
        friend bool operator==(Node& LHS, Node& RHS);
        friend bool operator!=(Node& LHS, Node& RHS);
        friend bool operator<=(Node& LHS, Node& RHS);

    private:
        string _artist;
        vector<Song> _popular_songs;
        size_t _position = 0;
        vector<std::pair<Node*, double>> _neighbors;
        int AddNeighborRecursive(Node* node, int min, int max);
};