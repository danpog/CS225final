#pragma once
#include <string>
using namespace std;

class Song  {
    public:
        Song() = default;
        Song(std::string name, std::string album, std::string artist);
        std::string toString();

    private:
        string _name;
        string _album;
        string _artist;
};