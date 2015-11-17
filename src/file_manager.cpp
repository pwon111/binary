#include "file_manager.hpp"

#define BUFFER_SIZE 256

// causes seg fault at the moment
bool fexists(string filename) {
    ifstream ifile(filename);
    return ifile;
}

void saveTerrain(const Terrain& trn) {
    if(!trn.gridExists()) {
        printf("ERROR: grid has not been defined! define grid before saving\n");
        return;
    }

    // string filename;
    // printf("Enter save filename: \n");
    // cin >> filename;
    // cout << filename << endl;

    // char read_buffer[BUFFER_SIZE];
    // printf("Enter save filename: \n");
    // cin >> read_buffer;
    // cout << read_buffer << endl;

    // if(fexists("../output/save_file.trn"))
    //     remove("../output/save_file.trn");

    // terrain dimensions (used frequently below)
    int x_dim = trn.getXDim();
    int y_dim = trn.getYDim();

    ofstream save_file;
    save_file.open("../output/save_file.trn", ios::out);

    // file title
    save_file << "TRNv0.0\n";
    // dimension
    save_file << x_dim << "\n";
    save_file << y_dim << "\n";
    // MAX/MIN altitude
    save_file << trn.getMaxAlt() << "\n";
    save_file << trn.getMinAlt() << "\n";
    // altitudes
    Coordinate coord(-1, -1);
    for(int x = 0; x < x_dim; x++)
        for(int y = 0; y < y_dim; y++) {
            coord.x = x;
            coord.y = y;
            save_file << trn.getAlt(coord) << " "
                    << trn.getElevationOrient(coord) << " "
                    << trn.getDelAlt(coord) << "\n";
        }

    save_file.close();

    printf("...terrain saved.\n");
}

Terrain* loadTerrain() {
    Terrain* trn = new Terrain();
    // read and parse
    int x_dim, y_dim;
    int max_alt, min_alt;
    char read_buffer[BUFFER_SIZE];
    ifstream save_file("../output/save_file.trn");
    if(save_file.is_open()) {
        assert(save_file.getline(read_buffer, BUFFER_SIZE));
        printf("Loading FILE_TYPE:\t%s\n", read_buffer);
        // dimension
        assert(save_file.getline(read_buffer, BUFFER_SIZE));
        x_dim = atoi(read_buffer);
        assert(save_file.getline(read_buffer, BUFFER_SIZE));
        y_dim = atoi(read_buffer);
        printf("\tXDIM:\t\t%d\n\tYDIM:\t\t%d\n", x_dim, y_dim);
        // max/min altitudes
        assert(save_file.getline(read_buffer, BUFFER_SIZE));
        max_alt = atoi(read_buffer);
        assert(save_file.getline(read_buffer, BUFFER_SIZE));
        min_alt = atoi(read_buffer);
        printf("\tMAX_ALTITUDE:\t%d\n\tMIN_ALTITUDE:\t%d\n", max_alt, min_alt);

        trn->setDim(x_dim, y_dim);
        trn->setMaxAlt(max_alt);
        trn->setMinAlt(min_alt);

        printf("\tLOADING...\t");
        cout.flush();

        char *parse_token;
        Coordinate coord(-1, -1);
        int alt;
        Orientation orient;
        int del_alt;
        for(int x = 0; x < x_dim; x++)
            for(int y = 0; y < y_dim; y++) {
                coord.x = x;
                coord.y = y;
                assert(save_file.getline(read_buffer, BUFFER_SIZE));
                // altitude
                parse_token = strtok(read_buffer, " ");
                alt = atoi(parse_token);
                // printf("alt: %d\t", alt);
                assert(alt <= max_alt);
                assert(alt >= min_alt);

                // direction
                parse_token = strtok(NULL, " ");
                orient = (Orientation) atoi(parse_token);
                // printf("orient: %d\t", orient);

                // del_altitude
                parse_token = strtok(NULL, " ");
                del_alt = atoi(parse_token);
                if(orient == NONE)
                    assert(del_alt == 0);
                // printf("del_alt: %d\n", del_alt);

                // check file integrity
                parse_token = strtok(NULL, " ");
                assert(parse_token == NULL);

                trn->setAlt(coord, alt);
                trn->setElevationOrient(coord, orient);
                trn->setDelAlt(coord, del_alt);
            }
        assert(!save_file.getline(read_buffer, BUFFER_SIZE));
        printf("DONE.\n");
        return trn;
    } else {
        printf("ERROR: couldn't open save file! check that it exists\n");
    }
}