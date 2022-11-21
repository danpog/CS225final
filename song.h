#pragma once
#include <string>
using namespace std;

class Song  {
    public:
        Song();
        Song(std::string name, std::string album, std::string artist);
        std::string toString() const;
        std::string GetArtist() const {return _artist;};
        std::string GetName() const{return _name;};
        std::string GetAlbum() const{return _album;};
        friend bool operator==(const Song& LHS, const Song& RHS);
        friend ostream & operator <<(ostream &out, const Song& s);
        friend bool operator!=(const Song& LHS, const Song& RHS);
        size_t GetHash() const{return _hash;};

    private:
        string _name;
        string _album;
        string _artist;
        size_t _hash;
};

template<>
struct std::hash<Song&>
{
    std::size_t operator()(const Song& s) const noexcept
    {
        /*std::size_t h1 = std::hash<std::string>{}(s.GetName());
        std::size_t h2 = std::hash<std::string>{}(s.GetArtist());
        std::size_t h3 = std::hash<std::string>{}(s.GetAlbum());
        return h1 ^ h2 ^ h3; // or use boost::hash_combine*/
        return s.GetHash();
    }
};