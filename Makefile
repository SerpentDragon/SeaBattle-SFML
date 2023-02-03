all:
	# g++ -c *.cpp
	g++ -c Interface.cpp Field.cpp
	g++ *.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
	./sfml-app
