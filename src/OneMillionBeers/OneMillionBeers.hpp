#pragma once
#include "App/App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>

class OneMillionBeers : public App {
private:
    std::thread gameThread;

    GLFWwindow* window;

    void _gameThread();

    void init();
    void loop();
    void cleanup();
public:
    OneMillionBeers();
    ~OneMillionBeers();

    void start();
    void join();
};