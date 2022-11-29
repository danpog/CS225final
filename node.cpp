#include "node.h"
#include <string>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <queue>

using namespace std;

Node::Node(string artist):  _artist(artist) {}
Node::Node(string artist, vector<pair<Song , int>>& songs): _artist(artist), _popular_songs(songs)  {}
Song& Node::RequestSong()   {
    if (SongCount() == 0)   {
        throw invalid_argument("No songs to give!");
    }
    if ((int)_position >= SongCount())   {
        //Can modify this so that we know if we've looped back 
        //(yes I know I could use % but this just allows for a print statement or something more easily)
        _position = 0;
    }
    Song& to_return = _popular_songs[_position].first;
    ++_position;
    return to_return;
}

vector<pair<Song,int>> Node::GetAllSongs() {
    return _popular_songs;
}
Song Node::FindSong(string title)  {
    for (size_t j = 0; j < _popular_songs.size(); j++)    {
        pair<Song, int> i = _popular_songs[j];
        if (i.first._name == title) {
            return i.first;
        }
    }
    if (!_popular_songs.empty())    {
         return _popular_songs[0].first;
    }
    throw invalid_argument("Song not found and no songs to return!");
}

// Add song to node, updates ranking of popular songs
void Node::AddSong(Song& song)    {
    //Song s = Song(song.GetName(), song.GetAlbum(), song.GetArtist());
    for (size_t i = 0; i < _popular_songs.size(); ++i)  {
        if (_popular_songs[i].first == song)    {
            _popular_songs[i].second++;
            while (i != 0 && _popular_songs[i].second > _popular_songs[i-1].second) {
                std::pair<Song, int> temp = _popular_songs[i];
                swap(_popular_songs[i], _popular_songs[i-1]);
                i--;
            }
            return;
        }
    }
    _popular_songs.push_back(pair<Song&, int>(song, 1));
}

void Node::AddSongPair(Song& song, int frequency) {
    _popular_songs.push_back(pair<Song&, int>(song, frequency));
}

void Node::AddNeighbor(Node* node)   {
    _neighbors.insert(std::pair<Node*, int>(node, 0)).first -> second++;
    node->GetNeighbors().insert(std::pair<Node*, int>(this, 0)).first -> second++;
}

void Node::AddNeighborPair(pair<Node*, int> neighbor) {
    _neighbors.insert(std::pair<Node*, int>(neighbor.first, neighbor.second));
}

void Node::TrimNeighbors(size_t size, bool brute) {
    unordered_map<Node*, int> _new_neighbors;

    if (_neighbors.size() <= size) { return; }
    // Brute force taking top 20    
    if (brute) {       
        pair<Node*, int> max;
        for (size_t i = 0; i < size; ++i) {
            max = pair<Node*, int>(nullptr, 0.0);
            for (auto const& x : _neighbors) {
                if (x.second > max.second) {
                    max = x;
                }
            }
            _neighbors.erase(max.first);
            _new_neighbors.insert(max);
        }
        _neighbors.clear();
        _neighbors = _new_neighbors;
    } else {
        // Creating a max heap for second element of pair
        auto cmp = [](const pair<Node*, int>& lhs, const pair<Node*, int>& rhs)
        { return lhs.second < rhs.second;};
        priority_queue<pair<Node*, int>, vector<pair<Node*, int>>, decltype(cmp) > pQ(cmp);
        for(auto const& x : _neighbors) {
            pQ.push(x);
        }
        for (size_t i = 0; i < size; ++i) {
            auto tmp = pQ.top();
            //cout << tmp.first << " " << tmp.second << endl;
            _new_neighbors.insert(tmp);
            _neighbors.erase(tmp.first);
            pQ.pop();
        }
        _neighbors.clear();
        _neighbors = _new_neighbors;
    }
}

int Node::GetWeight(Node* node)  {
    unordered_map<Node*, int>::iterator it;
    if (_neighbors.count(node) == 0)    {
        return 0;
    }
    return _neighbors[node];
    /*for (it = _neighbors.begin(); it != _neighbors.end(); it++) {
        if (it->first == node) {
            return it->second;
        }
    }
    return 0;*/
}
int Node::GetWeight(string artist)   {
    unordered_map<Node*, int>::iterator it;
    for (it = _neighbors.begin(); it != _neighbors.end(); it++) {
        if (it->first -> GetArtist() == artist) {
            return it->second;
        }
    }
    return 0;
}

bool operator<(Node& LHS, Node& RHS)    {
    return LHS.GetArtist() < RHS.GetArtist();
}
bool operator==(Node& LHS, Node& RHS)   {
    return LHS.GetArtist() == RHS.GetArtist();
}
bool operator!=(Node& LHS, Node& RHS)   {
    return !(LHS == RHS);
}
bool operator<=(Node& LHS, Node& RHS)   {
    return LHS < RHS || LHS == RHS;
}
