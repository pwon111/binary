#ifndef GAME_HPP
#define GAME_HPP

#include "display.hpp"
#include "world.hpp"
#include "file_manager.hpp"

// types of views possible
enum ViewType {DEFAULT, MAP3D};

// class that controls the interaction between World and Display
class Game {
private:
    bool running = false;

    static Game* game;
    explicit Game();
    ~Game();

    // World x Display
    void generateNewTessellations();
    void deleteObsoleteTessellations();

    // Display
    void initOpenGLDisplay(ViewType type, int argc, char** argv);
    void setupOpenGLDisplay();
    void startOpenGLDisplay();
    void refreshOpenGLDisplay();    // probably don't need?
    void stopOpenGLDisplay();
    void killOpenGLDisplay();       // how do i call this?

    // World
    void initWorld();

public:
    static Game* instance(SingletonActionType type);

    void init(int argc, char** argv);
    void start();
    void update(int msecs);
};

#endif