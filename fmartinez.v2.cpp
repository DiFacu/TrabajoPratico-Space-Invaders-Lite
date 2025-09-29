#include <iostream>
#include <conio2.h>
#include <ctime>
#include <cstdlib>
#include <windows.h> 

using namespace std;

void pantallaInicio() {
	clrscr();
	textcolor(LIGHTCYAN);
	gotoxy(25,5);  cout << "SPACE INVADERS LITE";
	textcolor(WHITE); 
	gotoxy(20,7);  cout << "Controles: A (izq) | D (der) | SPACE (disparar)";
	gotoxy(20,9);  cout << "Objetivo: Elimina a todos los enemigos!";
	gotoxy(20,11); cout << "Autor: Facundo Martinez";
	gotoxy(20,13); cout << "Presiona cualquier tecla para comenzar...";
	getch();
	clrscr();
}

class Entidad {
protected:
	int x, y;
	bool activa;
public:
	Entidad(int _x=0, int _y=0) : x(_x), y(_y), activa(true) {}
	virtual void dibujar() = 0;
	virtual void borrar() {
		gotoxy(x,y); cout << " ";
	}
	int getX() { return x; }
	int getY() { return y; }
	void setXY(int _x, int _y) { x=_x; y=_y; }
	bool estaActiva() { return activa; }
	void desactivar() { activa=false; }
};

class Jugador : public Entidad {
	int vidas;
public:
	Jugador(int _x, int _y) : Entidad(_x,_y), vidas(3) {}
	void dibujar() override {
		gotoxy(x,y); textcolor(LIGHTCYAN);
		cout << "A"; 
	}
	void mover(char tecla) {
		borrar();
		if(tecla=='a' && x>2) x--;
		if(tecla=='d' && x<78) x++;
		dibujar();
	}
	int getVidas(){ return vidas; }
	void perderVida(){ vidas--; }
};


class Bala : public Entidad {
	int dy; 
public:
	Bala(int _x=0, int _y=0, int _dy=-1) : Entidad(_x,_y), dy(_dy) {}
	void dibujar() override {
		gotoxy(x,y); textcolor(WHITE);
		cout << "|";
	}
	void mover() {
		if(!activa) return;
		borrar();
		y += dy;
		if(y<=1 || y>=24) activa=false;
		else dibujar();
	}
};


class Enemigo : public Entidad {
protected:
	int resistencia;
public:
	Enemigo(int _x, int _y) : Entidad(_x,_y), resistencia(1) {}
	void dibujar() override {
		gotoxy(x,y); textcolor(LIGHTGREEN);
		cout << "M";
	}
	virtual void impactar() {
		resistencia--;
		if(resistencia<=0) activa=false;
	}
};


class EnemigoResistente : public Enemigo {
public:
	EnemigoResistente(int _x,int _y) : Enemigo(_x,_y) {
		resistencia=2;
	}
	void dibujar() override {
		gotoxy(x,y); textcolor(RED);
		cout << "W";
	}
};
int main() {
	srand(time(NULL));
	pantallaInicio();
	
	Jugador jugador(40,23);
	jugador.dibujar();	

	Bala balasJugador[5];
	Bala balasEnemigos[5];
	
	Enemigo* enemigos[20];
	for(int i=0;i<10;i++) enemigos[i] = new Enemigo(10+i*6,5);
	for(int i=0;i<10;i++) enemigos[10+i] = new EnemigoResistente(10+i*6,7);
	
	int direccion=1; 
	int puntaje=0;
	bool fin=false;
	
	while(!fin) {
	
		if(kbhit()) {
			char tecla=getch();
			if(tecla=='a'||tecla=='d') jugador.mover(tecla);
			if(tecla==' ') {
		
				for(int i=0;i<5;i++) {
					if(!balasJugador[i].estaActiva()) {
						balasJugador[i]=Bala(jugador.getX(),jugador.getY()-1,-1);
						balasJugador[i].dibujar();
						break;
					}
				}
			}
		}
		

		for(int i=0;i<5;i++) {
			if(balasJugador[i].estaActiva()) {
				balasJugador[i].mover();

				for(int j=0;j<20;j++) {
					if(enemigos[j]->estaActiva() &&
					   balasJugador[i].getX()==enemigos[j]->getX() &&
					   balasJugador[i].getY()==enemigos[j]->getY()) {
						enemigos[j]->impactar();
						balasJugador[i].desactivar();
						puntaje+=10;
					}
				}
			}
		}
		

		static int contador=0;
		contador++;
		if(contador%15==0){
			bool borde=false;
			for(int j=0;j<20;j++) {
				if(enemigos[j]->estaActiva()) {
					if((direccion==1 && enemigos[j]->getX()>=78) ||
						(direccion==-1 && enemigos[j]->getX()<=2))
						borde=true;
				}
			}
			if(borde) {
				direccion*=-1;
				for(int j=0;j<20;j++) if(enemigos[j]->estaActiva()) {
					enemigos[j]->borrar();
					enemigos[j]->setXY(enemigos[j]->getX(),enemigos[j]->getY()+1);
				}
			} else {
				for(int j=0;j<20;j++) if(enemigos[j]->estaActiva()) {
					enemigos[j]->borrar();
					enemigos[j]->setXY(enemigos[j]->getX()+direccion,enemigos[j]->getY());
				}
			}
		}
		
		for(int j=0;j<20;j++) if(enemigos[j]->estaActiva()) enemigos[j]->dibujar();
		

		for(int j=0;j<20;j++) if(enemigos[j]->estaActiva() && enemigos[j]->getY()>=23) {
			fin=true;
		}
		

		if(rand()%30==0){
			for(int i=0;i<5;i++){
				if(!balasEnemigos[i].estaActiva()){
					// buscar enemigo activo aleatorio
					int idx=rand()%20;
					if(enemigos[idx]->estaActiva()){
						balasEnemigos[i]=Bala(enemigos[idx]->getX(),enemigos[idx]->getY()+1,1);
						balasEnemigos[i].dibujar();
						break;
					}
				}
			}
		}
		

		for(int i=0;i<5;i++){
			if(balasEnemigos[i].estaActiva()){
				balasEnemigos[i].mover();
				if(balasEnemigos[i].getX()==jugador.getX() &&
				   balasEnemigos[i].getY()==jugador.getY()){
					jugador.perderVida();
					balasEnemigos[i].desactivar();
					if(jugador.getVidas()<=0) fin=true;
				}
			}
		}
		
		bool todosMuertos=true;
		for(int j=0;j<20;j++) if(enemigos[j]->estaActiva()) todosMuertos=false;
		if(todosMuertos) fin=true;
		
		gotoxy(2,1); textcolor(YELLOW);
		cout << "Vidas: " << jugador.getVidas() << "  Puntos: " << puntaje << "   ";
		
		Sleep(40); 
	}
	
	clrscr();
	gotoxy(30,10);
	if(jugador.getVidas()>0) cout << "GANASTE";
	else cout << "GAME OVER";
	gotoxy(30,12);
	cout << "Puntaje final: " << puntaje;
	gotoxy(25,15);
	cout << "Presiona una tecla para salir...";
	getch();
	
	return 0;
}
