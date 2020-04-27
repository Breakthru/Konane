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

#define DELAY 200

void interfaccia_grafica::impSuggerimenti(const char *testo)
{
gtk_label_set_text(GTK_LABEL(suggerimenti),testo);
}

void interfaccia_grafica::impIa(const char *testo)
{
gtk_label_set_text(GTK_LABEL(ia),testo);
}

void interfaccia_grafica::impStatus(const char *testo)
{
gtk_label_set_text(GTK_LABEL(status),testo);
}

void chiudi_finestra(GtkWidget *wid,gpointer data)
{
gtk_widget_destroy((GtkWidget *)data);
}

void interfaccia_grafica::impLivello(GtkWidget *widget,gpointer data)
{
((konane *)data)->setliv((int)gtk_range_get_value(GTK_RANGE(widget)));
}
void distruggiPopup(GtkWidget *wid,gpointer data)
{
gtk_widget_destroy((GtkWidget *)data);
}

void interfaccia_grafica::popup(char *titolo,char *testo)
{
GtkWidget *finestra;
GtkWidget *messaggio;
GtkWidget *bottone;
GtkWidget *vbox;

finestra = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_position(GTK_WINDOW(finestra),GTK_WIN_POS_MOUSE);
gtk_window_set_title(GTK_WINDOW(finestra),titolo);
messaggio=gtk_label_new(testo);
bottone=gtk_button_new_with_label("Ok");
gtk_signal_connect(GTK_OBJECT(bottone),"clicked",G_CALLBACK(distruggiPopup),finestra);
vbox=gtk_vbox_new(false,0);
gtk_box_pack_start(GTK_BOX(vbox),messaggio,false,false,5);
gtk_box_pack_start(GTK_BOX(vbox),bottone,false,false,5);
gtk_container_add(GTK_CONTAINER(finestra),vbox);
gtk_widget_show_all(finestra);
}

struct xdelay
{
int i,j;
interfaccia_grafica *pgioco;
gchar *file;
};

gboolean muovidopo(gpointer data)
{int i,j;
gchar *file;
i=((xdelay *)data)->i;
j=((xdelay *)data)->j;
file=((xdelay *)data)->file;

(((xdelay *)data)->pgioco)->impStatoPedina(i,j,file);
delete((xdelay *)data);

return(FALSE);
}

void imp_Stato_Pedina(int q,int i,int j,interfaccia_grafica *pgioco,gchar *file)
{xdelay *aux;
aux=new(xdelay);
aux->i = i;
aux->j = j;
aux->pgioco =pgioco;
aux->file = file;

g_timeout_add(q*DELAY,muovidopo,(gpointer)aux);

}


void interfaccia_grafica::mossaGrafica(tipo_mossa mossa,player a)
{char *filename="computer.xpm";
int mx,my,x,y,q=4;
if(DEBUG) printmossa(mossa);
coordinateMossa(mossa,mx,my,x,y);
if (a==giocatore) {filename="player.xpm"; q=0;}

imp_Stato_Pedina(q,mossa.i,mossa.j,this,"vuota.xpm");
imp_Stato_Pedina(q,mx,my,this,"vuota.xpm");
imp_Stato_Pedina(q,x,y,this,filename);

//mossa composta, chiamata ricorsiva
if((x!=mossa.i1)||(y!=mossa.j1)) {
//cout << "mossa composta, chiamata ricorsiva"<<endl;
tipo_mossa temp;
temp.i = x;temp.j =y;
temp.i1 = mossa.i1; temp.j1 = mossa.j1;
mossaGrafica(temp,a);
}
}


void interfaccia_grafica::impStatoPedina(int i,int j,gchar *data)
{
//cout << "modifico la casella "<<i <<j << endl;

gtk_image_set_from_file(GTK_IMAGE(scacchiera[i][j].immagine),data);
//g_usleep(200);


}

void comesigioca(GtkWidget *wid,gpointer data)
{
GtkWidget *finestra;
GtkWidget *messaggio;
GtkTextBuffer *textbuffer;
GtkWidget *bottone;
GtkWidget *vbox;


finestra = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_position(GTK_WINDOW(finestra),GTK_WIN_POS_CENTER_ON_PARENT);
gtk_window_set_title(GTK_WINDOW(finestra),"Come si gioca");
gtk_window_set_default_size(GTK_WINDOW(finestra),450,200);

messaggio=gtk_text_view_new();
// gtk_widget_set_usize (messaggio,450,350);
textbuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(messaggio));
gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(messaggio),GTK_WRAP_WORD);
gtk_text_view_set_editable(GTK_TEXT_VIEW(messaggio),FALSE);
bottone=gtk_button_new_with_label("Ok");

gtk_signal_connect(GTK_OBJECT(bottone),"clicked",G_CALLBACK(distruggiPopup),finestra);
vbox=gtk_vbox_new(false,0);
gtk_box_pack_start(GTK_BOX(vbox),messaggio,true,false,5);
gtk_box_pack_start(GTK_BOX(vbox),bottone,false,false,5);
gtk_container_add(GTK_CONTAINER(finestra),vbox);
gtk_widget_show_all(finestra);

gtk_text_buffer_insert_at_cursor(textbuffer,"Il giocatore ha le pedine nere, il computer quelle bianche. Come mossa iniziale scegli una pedina da rimuovere.\n",-1);
gtk_text_buffer_insert_at_cursor(textbuffer,"Il computer eliminera' una sua pedina adiacente a quella rimossa. Dopodiche' il giocatore inizia a muovere. Le uniche mosse consentite sono quelle di saltare ",-1);
gtk_text_buffer_insert_at_cursor(textbuffer,"una pedina avversaria per mangiarla. Si puo' mangiare una pedina in orizzontale o in verticale. E' possibile anche mangiare piu' pedine, ma senza cambiare direzione.\n" ,-1);
gtk_text_buffer_insert_at_cursor(textbuffer,"Per effettuare una mossa clicca su una tua pedina, poi clicca sulla casella di destinazione. Se vuoi mangiare piu' pedine, clicca direttamente sulla casella finale.\n",-1);
gtk_text_buffer_insert_at_cursor(textbuffer,"Il gioco prosegue fin quando uno dei giocatori rimane senza mosse disponibili, e perde. Non accade mai un pareggio. \n", -1);
gtk_text_buffer_insert_at_cursor(textbuffer,"Sulla destra vengono mostrate le mosse che hai a disposizione, e i calcoli che effettua il computer prima di muovere.\n" ,-1);

}


void nuova_partita(GtkWidget *wid,gpointer data)
{konane *gioco=(konane *)data;
int i,j;
gioco->numeromosse=0;

for (j=1 ; j <= NUMERO_CELLE_VERTICALI ; j++) {
for (i=1 ; i <= NUMERO_CELLE_ORIZZONTALI ; i++){

if (((i+j) % 2)==0) {
// questa cella appartiene al giocatore
gioco->stato.casella[i][j].occupata=true;
gioco->stato.casella[i][j].appartiene=giocatore;
gioco->impStatoPedina(i,j,"player.xpm");
}
else {
// questa cella appartiene al computer
gioco->stato.casella[i][j].occupata=true;
gioco->stato.casella[i][j].appartiene=computer;
gioco->impStatoPedina(i,j,"computer.xpm");
}

}}

gioco->impIa("");
gioco->impStatus("");
gioco->impSuggerimenti("");

}

//funzione costruttore dell'oggetto interfaccia grafica
interfaccia_grafica::interfaccia_grafica(konane *pgioco)
{
//int NUMERO_CELLE_ORIZZONTALI=8;
//int NUMERO_CELLE_VERTICALI=8;
GtkWidget *finestra, *bottone, *pixmapwid;
GtkWidget *mybox;
GtkWidget *myvbox;
GtkWidget *topHbox;
GtkWidget *suggVbox;
GtkWidget *selettore;
GtkWidget *frame;
GtkWidget *come, *esci, *newgame, *menuHbox;
GtkWidget *separatore;

gchar *nomefile;
int i,j;


    finestra = gtk_window_new (GTK_WINDOW_TOPLEVEL);

frame=gtk_frame_new("Livello");
selettore=gtk_hscale_new_with_range(1,6,1);
gtk_container_add(GTK_CONTAINER(frame),selettore);
gtk_range_set_value(GTK_RANGE(selettore),LIVELLO_DEFAULT);
g_signal_connect (G_OBJECT (selettore), "value-changed", G_CALLBACK (impLivello),pgioco);


menuHbox = gtk_hbox_new (false, 0);
topHbox = gtk_hbox_new (false, 0);
suggVbox = gtk_vbox_new (false, 0);

separatore = gtk_separator_menu_item_new();

come=gtk_button_new_with_label("Come si gioca");
g_signal_connect (G_OBJECT (come), "clicked", G_CALLBACK (comesigioca),NULL);
esci=gtk_button_new_with_label("Esci");
g_signal_connect (G_OBJECT (esci), "clicked", G_CALLBACK (gtk_main_quit),NULL);
newgame=gtk_button_new_with_label("Nuova partita");
g_signal_connect (G_OBJECT (newgame), "clicked", G_CALLBACK (nuova_partita),pgioco);

suggerimenti=gtk_label_new("");
ia=gtk_label_new("");
status=gtk_label_new("");
/*suggerimenti=gtk_label_new("Suggerimenti");
ia=gtk_label_new("Calcoli effettuati dal computer");
status=gtk_label_new("Barra di stato");*/
gtk_label_set_line_wrap(GTK_LABEL(suggerimenti),TRUE);
gtk_label_set_line_wrap(GTK_LABEL(ia),TRUE);
gtk_label_set_line_wrap(GTK_LABEL(status),TRUE);


    gtk_widget_show(finestra);

myvbox = gtk_vbox_new (false, 0);
for (j=1 ; j <= NUMERO_CELLE_VERTICALI ; j++) {

mybox = gtk_hbox_new (false, 0);
for (i=1 ; i <= NUMERO_CELLE_ORIZZONTALI ; i++){

bottone=gtk_button_new();
scacchiera[i][j].x=i;
scacchiera[i][j].y=j;
scacchiera[i][j].pgioco=pgioco;

gtk_widget_set_usize(bottone,50,50);
g_signal_connect (G_OBJECT (bottone), "clicked", G_CALLBACK (cliccato),&scacchiera[i][j]);

if (((i+j) % 2)==0) {
// questa cella appartiene al giocatore
nomefile="./player.xpm";
}
else {
// questa cella appartiene al computer
nomefile="./computer.xpm";
}


pixmapwid = gtk_image_new_from_file(nomefile);
scacchiera[i][j].immagine=pixmapwid;

gtk_container_add(GTK_CONTAINER(bottone),pixmapwid);
gtk_box_pack_start(GTK_BOX(mybox),bottone,false,false,5);
}
gtk_box_pack_start(GTK_BOX(myvbox),mybox,false,false,5);
}


gtk_box_pack_start(GTK_BOX(suggVbox),suggerimenti,true,false,5);
gtk_box_pack_start(GTK_BOX(suggVbox),ia,true,false,5);

gtk_box_pack_start(GTK_BOX(suggVbox),status,false,false,5);

gtk_box_pack_start(GTK_BOX(topHbox),myvbox,false,false,15);
gtk_box_pack_start(GTK_BOX(topHbox),suggVbox,false,false,5);

gtk_box_pack_start(GTK_BOX(menuHbox),come,false,false,5);
gtk_box_pack_start(GTK_BOX(menuHbox),esci,false,false,5);
gtk_box_pack_start(GTK_BOX(menuHbox),newgame,false,false,5);
gtk_box_pack_start(GTK_BOX(menuHbox),frame,true,true,5);

gtk_box_pack_start(GTK_BOX(myvbox),separatore,false,false,5);
gtk_box_pack_start(GTK_BOX(myvbox),menuHbox,false,false,5);

gtk_container_add(GTK_CONTAINER(finestra),topHbox);
gtk_window_set_title( GTK_WINDOW(finestra), "Gioco del Konane");

 g_signal_connect (G_OBJECT (finestra), "destroy",
	              G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect (G_OBJECT (finestra), "delete_event",
	 	      G_CALLBACK (gtk_main_quit), NULL);


gtk_widget_show_all(finestra);



}

void interfaccia_grafica::cliccato(GtkWidget *wid,gpointer data)
{int x,y;
int i,j;
konane *gioco;
x= ((pulsante *)data)->x;
y= ((pulsante *)data)->y;
// la funzione riceve la struttura dati al gioco tramite un puntatore
// recupero i membri x,y e gioco grazie ad una conversione di tipo
gioco = ((pulsante *)data)->pgioco;
if(DEBUG) stampa_scacchiera(gioco->stato);
//cout << "bottone cliccato " << x << " " << y << endl;

if( (! gioco->stato.casella[x][y].occupata) && (!gioco->stato.pedina_selezionata) ) return;
if( (gioco->stato.casella[x][y].occupata) && (gioco->stato.casella[x][y].appartiene!=giocatore) )return;
//casi in cui non devo fare nulla.

if (gioco->numeromosse == 0) 
{ //elimina la pedina scelta dal giocatore nella prima mossa
 assegnapedina(gioco->stato.casella[x][y],nessuno);
 gioco->impStatoPedina(x,y,"vuota.xpm");
 gioco->numeromosse=1;
 mossaInizialeComputer(gioco->stato,x,y);
 cout <<"inizio rimuovendo la pedina in ("<< x <<"," << y<< ")" << endl;
 assegnapedina(gioco->stato.casella[x][y],nessuno);
 gioco->impStatoPedina(x,y,"vuota.xpm");

 return;
 } //finita la prima mossa

if (gioco->stato.pedina_selezionata) 
{ //il giocatore ha indicato una mossa 
i=gioco->stato.sele.x;
j=gioco->stato.sele.y;
//cout << "la pedina sele attualmente e' " << i << ","<<j << endl;
if( (x==i) && (y==j) ) 
{ // cliccare la pedina selezionata significa DEselezionarla.
//cout << "il giocatore ha DEselezionato la pedina " << i <<"," << j << endl;
gioco->stato.pedina_selezionata=false;
//grafica
gioco->impStatoPedina(i,j,"player.xpm");

return; //dopo il deselect l'azione e' finita
} //fine deselect
tipo_mossa mossa;
mossa.i=i; mossa.j=j;
mossa.i1=x; mossa.j1=y;
if( mossa_valida(gioco->stato,mossa,giocatore) ) 
{// il giocatore ha indicato una mossa valida 
//cout << "La mossa e' valida\n";
gioco->mossaUtente(mossa);
return;
} // l'azione e' finita, il giocatore ha fatto una mossa
else { // la mossa non e' valida
cout <<"La mossa NON e' valida!" << endl;
return;
}
} // non c'era pedina selezionata
else { // il giocatore ha selezionato una sua pedina
gioco->stato.pedina_selezionata=true;
//cout << "il giocatore ha selezionato la pedina " << x << " , " << y << endl;
gioco->stato.sele.x = x;
gioco->stato.sele.y = y;

 //grafica
 gioco->impStatoPedina(x,y,"select.xpm");
 return;
}
}
