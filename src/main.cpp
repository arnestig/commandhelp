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

//#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "resources.h"

void handle_signal( int signal )
{
	if ( signal == SIGINT ) {
		Resources::Instance()->DestroyInstance();
		exit(0);
	}
}

int main( int argc, char *argv[] )
{
	// register our signal handler
	signal( SIGINT, handle_signal );

	/** load some test commands **/
	Resources::Instance()->getCommandDatabase()->addCommand("make && ./ch");
	Resources::Instance()->getCommandDatabase()->addCommand("ls -lah");
	Resources::Instance()->getCommandDatabase()->addCommand("dh_make -s -p quickrdp_1-0");

	Resources::Instance()->getWindow()->init();

	for(;;) {
		Resources::Instance()->getWindow()->draw();
	}
}

