/**
    Copyright (C) 2014-2020 commandhelp

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

Window::Window()
	:	selectedPosition( 0 ),
		searchText( "" )
{
	initscr();
	noecho();
	start_color();
}

Window::~Window()
{
	delwin( helpWindow );
	delwin( searchWindow );
	delwin( commandWindow );
	refresh();
	endwin();
}

void Window::init()
{
	loadCommands();
	int y,x;
	getmaxyx( stdscr, y, x );

	// help window
	helpWindow = newwin( 3, x/2 - 1, 1, x/2+1 );

	// search window
	searchWindow = newwin( 3, x/2 - 1, 1, 1 );
	keypad( searchWindow, true );

	// command window
	commandWindow = newwin( y-4, x - 2, 4, 1 );
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
	commands = Resources::Instance()->getCommandDatabase()->getCommands( searchText );
	if ( commands.empty() == false ) {
		curCommand = commands[ 0 ];
	} else {
		curCommand = NULL;
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
			if ( selectedPosition < commands.size() - 1 ) {
				selectedPosition++;
				if ( commands.size() > selectedPosition ) {
					curCommand = commands.at( selectedPosition );
				} else {
					curCommand = NULL;
				}
			}
		break;
		case K_CTRL_T:
			Resources::Instance()->getCommandDatabase()->addCommandInteractive();
			loadCommands();
		break;
		case K_CTRL_D:
			curCommand = Resources::Instance()->getCommandDatabase()->removeCommand( curCommand );
            commands = Resources::Instance()->getCommandDatabase()->getCommands( searchText );
            if ( selectedPosition == commands.size() && commands.size() > 0 ) {
                selectedPosition = commands.size()-1;
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
				} else {
					curCommand = NULL;
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
	wclear( searchWindow );
	wclear( helpWindow );
	wclear( commandWindow );
	// draw help
	mvwprintw( helpWindow, 1, 1, "Ctrl+T - %s", "Add new command" );

	// draw commands
	init_pair(1,COLOR_BLACK, COLOR_YELLOW);
	unsigned int commandIndex = 0;
	for( std::vector< Command* >::iterator it = commands.begin(); it != commands.end(); ++it ) {
		// draw background if this is our selected command
		if ( commandIndex == selectedPosition ) {
			wattron( commandWindow, COLOR_PAIR(1) );
		} 

		mvwprintw( commandWindow, 1 + commandIndex++, 1, "%s",(*it)->getName().c_str() );
		wattroff( commandWindow, COLOR_PAIR(1) );
	}
	
	// draw search box
	mvwprintw( searchWindow, 1, 1, "Search: %s", getSearchText().c_str() );

	box( searchWindow, 0, 0 );
	box( commandWindow, 0, 0 );
	box( helpWindow, 0, 0 );
	wnoutrefresh( commandWindow );
	wnoutrefresh( helpWindow );
	wnoutrefresh( searchWindow );
	doupdate();
	int c = wgetch(searchWindow);
	handleInput( c );
}

