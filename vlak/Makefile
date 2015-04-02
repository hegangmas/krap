#OBJS specifies which files to compile as part of the project
OBJS = main.cpp
#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main
#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib -w -lglut -LGL -o $(OBJ_NAME)
