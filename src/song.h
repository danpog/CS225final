#pragma once
#include <string>
using namespace std;

struct Song  {
    public:
        Song(string n, string al, string ar, string u): _name(n), _album(al), _artist(ar), _uri(u){};
         string _name;
         string _album;
         string _artist;
         string _uri;
         size_t _hash = std::hash<string>{}(_uri);
        friend bool operator==(const Song& LHS, const Song& RHS);
};

/*template<>
struct std::hash<Song&>
{
    std::size_t operator()(const Song& s) const noexcept
    {
        std::size_t h1 = std::hash<std::string>{}(s.GetName());
        std::size_t h2 = std::hash<std::string>{}(s.GetArtist());
        std::size_t h3 = std::hash<std::string>{}(s.GetAlbum());
        return h1 ^ h2 ^ h3; // or use boost::hash_combine
        return s._hash;
    }
};*/
