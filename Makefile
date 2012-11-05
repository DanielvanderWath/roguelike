.PHONY: roguelike

all: roguelike

SRC = 	armour.cpp \
	character.cpp \
	race.cpp \
	weapon.cpp \
	hand.cpp \
	item.cpp \
	main.cpp

LIB = 

INC = 

roguelike:
	g++ -o roguelike -g $(SRC) $(LIB) $(INC)
