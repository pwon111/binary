#include "pathfinder_test.hpp"

int main(int argc, char ** argv) {
    //use opengl stuff --------------------------------------
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(XRES, YRES);
    glutInitWindowPosition(800, 0);
    glutCreateWindow("Pathfinder");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutMouseFunc(mouse_pressed);
    glutMotionFunc(mouse_moved);
    
    glutKeyboardFunc(key_pressed);
    glutKeyboardUpFunc(key_up);
    glutSpecialFunc(key_special_pressed);
    glutSpecialUpFunc(key_special_up);

    glutMainLoop();

    return 0;
}

void init(void) {
    // glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left_param, right_param,
            bottom_param, top_param,
            near_param, far_param);

    glMatrixMode(GL_MODELVIEW);

    // Terrain init
    Terrain::instance()->setDim(x_dim, y_dim);
    Terrain::instance()->setMinAlt(min_alt);
    Terrain::instance()->setMaxAlt(max_alt);
}

void reshape(int width, int height) {
    YRES = (height == 0) ? 1 : height;
    XRES = (width == 0) ? 1 : width;
    glViewport(0, 0, XRES, YRES);
    glutPostRedisplay();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(-cam_orientation_angle,
              cam_orientation_axis[0], cam_orientation_axis[1], cam_orientation_axis[2]);
    glTranslatef(-cam_position[0], -cam_position[1], -cam_position[2]);

    draw_objects();
    glutSwapBuffers();
}

void draw_objects() {
    glPushMatrix();

    for(int x = 0; x < x_dim; x++)
        for(int y = 0; y < y_dim; y++) {
            draw_block(x, y);
        }

    glPopMatrix();
}

void mouse_pressed(int button, int state, int x, int y) {
    if(state == GLUT_DOWN) {
        mouse_x = x;
        mouse_y = y;
        is_pressed = true;
    } else if(state == GLUT_UP) {
        float x_coord = pixel_to_screen_coord(mouse_x, 'x');
        float y_coord = pixel_to_screen_coord(mouse_y, 'y');
        if(button == GLUT_LEFT_BUTTON) {
            if(x_coord > -x_bound * grid_scale &&
                x_coord < x_bound * grid_scale &&
                y_coord > -y_bound * grid_scale &&
                y_coord < y_bound * grid_scale)
            {
                select_coord.x = (int) roundf((x_coord + x_bound * grid_scale)
                                / (x_bound * grid_scale * 2) * y_dim - 0.5);
                select_coord.y = (int) roundf((y_coord + y_bound * grid_scale)
                                / (y_bound * grid_scale * 2) * x_dim - 0.5);
                assert(Terrain::instance()->validCoord(select_coord));
            }
        } else if(button == GLUT_RIGHT_BUTTON) {
            assert(src_dst == 's' || src_dst == 'd');
            printf("RIGHT CLICK AT: (%d, %d)\n",
                (int) roundf((x_coord + x_bound * grid_scale)
                    / (x_bound * grid_scale * 2) * y_dim - 0.5),
                (int) roundf((y_coord + y_bound * grid_scale)
                    / (y_bound * grid_scale * 2) * x_dim - 0.5));
            if(src_dst == 's') {
                if(x_coord > -x_bound * grid_scale &&
                    x_coord < x_bound * grid_scale &&
                    y_coord > -y_bound * grid_scale &&
                    y_coord < y_bound * grid_scale)
                {
                    select_src.x = (int) roundf((x_coord + x_bound * grid_scale)
                                    / (x_bound * grid_scale * 2) * y_dim - 0.5);
                    select_src.y = (int) roundf((y_coord + y_bound * grid_scale)
                                    / (y_bound * grid_scale * 2) * x_dim - 0.5);
                    assert(Terrain::instance()->validCoord(select_src));
                }
                src_dst = 'd';
            } else if(src_dst == 'd') {
                if(x_coord > -x_bound * grid_scale &&
                    x_coord < x_bound * grid_scale &&
                    y_coord > -y_bound * grid_scale &&
                    y_coord < y_bound * grid_scale)
                {
                    select_dst.x = (int) roundf((x_coord + x_bound * grid_scale)
                                    / (x_bound * grid_scale * 2) * y_dim - 0.5);
                    select_dst.y = (int) roundf((y_coord + y_bound * grid_scale)
                                    / (y_bound * grid_scale * 2) * x_dim - 0.5);
                    assert(Terrain::instance()->validCoord(select_dst));
                }
                src_dst = 's';
            }
        }
        is_pressed = false;

        glutPostRedisplay();
    }
}

void mouse_moved(int x, int y) {
    if(is_pressed)
    {
        mouse_x = x;
        mouse_y = y;
        // glutPostRedisplay();
    }
}

float deg2rad(float angle) {
    return angle * M_PI / 180.0;
}

void key_pressed(unsigned char key, int x, int y) {
    int mod = glutGetModifiers();
    switch(key)
    {
        // system commands
        case 27:    // ESC
            Terrain::destroyInstance();
            exit(0);
            break;
        // terrain commands
        // altitude
        case 'e':
            if(select_coord.x != -1 && select_coord.y != -1) {
                Terrain::instance()->incAlt(select_coord);
            }
            break;
        case 'q':
            if(select_coord.x != -1 && select_coord.y != -1) {
                Terrain::instance()->decAlt(select_coord);
            }
            break;
        // elevation direction
        case 'w':
            if(select_coord.x != -1 && select_coord.y != -1) {
                Terrain::instance()->setElevationOrient(select_coord, VERT);
                Terrain::instance()->setDelAlt(select_coord, 1);
            }
            break;
        case 's':
            // if(mod & GLUT_ACTIVE_CTRL) {
            //     printf("BLAH\n");
                // if(key == 's') {
                //     printf("Saving!\n");
                //     saveTerrain();
                // }
                // switch(key)
                // {
                //     case 's':
                //         printf("Saving!\n");
                //         saveTerrain();
                //         break;
                // }
            //} else 
            if(select_coord.x != -1 && select_coord.y != -1) {
                Terrain::instance()->setElevationOrient(select_coord, VERT);
                Terrain::instance()->setDelAlt(select_coord, -1);
            }
            break;
        case 'd':
            if(select_coord.x != -1 && select_coord.y != -1) {
                Terrain::instance()->setElevationOrient(select_coord, HORZ);
                Terrain::instance()->setDelAlt(select_coord, 1);
            }
            break;
        case 'a':
            if(select_coord.x != -1 && select_coord.y != -1) {
                Terrain::instance()->setElevationOrient(select_coord, HORZ);
                Terrain::instance()->setDelAlt(select_coord, -1);
            }
            break;
        case 'z':
            if(select_coord.x != -1 && select_coord.y != -1) {
                Terrain::instance()->decDelAlt(select_coord);
            }
            break;
        case 'x':
            if(select_coord.x != -1 && select_coord.y != -1) {
                Terrain::instance()->incDelAlt(select_coord);
            }
            break;
        case 'p':
            if(select_src.x != -1 && select_src.y != -1 &&
                select_dst.x != -1 && select_dst.y != -1)
            {
                pf.findPath(select_src, select_dst);
            }
    }
    glutPostRedisplay();
}

void key_up(unsigned char key, int x, int y) {

}

void key_special_pressed(int key, int x, int y) {
    switch(key)
    {
        case GLUT_KEY_F5:
            saveTerrain();
            break;
        case GLUT_KEY_F9:
            loadTerrain();
            glutPostRedisplay();
            break;
    }

}

void key_special_up(int key, int x, int y) {

}

/****************************** Terrain-specific ******************************/

void draw_block(int x, int y) {
    Coordinate coord(x, y);
    // variables required for drawing blocks
    int blk_alt = Terrain::instance()->getAlt(coord);
    int blk_del_alt = Terrain::instance()->getDelAlt(coord);
    RGBf blk_alt_rgb = alt_to_RGBf(blk_alt);
    RGBf blk_del_alt_rgb = alt_to_RGBf(blk_alt + blk_del_alt);
    Orientation elev_orient = Terrain::instance()->getElevationOrient(coord);

    // draw outline
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(3);
    if(x == select_coord.x && y == select_coord.y) {
        // do nothing
    } else if(x == select_src.x && y == select_src.y) {
        glColor3f(0.0, 1.0, 0.0);
    } else if(x == select_dst.x && y == select_dst.y) {
        glColor3f(0.0, 0.0, 1.0);
    } else {
        glLineWidth(1);
    }
    glBegin(GL_LINE_LOOP);
    glVertex3f(
        grid_coord_to_gl_coord(x + 0.0, 'x'),
        grid_coord_to_gl_coord(y + 0.0, 'y'),
        0.0);
    glVertex3f(
        grid_coord_to_gl_coord(x + 1.0, 'x'),
        grid_coord_to_gl_coord(y + 0.0, 'y'),
        0.0);
    glVertex3f(
        grid_coord_to_gl_coord(x + 1.0, 'x'),
        grid_coord_to_gl_coord(y + 1.0, 'y'),
        0.0);
    glVertex3f(
        grid_coord_to_gl_coord(x + 0.0, 'x'),
        grid_coord_to_gl_coord(y + 1.0, 'y'),
        0.0);
    glEnd();
    // if del_altitude is not 0, draw ramp outline
    if(elev_orient != NONE) {
        assert(blk_del_alt != 0);
        switch(elev_orient)
        {
            case VERT:
                glBegin(GL_LINE_LOOP);
                if(blk_del_alt > 0) {
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.2, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.8, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                } else if(blk_del_alt < 0) {
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.8, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.2, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                } else {
                    fprintf(stderr, "Error: del_alt is 0 %d\n", elev_orient);
                    exit(1);
                }
                glEnd();
                break;
            case HORZ:
                glBegin(GL_LINE_LOOP);
                if(blk_del_alt > 0) {
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.2, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.8, 'y'),
                        0.0);
                } else if(blk_del_alt < 0) {
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.2, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.8, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                } else {
                    fprintf(stderr, "Error: del_alt is 0 %d\n", elev_orient);
                    exit(1);
                }
                glEnd();
                break;
            default:
                fprintf(stderr, "Error: invalid orientation %d\n", elev_orient);
                exit(1);
        }
    }

    /**************************************************************************/

    // draw path
    vector<Coordinate>::iterator path_it;
    path_it = find(pf.getFoundPath().begin(), pf.getFoundPath().end(), Coordinate(x, y));
    if(path_it != pf.getFoundPath().end()) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(
            grid_coord_to_gl_coord(x + 0.3, 'x'),
            grid_coord_to_gl_coord(y + 0.3, 'y'),
            0.0);
        glVertex3f(
            grid_coord_to_gl_coord(x + 0.7, 'x'),
            grid_coord_to_gl_coord(y + 0.3, 'y'),
            0.0);
        glVertex3f(
            grid_coord_to_gl_coord(x + 0.7, 'x'),
            grid_coord_to_gl_coord(y + 0.7, 'y'),
            0.0);
        glVertex3f(
            grid_coord_to_gl_coord(x + 0.3, 'x'),
            grid_coord_to_gl_coord(y + 0.7, 'y'),
            0.0);
        glEnd();
    }

    // fill color
    glColor3f(blk_alt_rgb.r, blk_alt_rgb.g, blk_alt_rgb.b);
    glBegin(GL_QUADS);
    glVertex3f(
        grid_coord_to_gl_coord(x + 0.0, 'x'),
        grid_coord_to_gl_coord(y + 0.0, 'y'),
        -1.0);
    glVertex3f(
        grid_coord_to_gl_coord(x + 1.0, 'x'),
        grid_coord_to_gl_coord(y + 0.0, 'y'),
        -1.0);
    glVertex3f(
        grid_coord_to_gl_coord(x + 1.0, 'x'),
        grid_coord_to_gl_coord(y + 1.0, 'y'),
        -1.0);
    glVertex3f(
        grid_coord_to_gl_coord(x + 0.0, 'x'),
        grid_coord_to_gl_coord(y + 1.0, 'y'),
        -1.0);
    glEnd();
    // if del_altitude is not 0, draw ramp outline
    if(elev_orient != NONE) {
        assert(blk_del_alt != 0);
        glColor3f(blk_del_alt_rgb.r, blk_del_alt_rgb.g, blk_del_alt_rgb.b);
        switch(elev_orient)
        {
            case VERT:
                glBegin(GL_QUADS);
                if(blk_del_alt > 0) {
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.2, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.8, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                } else if(blk_del_alt < 0) {
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.8, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.2, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                } else {
                    fprintf(stderr, "Error: del_alt is 0 %d\n", elev_orient);
                    exit(1);
                }
                glEnd();
                break;
            case HORZ:
                glBegin(GL_QUADS);
                if(blk_del_alt > 0) {
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.2, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.8, 'y'),
                        0.0);
                } else if(blk_del_alt < 0) {
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.0, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.2, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 1.0, 'x'),
                        grid_coord_to_gl_coord(y + 0.8, 'y'),
                        0.0);
                    glVertex3f(
                        grid_coord_to_gl_coord(x + 0.0, 'x'),
                        grid_coord_to_gl_coord(y + 1.0, 'y'),
                        0.0);
                } else {
                    fprintf(stderr, "Error: del_alt is 0 %d\n", elev_orient);
                    exit(1);
                }
                glEnd();
                break;
            default:
                fprintf(stderr, "Error: invalid direction %d\n", elev_orient);
                exit(1);
        }
    }
}

float pixel_to_screen_coord(int pixel_coord, char type) {
    assert(type == 'x' || type == 'y');

    if(type == 'x') {
        return pixel_coord / (float) XRES * 2 * x_bound - x_bound;
    } else {
        return (1 - pixel_coord / (float) YRES) * 2 * y_bound - y_bound;
    }
}

RGBf alt_to_RGBf(int alt) {
    RGBf ans = {};
    if(alt < 0) {
        assert(Terrain::instance()->getMinAlt() <= alt);
        ans.b = alt / (float) Terrain::instance()->getMinAlt();
    } else {
        assert(Terrain::instance()->getMaxAlt() >= alt);
        ans.r = alt / (float) Terrain::instance()->getMaxAlt();
    }
    // if(alt != 0)
    //     printf("alt: %d | rgb: %f, %f, %f\n", alt, ans.r, ans.g, ans.b);
    return ans;
}

float grid_coord_to_gl_coord(const float coord, const char type) {
    assert(type == 'x' || type == 'y');
    if(type == 'x')
        return -x_bound * grid_scale + coord * (2 * x_bound * grid_scale) / x_dim;
    else if(type == 'y')
        return -y_bound * grid_scale + coord * (2 * y_bound * grid_scale) / y_dim;
}