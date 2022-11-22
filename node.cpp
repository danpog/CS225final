#include "node.h"
#include <string>
#include <vector>
#include <string>
#include <map>
#include <iostream>

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
void Node::AddNeighbor(Node* node)   {
    _neighbors.insert(std::pair<Node*, double>(node, 0)).first -> second++;
    node->GetNeighbors().insert(std::pair<Node*, double>(this, 0)).first -> second++;
} //

double Node::GetWeight(Node* node)  {
    unordered_map<Node*, double>::iterator it;
    for (it = _neighbors.begin(); it != _neighbors.end(); it++) {
        if (it->first == node) {
            return it->second;
        }
    }
    return 0;
}
double Node::GetWeight(string artist)   {
    unordered_map<Node*, double>::iterator it;
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
