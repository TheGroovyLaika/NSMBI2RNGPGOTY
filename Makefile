CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter 

jeu_video: main.o events.o jump.o collision.o entities.o player_state.o 
	g++ $^ -o $@ -lIrrlicht

main.o events.o jump.o collision.o entities.o player_state.o: events.h jump.h collision.h entities.h player_state.h
clean:
	@rm -f jeu_video *.o
