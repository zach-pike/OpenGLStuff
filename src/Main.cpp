#include "MyApp/MyApp.hpp"
#include "OneMillionBeers/OneMillionBeers.hpp"

int main() {
    auto game = std::make_unique<OneMillionBeers>();
    game->start();
    game->join();

    return 0;
}