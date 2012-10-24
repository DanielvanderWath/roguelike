.PHONY: roguelike

all: roguelike

SRC = 	main.cpp \
	character.cpp \
	race.cpp \
	item.cpp \
	weapon.cpp \
	hand.cpp \
	armour.cpp

LIB = 

INC = 

roguelike:
	g++ -o roguelike -g $(SRC) $(LIB) $(INC)
