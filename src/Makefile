CC = g++
FLAGS = -std=c++11 -g -o

INCLUDE = -I/usr/include/GL -I/usr/include -I../lib
LIBDIR = -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu
GAME_SOURCES = game.cpp game.hpp display.o renderer.o model.o world.o entity.o terrain.o file_manager.o pathfinder.o util.o timer.o
LIBS = -lGLEW -lGL -lGLU -lglut -lm -lboost_system -lboost_chrono

GAME_EXENAME = binary

all: binary

binary: $(GAME_SOURCES)
	$(CC) $(FLAGS) $(GAME_EXENAME) $(INCLUDE) $(LIBDIR) $(GAME_SOURCES) $(LIBS)

world.o: world.cpp world.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

display.o: display.cpp display.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

renderer.o: renderer.cpp renderer.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

model.o: model.cpp model.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

entity.o: entity.cpp entity.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

terrain.o: terrain.cpp terrain.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

file_manager.o: file_manager.cpp file_manager.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

pathfinder.o: pathfinder.cpp pathfinder.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

util.o: util.cpp util.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

timer.o: timer.cpp timer.hpp
	$(CC) $(FLAGS) $@ $(INCLUDE) $(LIBDIR) $(LIBS) -c $<

clean:
	rm -f *.o $(GAME_EXENAME)

.PHONY: all clean


