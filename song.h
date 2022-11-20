#pragma once
#include <string>
using namespace std;

class Song  {
    public:
        Song() = default;
        Song(std::string name, std::string album, std::string artist);
        std::string toString() const;
        std::string GetArtist() const;
        friend bool operator==(const Song& LHS, const Song& RHS);
        friend ostream & operator <<(ostream &out, const Song& s);
        friend bool operator!=(const Song& LHS, const Song& RHS);

    private:
        string _name;
        string _album;
        string _artist;
};