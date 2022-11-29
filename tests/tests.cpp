#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif

#include <catch/catch.hpp>
#include "../node.h"
#include "../playlist.h"
#include "../song.h"

TEST_CASE("Does nothing") { 
    Song s = Song("a", "b", "c", "d");
    Song t = Song("a", "b", "c", "e");
    Song u = Song("a", "b", "c", "e");
    
    REQUIRE(t == u); 
    REQUIRE_FALSE(s == t);
}