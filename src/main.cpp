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
#include <sys/stat.h>
#include <sys/types.h>
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
	// make sure we have a ~/.ch/ structure
	char *home_path = getenv( "HOME" );
	char data_path[256];
	sprintf(data_path,"%s/.ch",home_path);
	mkdir( data_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
	// register our signal handler
	signal( SIGINT, handle_signal );

	Resources::Instance()->getWindow()->init();

	for(;;) {
		Resources::Instance()->getWindow()->draw();
	}
}

