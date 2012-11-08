.PHONY: roguelike

all: roguelike

ifeq ($(TEST), 1)
MAIN = test.cpp
else
MAIN = main.cpp
endif

SRC = 	armour.cpp \
	character.cpp \
	race.cpp \
	weapon.cpp \
	hand.cpp \
	item.cpp \
	resistance.cpp \
	shield.cpp \
	buff.cpp \
	effect.cpp \
	floor.cpp \
	floortile.cpp \
	$(MAIN)

LIB = 

INC = 

roguelike:
	g++ -o roguelike -g $(SRC) $(LIB) $(INC)
