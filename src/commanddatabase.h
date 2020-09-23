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

#ifndef __COMMAND_DATABASE__H_
#define __COMMAND_DATABASE__H_

#include <string>
#include <map>
#include <vector>

class Command
{
    public:
        Command( std::string name );
        ~Command();

		std::string getName() const;
        std::string getGroup() const;
		void setName( std::string name );

    private:
        std::string name;
        std::string group;
};

class CommandDatabase
{
    public:
        CommandDatabase();
        ~CommandDatabase();

		bool addCommandInteractive();
		bool addCommand( std::string name );
        Command* removeCommand( Command *command );
        void loadDatabase();
		std::vector< Command* > getCommands( std::string searchText = "" );
        Command* getCommandByName( std::string searchText );
		std::vector< Command* > getCommandsByGroup( std::string group );
        std::vector< std::string > getGroups();

    private:
		void appendNewCommandText( char *add );
		void popNewCommandText();
		bool handleCommandInput( int c );
		void writeDatabase();

		std::string newCommandText;
        std::vector< Command* > commands;
};

#endif

