/*
Konane 
Copyright (C) 2005 Marco Paladini
breakthru@inwind.it

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#define numero_celle_orizzontali 8
#define numero_celle_verticali 8
#define LIVELLO_DEFAULT 3
#define DEBUG 0

using namespace std;
#include <iostream>
#include <sstream>
//#include "gioco.h"
#include "konane.cpp"
#include "interfaccia.cpp"

int main (int argc,char *argv[])
{
  gtk_init (&argc, &argv);

konane play;

cout << "tutto inizializzato\n" << endl;


gtk_main ();
}
