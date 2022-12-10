#include "song.h"
#include <iostream>
using namespace std;

bool operator==(const Song& LHS, const Song& RHS)    {
    //return hash<Song&>{}(LHS) == hash<Song&>{}(RHS);
    //return LHS._name == RHS._name && LHS._album == RHS._album && LHS._artist == RHS._artist;
    //return LHS._uri == RHS._uri;
    return LHS._hash == RHS._hash;
}
