#include "OneMillionBeers.hpp"

#include <functional>

OneMillionBeers::OneMillionBeers() {

}

OneMillionBeers::~OneMillionBeers() {

}

void OneMillionBeers::start() {
    gameThread = std::thread(std::bind(&OneMillionBeers::_gameThread, this));
}

void OneMillionBeers::join() {
    gameThread.join();
}