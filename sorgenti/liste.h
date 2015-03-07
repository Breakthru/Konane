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
#include <iostream>
#include <cstdlib>

template <class elemento> class lista {
struct NodoLista 
{elemento info;
NodoLista *next;};
NodoLista *testa,*corrente;
int n; //il numero di elementi
public:
lista() {n=0; testa=NULL; corrente=NULL; }
//Costruttore della classe, inizializza una lista vuota.


void inserisci(elemento e) //metodo per inserire un elemento
 {NodoLista *temp = new NodoLista;
  temp->info = e;
  temp->next= testa;
  testa = temp;
  n++;
  } 
// l'elemento passato alla funzione viene inserito in cima alla lista

void initEnum()   // inizializza una enumerazione degli elementi della lista
	{if (testa!=NULL) corrente=testa; 
	else {cout << "niente enumerazione di una lista vuota\n";exit(1);}
	}
// l'enumerazione  fatta grazie alla variabile NodoLista *corrente

elemento estrai()  // estrae un'elemento dall'enumerazione
{ 
if (corrente==NULL) initEnum();
if (corrente!=NULL) {
NodoLista *temp=corrente;
corrente=corrente->next;
return(temp->info); }
}

int numero() {return n;}
// restituisce la lunghezza della lista

void stampa()   // mostra la lista a video
{NodoLista *temp=testa;
while (temp!=NULL) {cout << temp->info << " "; temp=temp->next;}
cout << "\n";}

};
