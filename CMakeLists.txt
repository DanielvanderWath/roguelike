#2.8 is what i have; I'm really doing nothing special so it'll probably work with older versions
cmake_minimum_required(VERSION 2.8)

project(roguelike)

add_executable(	roguelike 

		armour.cpp
		buff.cpp
		character.cpp
		display.cpp
		effect.cpp
		floor.cpp
		floortile.cpp
		hand.cpp
		item.cpp
		race.cpp
		resistance.cpp
		shield.cpp
		thing.cpp
		weapon.cpp

		armour.h
		buff.h
		character.h
		display.h
		effect.h
		floor.h
		game.h
		hand.h
		item.h
		misc.h
		race.h
		resistance.h
		shield.h
		thing.h
		weapon.h

		main.cpp)

target_link_libraries(	roguelike
			ncurses
			)

