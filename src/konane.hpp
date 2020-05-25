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

#define NUMERO_CELLE_ORIZZONTALI 8
#define NUMERO_CELLE_VERTICALI 8
#define LIVELLO_DEFAULT 3
#define DEBUG 0

#include "gioco.hpp"
#include <gtk/gtk.h>
#include <iostream>
using namespace std;

class konane;  // dichiarazione
class pulsante // classe base per i pulsanti dell'interfaccia grafica
{
public:
  int x, y;            // le coordinate sulla scacchiera
  GtkWidget *immagine; // l'elemento grafico che contiene l'immagine
  konane *pgioco;      // un puntatore al gioco
  pulsante() {
    x = -1;
    y = -1;
  } // costruttore senza argomenti
  pulsante(int i, int j) {
    x = i;
    y = j;
  } // costruttore inizializzato
  int set(int i, int j) {
    x = i;
    y = j;
  }
};

struct cella // struttura dati per una cella della scacchiera
{
  int x, y;          // coordinate
  bool occupata;     // true se una pedina è in questa cella
  player appartiene; // a quale giocatore appartiene la pedina
};

struct tipo_statodelgioco // struttura dati per uno stato del gioco konane
{
  cella casella[NUMERO_CELLE_ORIZZONTALI + 1][NUMERO_CELLE_VERTICALI + 1];
  bool pedina_selezionata; // true se c'è una pedina selezionata
  cella sele;              // quale pedina è selezionata
};

struct tipo_mossa // struttura dati per una mossa del gioco konane
{
public:
  int i, j, i1,
      j1; // una mossa e' muovere una pedina da una cella (i,j) ad una (i1,j1)
};

class interfaccia_grafica // dati e metodi per la grafica
{
  pulsante scacchiera[NUMERO_CELLE_ORIZZONTALI + 1][NUMERO_CELLE_VERTICALI + 1];
  GtkWidget *suggerimenti;
  GtkWidget *ia;
  GtkWidget *status;

public:
  interfaccia_grafica(konane *pgioco);
  static void cliccato(GtkWidget *wid, gpointer data);
  // cosa fare quando un pulsante viene cliccato
  void impStatoPedina(int i, int j, gchar *data); // modifica una pedina
  void mossaGrafica(tipo_mossa mossa, player a);  // ridisegna la scacchiera
  void impSuggerimenti(const char *testo);        // aggiorna il testo
  void impIa(const char *testo);                  // aggiorna il testo
  void impStatus(const char *testo);              // aggiorna il testo
  static void impLivello(GtkWidget *widget, gpointer data);
  // cosa fare se l'utente modifica il livello
  void popup(char *titolo, char *testo); // crea una piccola finestra
};

// la classe che eredita le proprietà di gioco e di interfaccia grafica
class konane : public gioco<tipo_statodelgioco, tipo_mossa>,
               public interfaccia_grafica {
public:
  tipo_statodelgioco stato;      // stato del gioco
  int numeromosse;               // mosse eseguite finora
  konane();                      // costruttore, inizializza l'oggetto
  int mossaUtente(tipo_mossa a); // rispondere alla mossa del giocatore
  int valuta(tipo_statodelgioco stato);
  // funzione di valutazione ridefinita per lo specifico gioco konane
  lista<tipo_mossa> listamosse(tipo_statodelgioco stato, tipo_nodo tipo);
  // crea una lista di mosse possibili per un giocatore
  tipo_statodelgioco eseguimossa(tipo_statodelgioco stato, tipo_mossa mossa);
  // restituisce uno stato modificato da una mossa
  void aggiornaSuggerimenti(); // mostra le mosse del giocatore
};

// debugging functions
void printmossa(tipo_mossa mossa);
void coordinateMossa(tipo_mossa mossa, int &mangia_x, int &mangia_y,
                     int &presa_x, int &presa_y);
void stampa_scacchiera(tipo_statodelgioco stato);
void assegnapedina(cella &pedina, player a);
void mossaInizialeComputer(tipo_statodelgioco stato, int &x, int &y);
bool mossa_valida(tipo_statodelgioco &stato, const tipo_mossa &mossa,
                  player chimuove);
