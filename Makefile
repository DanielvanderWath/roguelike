.PHONY: roguelike

all: roguelike

TEST ?= 1 #use test mode by default for now

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
	display.cpp \
	$(MAIN)

LIB = -lncurses

INC = 

roguelike:
	g++ -o roguelike -g $(SRC) $(LIB) $(INC)
