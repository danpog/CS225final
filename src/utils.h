#pragma once

#include <vector>
#include <string>
#include "playlist.h"

using namespace std;

vector<Playlist> ParseJSON(string filename);
vector<Playlist> ParseCSV(string filename);
int SplitString(const std::string & str1, char sep, std::vector<std::string> &fields); //thanks to CS 225 :)
int LevenshteinDistance(string a, string b);
void clear2D(int** arr, int rows);
int min3(int x, int y, int z);