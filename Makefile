all:
	# g++ -c *.cpp
	g++ Interface.cpp settings.cpp -c
	g++ *.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
	./sfml-app
