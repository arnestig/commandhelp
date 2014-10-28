/**
    Copyright (C) 2014 commandhelp

    Written by Tobias Eliasson <arnestig@gmail.com>.

    This file is part of commandhelp <https://github.com/arnestig/commandhelp>.

    commandhelp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    commandhelp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with commandhelp.  If not, see <http://www.gnu.org/licenses/>.
**/

#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "window.h"
#include "resources.h"
#include <string.h>
#include <ncursesw/curses.h>

Window::Window()
	:	selectedPosition( 0 ),
		searchText( "" )
{
	initscr();
	keypad(stdscr, true);
	noecho();
	start_color();
}

Window::~Window()
{
	clear();
	refresh();
	doupdate();
	endwin();
}

void Window::runCommand()
{
	if ( curCommand != NULL ) {
		std::string commandName = curCommand->getName();
		for( int i = 0; i < commandName.length(); i++ ) {
			ioctl(0,TIOCSTI, (char*)commandName.c_str()+i);
		}
		kill(getpid(), SIGINT);
	}
}

std::string Window::getSearchText()
{
	return searchText;
}

void Window::loadCommands()
{
	commands.clear();
	commands = Resources::Instance()->getCommandDatabase()->getCommands();
	if ( commands.empty() == false ) {
		curCommand = commands[ 0 ];
	}
}

void Window::appendSearchText( char *add )
{
	searchText.append( add );
}

void Window::popSearchText()
{
	if ( searchText.length() > 0 ) {
		searchText.erase( searchText.end() - 1 );
	}
}

void Window::handleInput( int c )
{
	switch ( c ) {
		case KEY_DOWN:
			if ( selectedPosition < 10 ) {
				selectedPosition++;
				if ( commands.size() > selectedPosition ) {
					curCommand = commands.at( selectedPosition );
				}
			}
		break;
		case KEY_ENTER:
		case K_ENTER:
			runCommand();
		break;
		case KEY_UP:
			if ( selectedPosition > 0 ) {
				selectedPosition--;
				if ( commands.size() > selectedPosition ) {
					curCommand = commands.at( selectedPosition );
				}
			}
		break;
		case KEY_BACKSPACE:
		case K_BACKSPACE:
			popSearchText();
			loadCommands();
		break;
		default:
			if ( c > 31 && c < 127 ) {
				appendSearchText( (char*)(&c) );			
				loadCommands();
			}
		break;
	}
}

void Window::draw()
{
	// draw help
	mvprintw( Y_OFFSET_HELP, 0, "Help: %s", "Some help" );
	clrtoeol();

	// draw commands
	init_pair(1,COLOR_BLACK, COLOR_YELLOW);
	unsigned int commandIndex = 0;
	std::vector< std::string > commandNames = Resources::Instance()->getCommandDatabase()->getCommandNames();
	for( std::vector< std::string >::iterator it = commandNames.begin(); it != commandNames.end(); ++it ) {
		// draw background if this is our selected command
		if ( commandIndex == selectedPosition ) {
			attron(COLOR_PAIR(1));
		} 

		mvprintw( Y_OFFSET_COMMANDS + commandIndex++, 0, "%s",(*it).c_str() );
		attroff(COLOR_PAIR(1));
	}
	
	// draw search box
	mvprintw( Y_OFFSET_SEARCH, 0, "Search: %s", getSearchText().c_str() );
	clrtoeol();


	int c = wgetch(stdscr);
	handleInput( c );
	refresh();
}

