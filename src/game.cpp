#include "game.hpp"

int main(int argc, char** argv) {
    Game::instance(GET)->init(argc, argv);
    Game::instance(GET)->start();

    return 0;
}

/********** Game CLASS MEMBER FUNCTION DEFINITIONS **********/

Game* Game::game = NULL;

Game::Game() {

}

Game::~Game() {
    if (game) {
        delete game;
    }
    // maybe kill display as well?
    if (Game::game) {
        delete Game::game;
    }
    if (Display::display) {
        delete Display::display;
    }
}

/****************************** Game - Display ***************************/

// static instance accessor for Game
Game* Game::instance(SingletonActionType type) {
    switch (type)
    {
        case GET:
            if (game == NULL) {
                game = new Game();
            }
            break;
        case DESTROY:
            delete game;
            game = NULL;
            break;
        case EXISTS:
            break;
        default:
            fprintf(stderr, "ERROR: invalid SingletonActionType %d\n", type);
            exit(1);
    }
    return game;
}

void Game::initOpenGLDisplay(
    ViewType type, 
    int argc, 
    char** argv)
{
    // check for display existance (make sure it doesn't exist)
    if(Display::display != NULL) {
        printf("ERROR Game::initOpenGLDisplay display already exists\n");
    }
    Display::display = new Display();
    // init depending on type
    switch(type)
    {
        case DEFAULT:
            // FIX HERE
            printf("NOT READY FOR THIS YET\n");
            exit(1);
            break;
        case MAP3D:
            Display::display->view = new Map3DView();
            break;
    }
    Display::display->setup(argc, argv);
}

void Game::setupOpenGLDisplay() {
    // setup gl functions
    glutDisplayFunc(Display::displayCallback);
    glutReshapeFunc(Display::reshapeCallback);
    glutMouseFunc(Display::mousePressedCallback);
    glutMotionFunc(Display::mouseMovedCallback);
    glutKeyboardFunc(Display::keyPressedCallback);
    glutKeyboardUpFunc(Display::keyReleasedCallback);
    glutSpecialFunc(Display::specKeyPressedCallback);
    glutSpecialUpFunc(Display::specKeyReleasedCallback);
    if (Display::display->dynamic) {
        Display::display->timer.resetClock();
        glutTimerFunc(
            1000 / Display::desired_fps,
            Display::timerCallback,
            Display::display->timer.checkClock(MICRO));
    }
}

void Game::startOpenGLDisplay() {
    glutMainLoop();
    // use running flag
}

void Game::refreshOpenGLDisplay() {
}

void Game::stopOpenGLDisplay() {

}

void Game::killOpenGLDisplay() {

}

/******************************* Game - Game *****************************/

void Game::initWorld() {
    // gen models for terrainBlock and core
    Model::createModel(terrain_block_model_id);
    Model::createModel(core_model_id);

    // init terrain
    World::world = new World(loadTerrain());

    // init test entity
    Core* test_entity = new Core();
    World::world->addEntity(test_entity);

    // init test group
    Group* test_group = new Group();
    test_group->addEntity(test_entity);
    World::world->addGroup(test_group);
}

/************************* Game - Game x Display *************************/

void Game::generateNewTessellations() {
    // FIX HERE
    // this->tes = genTessellationBRUTE(this->terrain);
    // renderSetupBRUTE(this->tes, &(this->vbo_vertex_id), &(this->vbo_index_id));

}

void Game::deleteObsoleteTessellations() {
    // FIX HERE
    // this->tes = genTessellationBRUTE(this->terrain);
    // renderSetupBRUTE(this->tes, &(this->vbo_vertex_id), &(this->vbo_index_id));

}

/*************************** Public Game Methods *************************/

void Game::init(int argc, char** argv) {
    this->initOpenGLDisplay(MAP3D, argc, argv);
    this->initWorld();
    Display::display->world = World::world;
    this->setupOpenGLDisplay();
}

void Game::start() {
    this->startOpenGLDisplay();
}

void Game::update(int msecs) {
    // update world
    World::world->update(msecs);

    // // if there are tessellations we don't need anymore, delete
    // this->deleteObsoleteTessellations();
    // // if there are new tessellations required generate them
    // this->generateNewTessellations();

    // update display
    Display::display->updateView(msecs);

    // redraw
    Display::display->drawFrame();
}
