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

#include "liste.hpp"
#include <sstream>

#define INFINITO 32000

enum player {nessuno,giocatore,computer};
// Un tipo di dati enumerato, costanti che individuano un giocatore
enum tipo_nodo {min_,max_};
// le due costanti che definiscono la struttura dati tipo_nodo

template <class tipo_statodelgioco, class tipo_mossa>
class gioco {
// la classe virtuale gioco si basa su due tipi di dati virtuali, tipo_statodelgioco e tipo_mossa

int livello_massimo;
int n_esplorati; // numero di nodi esplorati
int n_tagli; // numero di tagli effettuati
inline int massimo(int a,int b) { return a>b?a:b;}
inline int minimo(int a,int b) { return a<b?a:b;}
public:
// funzioni virtuali, devono essere ridefinite a seconda dell'oggetto che si vuole costruire

virtual int valuta(tipo_statodelgioco stato) {std::cout <<"errore: valuta non e' stato definito\n";}
// esegue la funzione di valutazione di uno stato

virtual lista<tipo_mossa> listamosse(tipo_statodelgioco stato, tipo_nodo tipo) {std::cout <<"errore: listamosse non e' stato definito\n";}
// restituisce la lista delle mosse possibili su uno stato per un giocatore (min o max)

virtual tipo_statodelgioco eseguimossa(tipo_statodelgioco stato,tipo_mossa mossa) {std::cout <<"errore: eseguimossa non e' stato definito\n";}
// esegue la mossa specificata sullo stato del gioco per restituire un nuovo stato del gioco

int n_valutati; // numero di nodi valutati
void setliv(int l) {livello_massimo=l;}
// impostare il livello massimo

int alfabeta(tipo_statodelgioco stato,int  livello, int alpha,int beta, tipo_nodo tipo);
// implementazione dell'algoritmo di alpha-beta pruning

tipo_mossa decidiMossa(tipo_statodelgioco gioco);
// funzione che sfrutta l'alpha-beta pruning per decidere la mossa migliore

};

template <class tipo_statodelgioco, class tipo_mossa>
int gioco<tipo_statodelgioco,tipo_mossa>::alfabeta(tipo_statodelgioco stato,int  livello, int alpha,int beta, tipo_nodo tipo)
{
if (livello >= livello_massimo) return(valuta(stato));
//se non e' stato raggiunto il livello massimo espandi il nodo
lista<tipo_mossa> possibili_mosse;
possibili_mosse = listamosse(stato,tipo);
//std::cout << "ci sono " << possibili_mosse.numero() << " mosse da qui per " << tipo << endl;
tipo_statodelgioco figlio;
n_esplorati++;
int v;
register int i;
    if (tipo==min_) {
            for (i=0;i<possibili_mosse.numero();i++) {
	    figlio=eseguimossa(stato,possibili_mosse.estrai()); //questo sviluppa il figlio
		v=alfabeta(figlio,livello+1,alpha,beta,max_);
	beta = minimo (beta, v);
            if (beta <= alpha) {n_tagli++; return alpha;}}
        return beta;}

    if (tipo==max_){
    for (i=0;i<possibili_mosse.numero();i++) {
	    figlio=eseguimossa(stato,possibili_mosse.estrai()); //questo sviluppa il figlio
v=alfabeta(figlio,livello+1,alpha,beta,min_);

        alpha = massimo (alpha, v);
            if (beta <= alpha) {n_tagli++; return beta;}}
        return alpha;}

}

template <class tipo_statodelgioco, class tipo_mossa> tipo_mossa gioco<tipo_statodelgioco,tipo_mossa>::decidiMossa(tipo_statodelgioco gioco)
{
std::ostringstream testo;
tipo_mossa mossaMigliore,mossa;
tipo_statodelgioco figlio;
lista<tipo_mossa> listaMosse;
int valoreMigliore;
int valore;
register int i;
bool taglio_mossa_vincente = false;
n_esplorati=1; n_valutati=0; n_tagli=0;
testo << "Valutazioni del computer per la mossa precedente:\n";
     //genero tutte le mosse possibili per quello stato del gioco
    listaMosse = listamosse(gioco,max_);
    mossa=listaMosse.estrai();
    testo << listaMosse.numero() << " mosse possibili:\n(" << mossa.i << "," << mossa.j << ") ---> (" << mossa.i1 << "," << mossa.j1 << ")";
    figlio=eseguimossa(gioco,mossa);
    valoreMigliore = alfabeta(figlio, 1, -INFINITO, +INFINITO,min_);
    testo << " valore " << valoreMigliore;
     mossaMigliore = mossa;
if(valoreMigliore== INFINITO) {taglio_mossa_vincente=true;}
     for (i=1;i<listaMosse.numero() && !taglio_mossa_vincente;i++) {
          mossa=listaMosse.estrai();
	  figlio=eseguimossa(gioco,mossa);
    valore = alfabeta(figlio, 1, -INFINITO, +INFINITO,min_);
    testo << "\n(" << mossa.i << "," << mossa.j << ") ---> (" << mossa.i1 << "," << mossa.j1 << ") valore "<<valore ;
if(valore== INFINITO) {taglio_mossa_vincente=true;}
          if (valore > valoreMigliore) {
                valoreMigliore = valore;
                mossaMigliore = mossa;}
}

if(taglio_mossa_vincente) {testo << "\ntaglio su mossa vincente \n";}
testo << "\n\nEsplorati " << n_esplorati << " nodi, " << n_valutati << " foglie valutate.\nEffettuato " << n_tagli << " tagli";
mostraCalcoli(testo.str().c_str(),this);
     return mossaMigliore;
}
