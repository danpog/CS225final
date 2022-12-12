#include "utils.h"
#include <fstream>
using namespace std;

/**
* Turn a (properly formatted) CSV into a vector of Playlists. This does not put the Playlists
* into a graph.
*/
vector<Playlist> ParseCSV(string filename)  {
    if (filename.empty()) {
        throw std::invalid_argument("No file name");
    }

    vector<Playlist> all_playlists = vector<Playlist>();
    std::ifstream playlist_file(filename);
    string line;
    std::getline(playlist_file,line);
    while (playlist_file.good())    {
        if (line.substr(0,9) == "Playlist:") {
            vector<string> parts;
            int count = SplitString(line, ',', parts);
            if (count > 3) {
                string s = parts[1];
                for (int i = 2; i < count - 1; i++) {
                    s += parts[i];
                }
                parts[1] = s;
                parts[2] = parts[count - 1];
            }
            Playlist p(parts[1]);
            p.SetID(std::stoi(parts[2]));
            if (std::stoi(parts[2]) % 1000 == 0)    {
                cout << "Parsing playlist " << parts[2]<< endl;
            }
            if (!playlist_file.good())  {
                break;
            }
            std::getline(playlist_file,line);
            while (line.substr(0,9) != "Playlist:") {
                vector<string> song;
                int length = SplitString(line, ',', song);
                if (length  == 0)   {
                    break;
                }
                if (length != 4)    {
                    int temp = 0;
                    for (int i = 0; i < 4; i++){
                        string curr;
                        curr += song[temp];
                        while (song[temp].at(song[temp].length() - 1) != '\"')  {
                            temp++;
                            curr += "," + song[temp];
                        }
                        temp++;
                        song[i] = curr;
                    }
                }
                Song s = Song(song[0], song[1], song[2], song[3]);
                p.AddSong(s);
                if (!playlist_file.good())  {
                    break;
                }
                std::getline(playlist_file,line);
                if (line.size() == 0)   {
                    break;
                }
            }
            all_playlists.push_back(p);
        }
    }
    return all_playlists;
}
/**
* Split a string by any character (typically ','), this code was from CS 225.
*/
int SplitString(const std::string & str1, char sep, std::vector<std::string> &fields) {
    std::string str = str1;
    std::string::size_type pos;
    while((pos=str.find(sep)) != std::string::npos) {
        fields.push_back(str.substr(0,pos));
        str.erase(0,pos+1);  
    }
    fields.push_back(str);
    return fields.size();
}

/**
* Return the minimum of 3 integers.
*/
int min3(int x, int y, int z){
    if(x <= y && x <= z)
        return x;
    if(y <=x && y <= z)
        return y;
    return z;
}

/**
* Free memory from a 2D array of size rows.
*/
void clear2D(int** arr, int rows){
  for(int i = 0; i < rows; i++){
    delete[] arr[i];
  }
  
  delete[] arr;
}
/**
* Find the similarity of two strings. This algorithm was very much found on the 
* internet and slightly adapted for our purposes.
*/
int LevenshteinDistance(string a, string b){
  int len_a = a.size();
  int len_b = b.size();
  int** matrix = new int*[len_a + 1];

  for(int i = 0; i < len_a + 1; i++)    {
    matrix[i] = new int[len_b + 1];
  }

  for(int i = 0; i < len_a + 1; i++)    {
    matrix[i][0] = i;
  }

  for(int j = 0; j < len_b + 1; j++)    {
    matrix[0][j] = j;
  }
    
  int ins;
  int del;
  int rep;

  for(int i = 1; i < len_a + 1; i++){
    for(int j = 1; j < len_b + 1; j++){
      if(a[i - 1] == b[j - 1]){
        matrix[i][j] = matrix[i - 1][j - 1];
      }
      else{
        ins = matrix[i][j - 1];
        del = matrix[i - 1][j];
        rep = matrix[i - 1][j - 1];
        matrix[i][j] = 1 + min3(ins, del, rep);
      }
    }
  }
  int to_return = matrix[len_a][len_b];
  clear2D(matrix, len_a + 1);
  return to_return;
}