#include "node.h"
#include <string>
#include <vector>
#include <string>

using namespace std;

Node::Node(string artist):  _artist(artist) {}
Node::Node(string artist, vector<Song>& songs): _artist(artist), _popular_songs(songs)  {}
Song& Node::RequestSong()   {
    if ((int)_position >= SongCount())   {
        //Can modify this so that we know if we've looped back 
        //(yes I know I could use % but this just allows for a print statement or something more easily)
        _position = 0;
    }
    Song& to_return = _popular_songs[_position];
    _position++;
    return to_return;
}
void Node::AddSong(Song& song)    {
    _popular_songs.push_back(song);
}
void Node::AddNeighbor(Node* node)   {
    bool found = false;
    for (std::pair<Node*, double>& p : _neighbors)  {
        if (p.first == node)    {
            p.second++;
            found = true;
            break;
        }
    }
    if (!found) {
        _neighbors.push_back(std::pair<Node*, double>(node, 1));
    }
}
double Node::GetWeight(Node* node)  {
    for (std::pair<Node*, double>& p : _neighbors)  {
        if (p.first == node)    {
            return p.second;
        }
    }
    return 0;
}
double Node::GetWeight(string artist)   {
     for (std::pair<Node*, double>& p : _neighbors)  {
        if (p.first -> GetArtist() == artist)    {
            return p.second;
        }
    }
    return 0;
}