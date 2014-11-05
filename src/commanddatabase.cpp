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
#include <stdlib.h>
#include <fstream>

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
	switch ( c ) {
		case KEY_ENTER:
		case K_ENTER:
			addCommand( newCommandText );
			newCommandText.clear();
			return false;
		break;
		case KEY_BACKSPACE:
		case K_BACKSPACE:
			popNewCommandText();
			return true;
		break;
		default:
			if ( c > 31 && c < 127 ) {
				appendNewCommandText( (char*)(&c) );			
				return true;
			}
		break;
	}
}

void CommandDatabase::appendNewCommandText( char *add )
{
	newCommandText.append( add );
}

void CommandDatabase::popNewCommandText()
{
	if ( newCommandText.length() > 0 ) {
		newCommandText.erase( newCommandText.end() - 1 );
	}
}

void CommandDatabase::loadDatabase()
{
    // delete our previous command database
    for ( std::vector< Command* >::iterator it = commands.begin(); it != commands.end(); ++it ) {
        delete (*it);
    }
    commands.clear();

	char *home_path = getenv( "HOME" );
	char database_path[256];
	sprintf(database_path,"%s/.ch/commands",home_path);
	std::ifstream ifs;
	ifs.open( database_path, std::ifstream::in );
	if ( ifs.is_open() == true ) {
		std::string line;

		while (std::getline(ifs, line)) {
			addCommand( line );
		}
	}
	ifs.close();
}

void CommandDatabase::writeDatabase()
{
	char *home_path = getenv( "HOME" );
	char database_path[256];
	sprintf(database_path,"%s/.ch/commands",home_path);
	std::ofstream ofs;
	ofs.open( database_path, std::ifstream::out );
	if ( ofs.is_open() == true ) {
		for ( std::vector< Command* >::iterator it = commands.begin(); it != commands.end(); ++it ) {
			ofs << (*it)->getName() << std::endl;
		}
	}
	ofs.close();
}

bool CommandDatabase::addCommand( std::string name )
{
	commands.push_back( new Command( name ) );
	writeDatabase();
	return true;
}

bool CommandDatabase::addCommandInteractive()
{
	int height = 3;
	int width = 100;
	int y,x;
	getmaxyx( stdscr, y, x );
	WINDOW *newCommand = newwin( height,width,(y-height)/2,(x-width)/2 );
	box( newCommand, 0, 0 );
	mvwprintw( newCommand, 1, 1, "Add new command: %s",newCommandText.c_str() );
	int c = wgetch( newCommand );
	while ( handleCommandInput( c ) == true ) {
		wclear( newCommand );
		box( newCommand, 0, 0 );
		mvwprintw( newCommand, 1, 1, "Add new command: %s",newCommandText.c_str() );
		c = wgetch( newCommand );
	}
	
	if ( newCommandText.empty() == false ) {
		return true;
	} else {
		return false;
	}
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
