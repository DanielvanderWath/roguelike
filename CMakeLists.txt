#2.8 is what i have; I'm really doing nothing special so it'll probably work with older versions
cmake_minimum_required(VERSION 2.8)

project(roguelike)

add_executable(	roguelike 

		armour.cpp
		character.cpp
		race.cpp
		weapon.cpp
		hand.cpp
		item.cpp
		resistance.cpp
		shield.cpp
		buff.cpp
		effect.cpp
		floor.cpp
		floortile.cpp
		display.cpp

		armour.h
		buff.h
		character.cpp
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
		shield.cpp
		shield.h
		weapon.h

		main.cpp)

target_link_libraries(	roguelike
			ncurses
			)

