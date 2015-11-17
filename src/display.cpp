#include "display.hpp"

/******************* CAMERA STRUCT MEMBER FUNCTION DEFINITIONS ****************/

const float Camera::screenWidth() const {
    return this->right_param - this->left_param;
}

const float Camera::screenHeight() const {
    return this->top_param - this->bottom_param;
}

const Vector3f Camera::rotateToAxisVec() const {
    Vector3f default_axis;
    default_axis << 0.0, 0.0, -1.0;
    Vector3f rotate_to_axis = default_axis.cross(this->axis);
    float sin_theta = rotate_to_axis.norm();
    float cos_theta = sqrt(default_axis.dot(this->axis));
    if (sin_theta < 0.001) {
        return cos_theta > 0 ?
            Vector3f(0.0, 0.0, 0.0)     // don't rotate
            : Vector3f(0.0, M_PI / 2, 0.0);  // change direction
    }
    float theta = cos_theta > 0 ? asin(sin_theta) : M_PI - asin(sin_theta);
    return rotate_to_axis / sin_theta * theta;
}

// function for syncing mouse's pixel coordinate and screen coordinate
void syncMouseCoords(Mouse& mouse, const Camera& cam) {
    mouse.screen_x = mouse.pixel_x / (float) cam.x_res * cam.screenWidth()
        + cam.left_param;
    mouse.screen_y = mouse.pixel_y / (float) cam.y_res * cam.screenHeight()
        + cam.bottom_param;
}

/********************************** View Class ********************************/

View::View() : cam(default_cam), mouse() {

}

View::~View() {

}

// opengl functions (VIRTUAL DEFAULT CALLBACK FUNC)
void View::setupFunc(int argc, char** argv) {
    this->defaultSetupFunc(argc, argv);
}
void View::reshapeFunc(int width, int height) {
    this->defaultReshapeFunc(width, height);
}
void View::displayFunc(World* world) {
    this->defaultDisplayFunc(world);
}
void View::mousePressedFunc(int button, int state, int x, int y) {
    this->defaultMousePressedFunc(button, state, x, y);
}
void View::mouseMovedFunc(int x, int y) {
    this->defaultMouseMovedFunc(x, y);
}
void View::keyPressedFunc(unsigned char key, int x, int y) {
    this->defaultKeyPressedFunc(key, x, y);
}
void View::keyReleasedFunc(unsigned char key, int x, int y) {
    this->defaultKeyReleasedFunc(key, x, y);
}
void View::specKeyPressedFunc(int key, int x, int y) {
    this->defaultSpecKeyPressedFunc(key, x, y);
}
void View::specKeyReleasedFunc(int key, int x, int y) {
    this->defaultSpecKeyReleasedFunc(key, x, y);
}

// default implementations for opengl functions
void View::defaultSetupFunc(int argc, char** argv) {
    // create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(this->cam.x_res, this->cam.y_res);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Default View");
    // init glew
    glewInit();
}

void View::defaultReshapeFunc(int width, int height) {
    this->cam.x_res = (width == 0) ? 1 : width;
    this->cam.y_res = (height == 0) ? 1 : height;
    glViewport(0, 0, this->cam.x_res, this->cam.y_res);    // safe?
}

void View::defaultDisplayFunc(World* world) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void View::defaultMousePressedFunc(int button, int state, int x, int y) {
    if(state == GLUT_DOWN) {
        this->mouse.pixel_x = x;
        this->mouse.pixel_y = y;
        syncMouseCoords(this->mouse, this->cam);
        this->mouse.is_pressed = true;
    } else if(state == GLUT_UP) {
        if(button == GLUT_LEFT_BUTTON) {
            printf("LEFT CLICK AT: p(%d, %d)\t|\ts(%f, %f)\n",
                this->mouse.pixel_x, this->mouse.pixel_y,
                this->mouse.screen_x, this->mouse.screen_y);
        } else if(button == GLUT_RIGHT_BUTTON) {
            printf("RIGHT CLICK AT: p(%d, %d)\t|\ts(%f, %f)\n",
                this->mouse.pixel_x, this->mouse.pixel_y,
                this->mouse.screen_x, this->mouse.screen_y);
        }
        this->mouse.is_pressed = false;
    }
}

void View::defaultMouseMovedFunc(int x, int y) {
    if(this->mouse.is_pressed)
    {
        this->mouse.pixel_x = x;
        this->mouse.pixel_y = y;
        syncMouseCoords(this->mouse, this->cam);
    }
}

void View::defaultKeyPressedFunc(unsigned char key, int x, int y) {
    int mod = glutGetModifiers();   // still not sure how it works
    switch(key)
    {
        // system commands
        case 27:    // ESC
            exit(0);
            break;
        default:
            printf("KEY PRESS: key = %c, x = %d, y = %d\n", key, x, y);
    }
}

void View::defaultKeyReleasedFunc(unsigned char key, int x, int y) {
    switch(key)
    {
        default:
            printf("KEY RELEASE: key = %c, x = %d, y = %d\n", key, x, y);
    }
}

void View::defaultSpecKeyPressedFunc(int key, int x, int y) {
    switch(key)
    {
        default:
            printf("SPEC KEY PRESS: key = %d, x = %d, y = %d\n", key, x, y);
    }
}

void View::defaultSpecKeyReleasedFunc(int key, int x, int y) {
    switch(key)
    {
        default:
            printf("SPEC KEY RELEASE: key = %d, x = %d, y = %d\n", key, x, y);
    }
}


/******************************* Map3DView Class ******************************/

Map3DView::Map3DView() :
    View(),
    focus(init_focus),
    prev_focus(init_focus),
    focus_cam_match(false)
{
    updateCamPosition();
}

Map3DView::~Map3DView() {
    //
}

// opengl functions (VIRTUAL DEFAULT CALLBACK FUNC)
void Map3DView::setupFunc(int argc, char** argv) {
    // create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(this->cam.x_res, this->cam.y_res);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Map3DView");
    glewInit();
    // setup space
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_NORMALIZE);
    // setup camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(this->cam.left_param, this->cam.right_param,
            this->cam.bottom_param, this->cam.top_param,
            this->cam.near_param, this->cam.far_param);
    // switch back to model view matrix mode to be safe
    glMatrixMode(GL_MODELVIEW);
}
void Map3DView::displayFunc(World* world) {
    if (world) {
        glPushMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        Vector3f rotate_to_axis = this->cam.rotateToAxisVec();
        float cam_r2a_angle = rotate_to_axis.norm();
        if (cam_r2a_angle > 0.0001) {
            rotate_to_axis /= cam_r2a_angle;
            glRotatef(degrees(-cam_r2a_angle),
                       rotate_to_axis[0],
                       rotate_to_axis[1],
                       rotate_to_axis[2]);
        }
        glRotatef(degrees(-this->cam.angle),
                this->cam.axis[0],
                this->cam.axis[1],
                this->cam.axis[2]);
        glTranslatef(-this->cam.position[0],
                     -this->cam.position[1],
                     -this->cam.position[2]);
        this->render(world);
        glutSwapBuffers();

        glPopMatrix();
    } else {
        fprintf(stderr,
            "ERROR Map3DView::displayFunc world has not been set yet\n");
    }
}
void Map3DView::keyPressedFunc(unsigned char key, int x, int y) {
    defaultKeyPressedFunc(key, x, y);
    switch (key) {
        case 'w':
            this->prev_focus = focus;
            this->focus += NORTH;
            this->focus_cam_match = false;
            break;
        case 'a':
            this->prev_focus = focus;
            this->focus += WEST;
            this->focus_cam_match = false;
            break;
        case 's':
            this->prev_focus = focus;
            this->focus += SOUTH;
            this->focus_cam_match = false;
            break;
        case 'd':
            this->prev_focus = focus;
            this->focus += EAST;
            this->focus_cam_match = false;
            break;
    }
}

void Map3DView::updateCamPosition() {
    // FIX HERE
    int x_dim = 21; // this->terrain->getXDim();
    int y_dim = 21; // this->terrain->getYDim();
    this->cam.position[0] = focus.x - x_dim / 2 + 0.5 * (1 - x_dim % 2);
    this->cam.position[1] = focus.y - y_dim / 2 + 0.5 * (1 - y_dim % 2);
    this->cam.position[2] = 10;
}

void Map3DView::updateCamAxis() {

}

void Map3DView::update(int msecs) {
    // if camera position does not match focus, update camera position
    if (!this->focus_cam_match) {
        updateCamPosition();
        updateCamAxis();
        this->focus_cam_match = true;
    }
}

// render all of the tessellations
void Map3DView::render(World* world) {
    // render terrain
    const Terrain* terrain = world->getTerrain();
    int x_dim = terrain->getXDim();
    int y_dim = terrain->getYDim();

    for (int x = 0; x < x_dim; x++) {
        for (int y = 0; y < y_dim; y++) {
            glPushMatrix();

            glTranslatef(
                -x_dim / 2 - (x_dim % 2 - 1) * 0.5 + x,
                -y_dim / 2 - (y_dim % 2 - 1) * 0.5 + y,
                (terrain->getAlt(Coordinate(x, y))
                    + terrain->getEndAlt(Coordinate(x, y))) / 2.0);
            renderBRUTE(Model::getModel(terrain_block_model_id));

            glPopMatrix();
        }
    }

    // render entities
    const vector<Entity*>& entities = world->getEntities();

    for (int i = 0; i < entities.size(); i++) {
        glPushMatrix();

        const Coordinate& coord = entities[i]->getCurrCoord();
        glTranslatef(
            -x_dim / 2 - (x_dim % 2 - 1) * 0.5 + coord.x,
            -y_dim / 2 - (y_dim % 2 - 1) * 0.5 + coord.y,
            (terrain->getAlt(Coordinate(coord.x, coord.y))
                + terrain->getEndAlt(Coordinate(coord.x, coord.y))) / 2.0);

        renderBRUTE(Model::getModel(core_model_id));

        glPopMatrix();
    }

    // glPushMatrix();
    // // FIX HERE
    // // renderWireBRUTE(this->tes, this->vbo_vertex_id, this->vbo_index_id);
    // // renderBRUTE(this->tes, this->vbo_vertex_id, this->vbo_index_id);
    // for (int i = 0; i < this->tessellations.size(); i++) {
    //     // renderWireBRUTE(this->tessellations.at(i));
    //     renderBRUTE(this->tessellations.at(i));
    // }
    // glPopMatrix();
}

/******************* DISPLAY CLASS MEMBER FUNCTION DEFINITIONS ****************/

Display* Display::display = NULL;

Display::Display() : view(NULL), world(NULL), dynamic(true), timer() {

}

Display::~Display() {
    // coordinate with Game
}

void Display::setView(View* view) {
    this->view = view;
}
void Display::updateView(int msecs) {
    assert(display);
    assert(display->view);
    this->view->update(msecs);
}

void Display::setWorld(World* world) {
    this->world = world;
}

void Display::setup(int argc, char** argv) {
    assert(display);
    assert(display->view);
    display->view->setupFunc(argc, argv);
}

void Display::drawFrame() {
    glutPostRedisplay();
}

// opengl functions (STATIC CALLBACK)
void Display::reshapeCallback(int width, int height) {
    assert(display);
    assert(display->view);
    display->view->reshapeFunc(width, height);
}
void Display::displayCallback() {
    assert(display);
    assert(display->view);
    display->view->displayFunc(display->world);
}
void Display::mousePressedCallback(int button, int state, int x, int y) {
    assert(display);
    assert(display->view);
    display->view->mousePressedFunc(button, state, x, y);
}
void Display::mouseMovedCallback(int x, int y) {
    assert(display);
    assert(display->view);
    display->view->mouseMovedFunc(x, y);
}
void Display::keyPressedCallback(unsigned char key, int x, int y) {
    assert(display);
    assert(display->view);
    display->view->keyPressedFunc(key, x, y);
}
void Display::keyReleasedCallback(unsigned char key, int x, int y) {
    assert(display);
    assert(display->view);
    display->view->keyReleasedFunc(key, x, y);
}
void Display::specKeyPressedCallback(int key, int x, int y) {
    assert(display);
    assert(display->view);
    display->view->specKeyPressedFunc(key, x, y);
}
void Display::specKeyReleasedCallback(int key, int x, int y) {
    assert(display);
    assert(display->view);
    display->view->specKeyReleasedFunc(key, x, y);
}
void Display::timerCallback(int msecs) {
    assert(display);
    // assert(display->view);
    display->defaultTimerFunc(msecs);
}

// NOTE NOTE NOTE NOTE NOTE NOTE!!!!!!!!!!!!!!!!!!!!!!
// MAY NEED TO ENABLE GL_NORMALIZE!!!!!!!!!!!!!!!!!!

// void Display::timerFunc(int msecs) {
//     this->defaultTimerFunc(msecs);
// }

void Display::defaultTimerFunc(int msecs) {
    glutTimerFunc(
        1000 / Display::desired_fps,
        Display::timerCallback,
        this->timer.checkClock(MICRO));
    printf("FPS: %f\n", 1000000.0 / (this->timer.checkClock(MICRO) - msecs));
    Game::instance(GET)->update(msecs);
}