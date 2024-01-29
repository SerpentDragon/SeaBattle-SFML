all:
	# g++ -c *.cpp
	g++ Mechanics.cpp -c
	g++ *.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
	./sfml-app
