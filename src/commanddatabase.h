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

#ifndef __COMMAND_DATABASE__H_
#define __COMMAND_DATABASE__H_

#include <string.h>
#include <map>
#include <vector>

class Command
{
    public:
        Command( std::string name );
        ~Command();

	std::string getName() const;
	void setName( std::string name );
	bool execute();

    private:
        std::string name;
};

class CommandDatabase
{
    public:
        CommandDatabase();
        ~CommandDatabase();

	bool addCommand( std::string name );

    private:
        void loadDatabase();
        std::vector< Command* > commands;
};

#endif

