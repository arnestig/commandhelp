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

#include "commanddatabase.h"
#include "resources.h"

/** BEGIN COMMAND **/
Command::Command( std::string name )
    :   name( name )
{
}

Command::~Command()
{
}

std::string Command::getName() const
{
    return name;
}

void Command::setName( std::string name )
{
    this->name = name;
}

/** END COMMAND **/


/** BEGIN COMMANDDATABASE **/

CommandDatabase::CommandDatabase()
{
}

CommandDatabase::~CommandDatabase()
{
    for ( std::vector< Command* >::iterator it = commands.begin(); it != commands.end(); ++it ) {
        delete (*it);
    }
    commands.clear();
}

bool CommandDatabase::handleCommandInput( int c )
{
	return true;
}

void CommandDatabase::loadDatabase()
{
    // delete our previous command database
    for ( std::vector< Command* >::iterator it = commands.begin(); it != commands.end(); ++it ) {
        delete (*it);
    }
    commands.clear();

    /** Parse commands here
        while () { addCommand(); } 
    **/
}

bool CommandDatabase::addCommand( std::string name )
{
	commands.push_back( new Command( name ) );
	return true;
}

bool CommandDatabase::addCommandInteractive()
{
	int height = 10;
	int width = 40;
	int y,x;
	getmaxyx( stdscr, y, x );
	WINDOW *newCommand = newwin( height,width,(y-height)/2,(x-width)/2 );
	box( newCommand, 0, 0 );
	mvwprintw( newCommand, 1, 1, "Command: " );
	int c = wgetch( newCommand );
	while ( handleCommandInput( c ) == true ) {
		c = wgetch( newCommand );
	}

	wrefresh( newCommand );
	return true;
}

std::vector< Command* > CommandDatabase::getCommands( std::string searchText )
{
	std::vector< Command* > retval;
	if ( searchText.empty() == false ) {
		for ( std::vector< Command* >::iterator it = commands.begin(); it != commands.end(); ++it ) {
			if ( (*it)->getName().find( searchText ) != std::string::npos ) {
				retval.push_back( (*it) );
			}
		}
	} else {
		retval = commands;
	}
	return retval;
}

/** END COMMANDDATABASE **/
