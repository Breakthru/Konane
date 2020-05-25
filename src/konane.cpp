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

#include "konane.hpp"

using namespace std;

bool appartiene(cella &a, player chimuove);

void mostraCalcoli(const char *testo,
                   gioco<tipo_statodelgioco, tipo_mossa> *punt) {
  ((konane *)punt)->impIa(testo);
}

void printmossa(tipo_mossa mossa) {
  cout << "mossa da (" << mossa.i << "," << mossa.j << ") a (" << mossa.i1
       << "," << mossa.j1 << ")" << endl;
}

void stampa_scacchiera(tipo_statodelgioco stato) {
  int i, j;
  for (j = 1; j <= NUMERO_CELLE_VERTICALI; j++) {
    for (i = 1; i <= NUMERO_CELLE_ORIZZONTALI; i++) {
      if (stato.casella[i][j].occupata == false)
        cout << "-";
      else if (stato.casella[i][j].appartiene == giocatore)
        cout << "G";
      else
        cout << "C";
    }
    cout << endl;
  }
}
void delayemuovi(tipo_mossa mossaComputer, konane *pgioco);

void mossaInizialeComputer(
    tipo_statodelgioco stato, int &x,
    int &y) { // decidere la pedina che il computer deve inizialmente togliere

  do {
    x = 1 + (int)rand() % (NUMERO_CELLE_ORIZZONTALI);
    y = 1 + (int)rand() % (NUMERO_CELLE_VERTICALI);
    // cout << x << y << endl;
  } while (!(appartiene(stato.casella[x][y], computer) &&
             ((x < NUMERO_CELLE_ORIZZONTALI &&
               stato.casella[x + 1][y].occupata == false) ||
              (x > 1 && stato.casella[x - 1][y].occupata == false) ||
              (y < NUMERO_CELLE_VERTICALI &&
               stato.casella[x][y + 1].occupata == false) ||
              (y > 1 && stato.casella[x][y - 1].occupata == false))));
}

// cosa fare quando il giocatore effettua una mossa
int konane::mossaUtente(tipo_mossa mossa) {
  cout << "il giocatore fa la  ";
  printmossa(mossa);
  stato.pedina_selezionata = false;

  stato = eseguimossa(stato, mossa); // esegue la mossa valida da x,y a i,j
  numeromosse++;
  // interfaccia grafica
  mossaGrafica(mossa, giocatore);
  // cout << "pausa di 1 secondo" << endl;
  // g_usleep(1000);
  // se dopo la mossa del giocatore il pc non ha pi mossa, ha vinto
  if ((listamosse(stato, max_)).numero() == 0) {
    popup("Ho perso!!!", "Hai vinto!");
    return (1);
  }
  // eventuale delay prima di fare la mossa del pc

  tipo_mossa mossaComputer;
  mossaComputer = decidiMossa(stato);
  ostringstream testo;
  testo << "il computer ha mosso (" << mossaComputer.i << "," << mossaComputer.j
        << ") ---> (" << mossaComputer.i1 << "," << mossaComputer.j1 << ")";
  impStatus(testo.str().c_str());
  mossaGrafica(mossaComputer, computer);
  // esegui la mossa del computer
  stato = eseguimossa(stato, mossaComputer);

  // facciamo il punto della situazione
  aggiornaSuggerimenti();

  if ((listamosse(stato, min_)).numero() == 0) {
    popup("Pc felice!", "Ho vinto!!!");
    return (0);
  }
  if ((listamosse(stato, max_)).numero() == 0) {
    popup("Ho perso!!!", "Hai vinto!");
    return (1);
  }
}

int konane::valuta(tipo_statodelgioco stato) {
  int nPlayer, nPc;
  lista<tipo_mossa> temp;
  temp = listamosse(stato, min_);
  nPlayer = temp.numero();
  temp = listamosse(stato, max_);
  nPc = temp.numero();
  n_valutati++;
  if (nPc == 0)
    return (-INFINITO);
  if (nPlayer == 0)
    return (INFINITO);
  return (nPc - nPlayer);
}

void konane::aggiornaSuggerimenti() {
  lista<tipo_mossa> pc, g;
  ostringstream testo;
  int i;
  tipo_mossa temp;
  g = listamosse(stato, min_);
  pc = listamosse(stato, max_);

  testo << "Computer: " << pc.numero() << " mosse disponibili\n";
  testo << "Giocatore: " << g.numero() << " mosse disponibili\n";
  testo << "Le mosse che hai a disposizione ora sono:\n";
  for (i = 0; i < g.numero(); i++) {
    temp = g.estrai();
    testo << "(" << temp.i << "," << temp.j << ") ---> (" << temp.i1 << ","
          << temp.j1 << ")\n";
  }
  testo << endl;
  impSuggerimenti(testo.str().c_str());
}

bool appartiene(cella &a, player chimuove) {
  if (a.occupata == false)
    return (false);
  else if (a.appartiene == chimuove)
    return (true);
  else
    return (false);
}

player inverso(player a) {
  if (a == computer)
    return (giocatore);
  else if (a == giocatore)
    return (computer);
  else
    cout << "errore. giocatore non definito\n" << endl;
}

void trovaCandidatoMossaMultipla(int i, int j, int i1, int j1, int &i2,
                                 int &j2) {
  i2 = i1;
  j2 = j1;
  if (j == j1) { // mosse orizzontali
    if (i > i1) {
      j2 = j;
      i2 = i1 + 2;
    }
    if (i < i1) {
      j2 = j;
      i2 = i1 - 2;
    }
  }                   // fine mosse orizzontali
  else if (i == i1) { // mosse verticali
    if (j > j1) {
      i2 = i;
      j2 = j1 + 2;
    }
    if (j < j1) {
      i2 = i;
      j2 = j1 - 2;
    }
  } // fine mosse verticali
}

bool saltaunacella(tipo_statodelgioco &stato, const tipo_mossa &mossa,
                   player occupatada) {
  if (mossa.i == mossa.i1) // salto in verticale
  {
    if ((mossa.j1 == mossa.j + 2) &&
        appartiene(stato.casella[mossa.i][mossa.j + 1], occupatada))
      return (true);
    if ((mossa.j1 == mossa.j - 2) &&
        appartiene(stato.casella[mossa.i][mossa.j - 1], occupatada))
      return (true);
  } // fine salto verticale

  if (mossa.j == mossa.j1) // salto orizzontale
  {
    if ((mossa.i1 == mossa.i + 2) &&
        appartiene(stato.casella[mossa.i + 1][mossa.j], occupatada))
      return (true);
    if ((mossa.i1 == mossa.i - 2) &&
        appartiene(stato.casella[mossa.i - 1][mossa.j], occupatada))
      return (true);
  } // fine salto verticale

  return (false);
}

bool mossa_valida(tipo_statodelgioco &stato, const tipo_mossa &mossa,
                  player chimuove) { // controlla che la mossa sia valida per
                                     // chimuove sullo stato.
  int i = mossa.i;
  int i1 = mossa.i1;
  int j = mossa.j;
  int j1 = mossa.j1;
  int i2, j2;

  if ((i == i1) && (j == j1))
    return (true);
  // fine ricorsione

  player avversario = inverso(chimuove);
  if ((i < 1 || i > NUMERO_CELLE_ORIZZONTALI) ||
      (j < 1 || j > NUMERO_CELLE_VERTICALI) ||
      (i1 < 1 || i1 > NUMERO_CELLE_ORIZZONTALI) ||
      (j1 < 1 || j1 > NUMERO_CELLE_VERTICALI))
    return (false);

  if (stato.casella[i1][j1].occupata)
    return (false);

  if ((i != i1) && (j != j1))
    return (false);

  if (saltaunacella(stato, mossa, avversario))
    return (true);

  if (i == i1) {
    if ((abs(j - j1) % 2) != 0)
      return (false);
  }
  if (j == j1) {
    if ((abs(i - i1) % 2) != 0)
      return (false);
  }

  trovaCandidatoMossaMultipla(i, j, i1, j1, i2, j2);
  if ((i == i2) && (j == j2))
    return (false);
  tipo_mossa semplice, resto;
  semplice.i = i;
  semplice.j = j;
  semplice.i1 = i2;
  semplice.j1 = j2;
  resto.i = i2;
  resto.j = j2;
  resto.i1 = i1;
  resto.j1 = j1;
  return (mossa_valida(stato, semplice, chimuove) &&
          mossa_valida(stato, resto, chimuove));
}

void mossePossibili(tipo_statodelgioco &stato, int x, int y,
                    lista<tipo_mossa> &lista, player chimuove) {
  int i = x, j = y;
  tipo_mossa temp;
  for (i = 1; i <= NUMERO_CELLE_ORIZZONTALI;
       i++) { // possibili mosse orizzontali
    if (appartiene(stato.casella[i][j], chimuove)) {
      temp.i = i;
      temp.j = j;
      temp.i1 = x;
      temp.j1 = y;
      if (mossa_valida(stato, temp, chimuove)) {
        lista.inserisci(temp);
        // cout << "trovato una mossa possib " << temp.i<<temp.j<< " "
        // <<temp.i1<<temp.j1<<endl;
      }
    }
  }
  i = x;
  j = y;
  for (j = 1; j <= NUMERO_CELLE_VERTICALI; j++) { // possibili mosse verticali
    if (appartiene(stato.casella[i][j], chimuove)) {
      temp.i = i;
      temp.j = j;
      temp.i1 = x;
      temp.j1 = y;
      if (mossa_valida(stato, temp, chimuove)) {
        lista.inserisci(temp);
        // cout << "trovato una mossa possib " << temp.i<<temp.j<< " "
        // <<temp.i1<<temp.j1<<endl;
      }
    }
  }
}

player giocatore_nodo(tipo_nodo tipo) {
  if (tipo == min_)
    return (giocatore);
  else if (tipo == max_)
    return (computer);
  else
    cout << "errore, tipo non vale ne' min ne' max\n" << endl;
}

lista<tipo_mossa> konane::listamosse(tipo_statodelgioco stato, tipo_nodo tipo) {
  int i, j;
  lista<tipo_mossa> ret;
  for (j = 1; j <= NUMERO_CELLE_VERTICALI; j++) {
    for (i = 1; i <= NUMERO_CELLE_ORIZZONTALI; i++) {
      if (stato.casella[i][j].occupata == false)
        mossePossibili(stato, i, j, ret, giocatore_nodo(tipo));
      // aggiungi alla lista le possibili mosse che terminano nella cella ij
    }
  }
  return ret;
}

void coordinateMossa(
    tipo_mossa mossa, int &mangia_x, int &mangia_y, int &presa_x,
    int &presa_y) { // trova le coordinate in base alla direzione
  mangia_x = mossa.i;
  mangia_y = mossa.j;
  presa_x = mossa.i;
  presa_y = mossa.j;

  if (mossa.j == mossa.j1) {  // mosse orizzontali
    if (mossa.i > mossa.i1) { // verso ovest
      mangia_x = mossa.i - 1;
      mangia_y = mossa.j; // mangiata la pedina ad ovest
      presa_x = mossa.i - 2;
      presa_y = mossa.j;      // prendo la casella a due ovest
    }                         // fine mossa verso ovest
    if (mossa.i < mossa.i1) { // mossa verso est
      mangia_x = mossa.i + 1;
      mangia_y = mossa.j; // mangiata la pedina ad est
      presa_x = mossa.i + 2;
      presa_y = mossa.j;      // prendo la casella a due est
    }                         // fine mossa verso est
  }                           // fine del caso mosse orizzontali
  if (mossa.i = mossa.i1) {   // mosse verticali
    if (mossa.j > mossa.j1) { // mossa verso nord
      mangia_x = mossa.i;
      mangia_y = mossa.j - 1; // mangiata la pedina a nord
      presa_x = mossa.i;
      presa_y = mossa.j - 2;  // prendo la casella a due nord
    }                         // fine mossa verso nord
    if (mossa.j < mossa.j1) { // mossa verso sud
      mangia_x = mossa.i;
      mangia_y = mossa.j + 1; // mangiata la pedina a sud
      presa_x = mossa.i;
      presa_y = mossa.j + 2; // prendo la casella a due sud
    }                        // fine mossa verso sud
  }                          // fine mosse verticali

} // fine

void assegnapedina(cella &pedina, player a) { // assegna la pedina al giocatore
                                              // a
  if (a == nessuno)
    pedina.occupata = false;
  else {
    pedina.occupata = true;
    pedina.appartiene = a;
  }
}

tipo_statodelgioco konane::eseguimossa(tipo_statodelgioco stato,
                                       tipo_mossa mossa) {
  int mangia_x, mangia_y;
  int presa_x, presa_y;
  tipo_statodelgioco ret;
  ret = stato;
  coordinateMossa(mossa, mangia_x, mangia_y, presa_x, presa_y);
  // cout << "da " << mossa.i<<"," << mossa.j << " mangio " << mangia_x<<
  // ","<<mangia_y<< " e prendo "<<presa_x<<","<<presa_y<<endl;
  if (!stato.casella[mossa.i][mossa.j].occupata) {
    cout << "errore!";
    exit(1);
  }
  assegnapedina(ret.casella[presa_x][presa_y],
                ret.casella[mossa.i][mossa.j].appartiene);
  assegnapedina(ret.casella[mossa.i][mossa.j], nessuno);
  assegnapedina(ret.casella[mangia_x][mangia_y], nessuno);

  if ((mossa.i1 != presa_x) ||
      (mossa.j1 != presa_y)) { // esegui una mossa composta
    tipo_mossa temp;
    temp.i = presa_x;
    temp.j = presa_y;
    temp.i1 = mossa.i1;
    temp.j1 = mossa.j1;
    return (eseguimossa(ret, temp));
  }

  return (ret);
}

konane::konane() : interfaccia_grafica(this) {

  int i, j;
  numeromosse = 0;
  setliv(LIVELLO_DEFAULT);
  stato.pedina_selezionata = false;
  for (j = 1; j <= NUMERO_CELLE_VERTICALI; j++) {
    for (i = 1; i <= NUMERO_CELLE_ORIZZONTALI; i++) {

      if (((i + j) % 2) == 0) {
        // questa cella appartiene al giocatore
        stato.casella[i][j].occupata = true;
        stato.casella[i][j].appartiene = giocatore;
      } else {
        // questa cella appartiene al computer
        stato.casella[i][j].occupata = true;
        stato.casella[i][j].appartiene = computer;
      }
    }
  }

  if (DEBUG)
    stampa_scacchiera(stato);
}
