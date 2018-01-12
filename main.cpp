#include <iostream>
#include <windows.h>


#define DIMENSIONE_TAVOLO 		5   //ti associa una parola a un numero(per evitare di continuare a scrivere 3)  (*1)
#define NUMERO_NAVI 			2   //(*)
#define DIMENSIONE_MASSIMA_NAVE 5

#define INIT 					0
#define ACQUA 					1
#define COLPITO 				8
#define COLPITO_AFFONDATO 		9


/*
* CLASSE COORDINATE
*/
class Coordinate {  
	private:
		int colonna;
		int riga;
		bool colpito;
		
	public:
		Coordinate();
		void setXY (std::string s);
		int getRiga() { return riga; }
        int getColonna() { return colonna; }
        std::string getXY() { return (std::to_string(riga) + "; " + std::to_string(colonna)); }
        bool isColpito () { return colpito; }
        void setColpito () { colpito = true; }
			
};

Coordinate::Coordinate(){
	colpito = false;
}

void Coordinate::setXY (std::string s){    //converte ad esempio A1 IN VALORE DI COLONNA E DI RIGA 
	if(s.size()>=2){
		riga = std::toupper(s[0])-'A';   //converte da carrattere a numero es. A-A=0
		colonna = s[1] - '1';
	}
	else
		std::cout << "Coordinata non valida!!" << std::endl;
}



/*
* CLASSE SHIP - NAVE (ogni giocatore ne detiene la sua lista)
*/
class Ship {
	private:
		int dimensione;      // dimensione nave (esempio nave da due, nave da tre...)
		Coordinate lista[DIMENSIONE_MASSIMA_NAVE];    // lista delle coordinate della mia nave (tra le parentesi quadre c'è la grandezza massima della nave che io posso inserire)
		int stato;  // esempio 1=affondata, 2=colpita
		
	public:
		Ship();
		void setdimensione (int d);
		int getdimensione () { return dimensione; }
		void setCoordinata (Coordinate c, int i);  
		Coordinate getCoordinata(int pos) { return lista[pos]; };
		Coordinate* getCoordinate() { return lista; };
		int setAttacco(Coordinate c);  //hanno ataccato in quelle coordinate li 
		int getstato () { return stato; }  // restituisce lo stato cioe sono stato colpito, colpito e affondato o non colpito
		void printCoord();  //stampa le coordinate per vedere dov'è la nave 
		void reset();  //dove non c'è la nave (-1)
};

Ship::Ship(){  //le caselle vuote le riempio con -1
	reset();
}

void Ship::reset(){
	Coordinate c;
	c.setXY("@0"); //@0=-1
	for(int i=0; i<DIMENSIONE_MASSIMA_NAVE; i++)
		lista[i]=c;
}

void Ship::setdimensione(int d){
	dimensione=d;
}

void Ship::setCoordinata(Coordinate c, int i){
	lista[i]=c;
}

int Ship::setAttacco(Coordinate c){
	int colpiti = 0, n_acqua = 0;
	for(int i=0; i<DIMENSIONE_MASSIMA_NAVE; i++){
		if(lista[i].getColonna() == c.getColonna() && lista[i].getRiga() == c.getRiga()){
			lista[i].setColpito();
			n_acqua++;
		}		
		if(lista[i].isColpito())
			colpiti++;				
	}
	if(colpiti == dimensione){
		stato = COLPITO_AFFONDATO;
		return stato;
	}		
	if(n_acqua==0)
		return ACQUA;
	if(colpiti>0)
		return COLPITO;
}


void Ship::printCoord(){  //serve per non stampare -1 dove non c'è la nave 
	for(int i=0; i<DIMENSIONE_MASSIMA_NAVE; i++){
		if(lista[i].getRiga()>=0)  //con questo non mi stampa le celle con -1 ovvero quelle vuote
			std::cout << lista[i].getXY() << std::endl;
	}
		
}



/*
* CLASSE BOARD - TAVOLO DA GIOCO (ogni giocatore ne possiede un'istanza per gli attacchi e una per il posizionamento delle navi)
*/
class Board {
	private:
		int mappa[DIMENSIONE_TAVOLO][DIMENSIONE_TAVOLO];
		
	public:
		void posizionaNavi(Ship s);
		int aggiornaTavolo (Coordinate c, int stato);  //attacco
		int getPos(Coordinate c) { //restituisce cosa c'è sul tavolo in una determinata coordinata
			return mappa[c.getRiga()][c.getColonna()];
		}
		void inizializza();
		void printBoard();		
};

void Board::posizionaNavi(Ship s){
	for(int i=0; i<DIMENSIONE_MASSIMA_NAVE; i++){
		Coordinate c = s.getCoordinata(i);
		if(c.getRiga()>=0){
			mappa[c.getRiga()][c.getColonna()] = s.getdimensione(); //imposto sulla mappa il valore della nave
		}
		else break;
	}
}

void Board::inizializza(){
	for(int i=0; i<DIMENSIONE_TAVOLO; i++){
		for(int j=0; j<DIMENSIONE_TAVOLO; j++){
			mappa[i][j]=INIT;
		}
	}
}


int Board::aggiornaTavolo(Coordinate c, int stato){
	mappa[c.getRiga()][c.getColonna()] = stato;
}


void Board::printBoard(){
	std::cout << "***************************************"<<std::endl;
	
	std::cout << "      ";
	for(int i=1; i<=DIMENSIONE_TAVOLO; i++)
		std::cout << i << "   ";
	std::cout << std::endl;
	
	for(int i=0; i<DIMENSIONE_TAVOLO; i++){
		char str[2];
		sprintf(str,"%c", i+65);
		std::cout << str <<" <|| ";
		for(int j=0; j<DIMENSIONE_TAVOLO; j++){
			std::cout << mappa[i][j] << " | ";
		}
		std::cout << std::endl ;
	}
	
	std::cout << "***************************************"<<std::endl;
}


/*
* CLASSE PLAYER - giocatore
*/
class Player {
	private:
		Ship 			navi[NUMERO_NAVI];  //(*)
		Board			tavolo;
		Board		 	attacchi;   // griglia degli attacchi del giocatore
		std::string 	nomegiocatore;
		
		
	public:
		void aggiungiNave(Ship l, int pos);
		Ship getNave(int pos);
		bool checkAttaccoValido(Coordinate a); //controllo che l'attacco che voglio fare sia valido
		int  attacca (Coordinate a, int statoDifesa);   //definire attacco che fara il giocatore
		int  difendi (Coordinate a);   //definire attacco che fara il giocatore
		Board getTavolo() { return tavolo; }
		Board getAttacchi() { return attacchi; }
		void setnomegiocatore (std::string n);
		std::string getnomegiocatore ();	
		void inizializzaGioco();
		int controllaStato();
};

void Player::inizializzaGioco(){
	tavolo.inizializza();
	attacchi.inizializza();
	
	for(int i=0;i<NUMERO_NAVI; i++)
		tavolo.posizionaNavi(navi[i]);
}

bool Player::checkAttaccoValido(Coordinate a){
	bool ret = false; //se attacco è valido set a true la variabile
	int att = attacchi.getPos(a);
	if(att==0) ret=true;
	return ret;
}
int Player::attacca(Coordinate a, int statoDifesa){
	attacchi.aggiornaTavolo(a, statoDifesa);
}

int Player::difendi(Coordinate a){
	int ret = ACQUA, nave;
	nave=tavolo.getPos(a);
	
	ret = navi[nave-2].setAttacco(a);		
	return ret;
}

int Player::controllaStato(){
	int navi_affondate=0;
	for(int i=0; i<NUMERO_NAVI; i++){
		if(navi[i].getstato()==COLPITO_AFFONDATO)
			navi_affondate++;
	}
	
	if(navi_affondate==NUMERO_NAVI) //se le mie navi sono tutte affondate restituisco 1 che vuol dire che ho perso, altrimenti 0
		return 1;
	return 0;
}

void Player::aggiungiNave(Ship l, int pos){
	navi[pos]=l;
}

Ship Player::getNave(int pos){
	return navi[pos];
}

void Player::setnomegiocatore(std::string s){
	nomegiocatore = s;
}

void attendiTasto(){
	system("pause");
}





/*
* PROGRAMMA PRINCIPALE
*/
int main(int argc, char** argv) {
	
	//Define two players
	Player P1,P2;   //istanza della classe Player 
	std::string player1Name, player2Name;
		
	std::cout << "BATTLESHIP (campo dimensioni:" << DIMENSIONE_TAVOLO << ", numero navi: " << NUMERO_NAVI <<")\n";
	std::cout << "Inserisci nome giocatore1:\n";
	std::cin >> player1Name;
	std::cout << "Inserisci nome giocatore2:\n";
	std::cin >> player2Name;
	
	P1.setnomegiocatore(player1Name);
	P2.setnomegiocatore(player2Name);

	std::string input;  //VARIABILE DI APPOGGIO (dove verrà messo quello che eio inserisco in questo caso le coordinate della nave)
	Coordinate coord;
	
	Ship Stmp;			//NAVE temporaneo d'appoggio
	//imposto navi giocatore 1 e 2
	for(int p=0; p<2; p++){
		for(int i=0; i<NUMERO_NAVI; i++){
			Stmp.reset();
			Stmp.setdimensione(i+2);
			std::cout << "giocatore " << p+1 << "-> inserisci le coordinate (es.A1, B2, ecc )della nave da "<<i+2<<std::endl;
			for(int j=0; j<i+2; j++){
				std::cout << "Coordinata " << j+1 << ": ";
				std::cin>>input;
				coord.setXY(input);  			//creata la coordinata (metedo guardare sopra)
				
				Stmp.setdimensione(i+2);
				Stmp.setCoordinata(coord,j);  //voglio assegnare una coordinata alla nave S1_1
			}			

			if(p==0)
				P1.aggiungiNave(Stmp,i);
			else if(p==1)
				P2.aggiungiNave(Stmp,i);

		}		
	}
	
	P1.getNave(0).printCoord();
    P2.getNave(0).printCoord();	
	
	P1.inizializzaGioco();
	P2.inizializzaGioco();
	
	std::cout << "TAVOLO GIOCATORE 1\n";
	P1.getTavolo().printBoard();
	P1.getAttacchi().printBoard();
	attendiTasto();
	
	std::cout << "TAVOLO GIOCATORE 2\n";
	P2.getTavolo().printBoard();
	P2.getAttacchi().printBoard();
	attendiTasto();
	
	
	system("cls");
	std::cout << "GAME START!!!" << std::endl;
	std::cout << player1Name << " vs " << player2Name << std::endl;
	attendiTasto();
	system("cls");
	
	int i=0;	
	Coordinate attacco;
	int statoAttacco = 0, statoDifesa = 0;
	
	int gameOver = 0;
	bool c;
	
	while(!gameOver){
		do{ //ciclo di attesa finchè non viene inserita una coordinata valida (cioè se non ho già inserito quella coordinata prima)
			std::cout << player1Name << " colpisci l'avversario: " << std::endl;
			std::cin>>input;
			attacco.setXY(input);
			c = P1.checkAttaccoValido(attacco); //controllo se attacco è valido, altrimenti lo segnalo all'utente e richiedo una coordinata
			if(!c)
				std::cout << player1Name << " hai già sparato in questa posizione!!" << std::endl;
		}while(!c);
		
		statoDifesa = P2.difendi(attacco);
		
		if(statoDifesa==ACQUA)
			std::cout<<"ACQUA!!!"<<std::endl;
		if(statoDifesa==COLPITO)
			std::cout<<"COLPITA NAVE!!!"<<std::endl;
		if(statoDifesa==COLPITO_AFFONDATO)
			std::cout<<"NAVE AFFONDATA!!!"<<std::endl;
					
		statoAttacco = P1.attacca(attacco, statoDifesa);
		P1.getAttacchi().printBoard();
		attendiTasto();
		system("cls");

		do{ //ciclo di attesa finchè non viene inserita una coordinata valida (cioè se non ho già inserito quella coordinata prima)
			std::cout << player2Name << " colpisci l'avversario: " << std::endl;
			std::cin>>input;
			attacco.setXY(input);
			c = P2.checkAttaccoValido(attacco);
			if(!c)
				std::cout << player2Name << " hai già sparato in questa posizione!!" << std::endl;
		}while(!c);
			
		statoDifesa = P1.difendi(attacco);
		if(statoDifesa<0)
			std::cout << player2Name << " hai già sparato in questa posizione!!" << std::endl;	
			
		if(statoDifesa==ACQUA)
			std::cout<<"ACQUA!!!"<<std::endl;
		if(statoDifesa==COLPITO)
			std::cout<<"COLPITA NAVE!!!"<<std::endl;
		if(statoDifesa==COLPITO_AFFONDATO)
			std::cout<<"NAVE AFFONDATA!!!"<<std::endl;
		
		statoAttacco = P2.attacca(attacco, statoDifesa);
		P2.getAttacchi().printBoard();
		attendiTasto();
		system("cls");
		
		
		//controllo se gioco è finito
		//se un giocatore ha tutte le navi affondate ha perso
		//se entrambi i giocatori sono in quello stato allora c'è un pareggio	
		gameOver += P1.controllaStato();
		gameOver += P2.controllaStato();
	}
	
	switch(gameOver){
		case 1:
			if(P2.controllaStato()==1)
				std::cout << "VINTO GIOCATORE 1!!" << std::endl;
			else
				std::cout << "VINTO GIOCATORE 2!!" << std::endl;
			break;
		case 2:
			std::cout << "PAREGGIO!!" << std::endl;
			break;
		default:
			std::cout << "IMPOSSIBILE!!" << std::endl;
			break;
	}
}


