#include <iostream>
#include <conio2.h>

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
int main() {
	pantallaInicio();
	
	return 0;
} 

