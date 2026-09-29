//Martin Ratti
//Guillermo Doyle
//Ana Clara Pennacchio
/************************************************************************
Idea:
Alicia tiene que encontrar 5 veces al gato de cheshire evitando las flores.
la quinta vez encunetra la llave para salir del laberinto y se enfrenta a la oruga (hay que evitar sus "balas")
Despues se repite el formato pero con las cartas y la reina de corazones (pero con más dificultad)

REVISAR               
● Instrucciones del juego donde además se explique la idea del trabajo.  ❌
● El videojuego deberá tener un título ❌
- Recuerden trabajar la retórica del juego, tanto en el sentido y en la estética. 

CORREGIR

Pausa post jefe
Balas de jefe

DETALLES
Flores saltan pared al colisionar
Conejito 

---------------------------------------------------

************************************************************************/
#include <iostream>
#include <ncurses.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Alicia.h"
#include "Enemigo.h"
#include "Gato.h"
#include "Jefe.h"
#include "Proyectil.h"
#include "Bala.h"

using namespace std;

//=========================================================================
//DECLARACIONES
//=========================================================================

// PANTALLA
const int ANCHO = 120;
const int ALTO = 40;
const int DELAY = 30; 

//MOVIMIENTO
enum opciones
{
	INICIAR = 18,
	INSTRUCCIONES = 20,
	CREDITOS = 22,
	SALIR = 24
};

enum direcciones
{
	IZQUIERDA,
	DERECHA,
	ARRIBA,
	ABAJO
};

//COLORES
enum pares
{
	PAR_PARED = 1,					//negro-verde
	PAR_GATO,						//negro-magenta
	PAR_ALICIA,						//negro-cian
	PAR_CORAZONES,					//negro-rojo
	PAR_CONEJO,						//negro-blanco
	PAR_FLORES,						//negro-amarillo
	PAR_ORUGA,						//negro-azul
};

//VARIABLES
int opcion_y;						//opcion del menu
bool salir;							//salir (false) = seguir jugando
bool game_over ;					//game over (true) = menú o salir
bool menu;							//menu (false) = juego
int nivel;							//En que nivel está el juego (1, 2, 3 o 4)
int disparo;						//si disparo <= 10, el jefe dispara la cantidad de balas que sea la variable
bool charla;						//si hay cutscene o no
bool charla_2;
bool fin;


//OBJETOS
Alicia prota;						//Protagonista
Gato gato;							//Recolectable
vector<Proyectil> Proyectiles;		//Disparos del protagonista en 2° nivel
vector<Bala> Balas;					//Disparos del jefe
Jefe jefe;							//Jefe (nivel 2 = orgua, nivel 4 = reina de corazones)
		
vector <Enemigo> Enemigos;			//Enemigos en el laverinto (nivel 1 = flores, nivle 3 = corazones)
vector <vector<int>> coordenada =	//Coordenadas de enemigos en laberintos 
{
	{37,12,1},
	{4,13,1},
	{10,26,1},
	{26,78,2},
	{30,72,2},
	{9,90,1},
	{17,27,1},
	{17,70,1},
	{24,70,1},
	{27,90,1},
	{36,80,1},
	{3,44,1},
	{22,32,2},
	{17,45,2},
	{17,57,2},
	{3,72,2},
	{17,90,2},
	{9,52,2}
};

//DIBUJOS
  string d_alicia [7]
  {
	" (>o<)______.",
	" . ((('')))  .",
	"| s| .   . |s |",
	"| s|   o   |s |",
	"| s|   _   |s |",
	"|   \\_____/   |",
	"L, , ,| |, , ,J"
  };
    string d_oruga [7]
  {
"   n________n   O",
"  (         )  o °",
"  (  T    T )   °",
"  (     C===)==U",
" (          )",
"  (          )",
" (           )"
  };
    string d_reina [7]
  {
"  |\\ |\\m/| /|",
" J|_\\|_V_|/_|L",
"CJ  .\\   /.  LD",
"CJ     L     LD",
"CJ    /-\\    LD",
" J\\_________/L",
" J,,|     |,,L"
  };
  string d_gato [7]
  {
    "  |\\________/|",
    "  {  @    @  }",
    " ={  \\__I__/ }=",
    "  {   \\WWW/  }",
    "  {          }",
    " {           }",
    "{             }"
  };
//FUNCIONES
void inicio();						//Menú
void menu_input();					//Input para el menu
void instruc_creditos(int cual);			
void setup();						//Inicia/reinicia valores para el 1° nivel
void cambio_nivel();				//Inicia/reinicia valores cada que se pasa de nivel
void input();						//Maneja movimientos y disparos
bool puede_moverse (int y, int x);	//Devuelve si el personaje puede moverse o no en la direccion que el input indique
void inicializar_pares_colores();	//Inicializa los pares de colores
void update();						//Actualiza valores y todo lo que pasa en cada momento en el jugeo
void draw(int alto, int ancho);		//Dibuja cada frame
void gameover();					//Mensaje al perder
void dibujar_laberinto(int alto, int ancho);
void cutscene(int nivel);			//Pequeñas charlas entre niveles
void cutscene_2(int nivel);
void final();


//=========================================================================
// MAIN
//=========================================================================

int main()
{
	setlocale(LC_ALL, "");
	initscr();
	noecho();
	curs_set(false);
	keypad(stdscr, true);
	srand (time(0));				//Semilla del random
	start_color();					//inicializa color
	inicializar_pares_colores();
	nodelay(stdscr, true);

	if (LINES < ALTO || COLS < ANCHO)
	{
		endwin();
		printf("La terminal tiene que tener como mínimo %dx%d\n\n", ANCHO, ALTO);
		exit(1);
	}

	salir = false;
	disparo = 5;
	menu = true;
	opcion_y = INICIAR;
	fin = false;

	while (!salir)
	{		
		inicio();
		menu_input();
		while (!menu)
		{	
			while (!game_over)
			{
				input();
				update();
				draw(ALTO, ANCHO);
			}
			if (game_over && !fin)	gameover();
		}
	}

	endwin(); 
	cout << endl;
	return 0;
}

//=========================================================================
// FUNCIONES
//=========================================================================

void inicio()
{	

	int color = rand()% 7 + 1;
	
	attron (COLOR_PAIR(PAR_GATO));
	dibujar_laberinto(ALTO, ANCHO);
	attroff (COLOR_PAIR(PAR_GATO));

	for (int y = 10; y < 32; y++)
	mvhline(y, 29, ' ', 65);

	
	attron (COLOR_PAIR(color));
	mvhline(9, 28, ACS_HLINE, 66);
	mvhline(31, 28, ACS_HLINE, 66);

	mvvline(9, 28, ACS_VLINE, 23);
	mvvline(9, 94, ACS_VLINE, 23);

	mvaddch(9, 28, ACS_ULCORNER);
	mvaddch(9, 94, ACS_URCORNER);
	mvaddch(31, 28, ACS_LLCORNER);
	mvaddch(31, 94, ACS_LRCORNER);
	attroff (COLOR_PAIR(color));

	attron (COLOR_PAIR(PAR_ALICIA));
	mvprintw(12, 33, "╔═╗╦  ╦╔═╗╦╔═╗  ┌─┐┌┐┌  ┌─┐┬    ╦  ╔═╗╔╗ ╔═╗╦═╗╦╔╗╔╦╗╔═╗");
	mvprintw(13, 33, "╠═╣║  ║║  ║╠═╣  ├┤ │││  ├┤ │    ║  ╠═╣╠╩╗║╣ ╠╦╝║║║║║ ║ ║");
	mvprintw(14, 33, "╩ ╩╩═╝╩╚═╝╩╩ ╩  └─┘┘└┘  └─┘┴─┘  ╩═╝╩ ╩╚═╝╚═╝╩╚═╩╝╚╝╩ ╚═╝");
	attroff (COLOR_PAIR(PAR_ALICIA));

	mvprintw(INICIAR, 38, "Iniciar juego");
	mvprintw(INSTRUCCIONES, 38, "Instrucciones");
	mvprintw(CREDITOS, 38, "Creditos");
	mvprintw(SALIR, 38, "Salir");


	mvprintw(30, 44, "Apretar \"z\" para elegir la opcion");
}

//* ///////////////////////////////////////////////////////////////////////
void menu_input()
{	
	wchar_t w_reloj = L'\u23f2';
	cchar_t reloj;
	setcchar(&reloj, &w_reloj, A_NORMAL, 0, nullptr);

	int tecla = getch();
	int x = 35;

    mvadd_wch(opcion_y, x, &reloj);
    refresh();

        mvaddch(opcion_y, x, ' '); 

		switch (tecla)
		{			
			case KEY_UP:
				if (opcion_y == INICIAR) opcion_y = SALIR;
				else opcion_y -= 2;
				break;
			case KEY_DOWN:
				if (opcion_y == SALIR) opcion_y = INICIAR;
				else opcion_y += 2;
				break;
			case 'z':
				switch (opcion_y)
				{
				case INICIAR:
					menu = false;
					setup();
					break;
				case INSTRUCCIONES:
					instruc_creditos(INSTRUCCIONES);
					break;
				case CREDITOS:
					instruc_creditos(CREDITOS);
					break;
				case SALIR:
					salir = true;
					break;
				default:
					break;
				}
			break;
		default:
			break;
	
		mvadd_wch(opcion_y, x, &reloj); 
        refresh();
	}
}

//* ///////////////////////////////////////////////////////////////////////
void instruc_creditos(int cual)
{
	nodelay(stdscr, false);
	for (int y = 5; y < 36; y++)
	mvhline(y, 20, ' ', 80);

	mvhline(4, 20, ACS_HLINE, 80);
	mvhline(36, 20, ACS_HLINE, 80);

	mvvline(4, 20, ACS_VLINE, 33);
	mvvline(4, 100, ACS_VLINE, 33);

	mvaddch(4, 20, ACS_ULCORNER);
	mvaddch(4, 100, ACS_URCORNER);
	mvaddch(36, 20, ACS_LLCORNER);
	mvaddch(36, 100, ACS_LRCORNER);

	switch (cual)
	{
	case INSTRUCCIONES:
		mvprintw ( 6, 22, "INSTRUCCIONES" );

		mvprintw ( 9, 22, "MOVIMIENTO:" );
		mvprintw ( 10, 22, "Usa las flechas del teclado para mover a Alicia en el laberinto." );

		mvprintw ( 13, 22, "DISPARAR:" );
		mvprintw ( 14, 22, "Presiona la tecla \"z\" para disparar y defenderte en los niveles de combate" );

		mvprintw ( 17, 22, "INTERACCIONES:" );
		mvprintw ( 18, 22, "Cuando te encuentres con el Gato de Cheshire" );
		mvprintw ( 19, 22, "(indicado con \"OWO\")" );
		mvprintw ( 20, 22, "acercate para interactuar. Cheshire te dará pistas o te desafiara a seguirlo." );
		mvprintw ( 21, 22, "¡Mantente alerta!. Es la unica manera de avanzar en el laberinto. " );

		mvprintw ( 24, 22, "OBSTACULOS:" );
		mvprintw ( 25, 22, "Esquiva los enemigos que te vas encontrando en el laberinto" );
		mvprintw ( 26, 22, "(indicados como ✿ o ♥)" );

		mvprintw ( 29, 22, "JEFES:" );
		mvprintw ( 30, 22, "Durante los combates contra jefes, usa la tecla \"z\" para disparar." );
		mvprintw ( 31, 22, "¡Intenta esquivar sus balas!" );
		mvprintw ( 32, 22, "Derrota a los jefes para avanzar al siguiente nivel." );

		break;
	case CREDITOS:
		mvprintw ( 6, 22, "CRÉDITOS:" );
		
		mvprintw ( 9, 22, "UNIVERSIDAD NACIONAL DE LAS ARTES" );
		mvprintw ( 10, 22, "Informática General" );
		mvprintw ( 11, 22, "CÁTEDRA TIRIGALL" );
		mvprintw ( 12, 22, "ÁREA TRANSDEPARTAMENTAL DE ARTES MULTIMEDIALES" );

		mvprintw ( 14, 22, "Titular: Lic. Juan José María Tirigall" );
		mvprintw ( 15, 22, "Ayudante: Lic. Noelia Melina Qualindi" );

		mvprintw ( 17, 22, "TRABAJO PRÁCTICO 2" );

		mvprintw ( 19, 22, "GameArt \"Alicia en el Laberinto\"" );
		mvprintw ( 20, 22, "Realizado por los Alumnos:" );
		mvprintw ( 21, 22, "Doyle, Guillermo" );
		mvprintw ( 22, 22, "Pennacchio, Ana Clara" );
		mvprintw ( 23, 22, "Ratti, Martin" );
		break;
	default:
		break;
	}	
	mvprintw ( 35, 48, "Presione cualquier tecla para volver al menú." );
	getch();
	clear();
	nodelay(stdscr, true);
}

//* ///////////////////////////////////////////////////////////////////////
void setup()
{
	game_over = false;
	fin = false;

	Balas.clear();
	Proyectiles.clear();
	Enemigos.clear();
	charla = true;
	charla_2 = true;

	prota.setup();				//Inicia o reinicia a Alicia
	gato.setup();				//Inicia o reinicia al Gato
	nivel = 1;					//Inicia o reinicia nivel
	for (int i = 0; i < 7; i++)	Enemigos.push_back(Enemigo(coordenada[i][0],coordenada[i][1],coordenada[i][2]));

	//Para seleccionar en que nivel != 1 comenzar escribir el nivel que se quiera -1
	//cambio_nivel();
}

//* ///////////////////////////////////////////////////////////////////////
void cambio_nivel()
{	
	nivel++;
	Balas.clear();
	Proyectiles.clear();
	Enemigos.clear();
	charla = true;
	charla_2 = true;
	
	if (nivel == 2 || nivel == 4)			//pase de nivel 1 a 2 & 3 a 4
	{
		prota.setY(36);
		prota.setX(1);
		jefe.setup(nivel);
	}
	else if (nivel == 3)				//pase de nivel 2 a 3
	{
		for (int i = 0;i < coordenada.size(); i++)	Enemigos.push_back(Enemigo(coordenada[i][0],coordenada[i][1],coordenada[i][2]));
		prota.setup();
		gato.setup();	
	}	
	if (nivel == 5)
	{
		final();
	}
}

//* ///////////////////////////////////////////////////////////////////////
void input()
{
	int tecla = getch();
	int y = prota.getY();
	int x = prota.getX();
	
	switch (tecla)
	{		
		
	case KEY_UP:
		
		if (puede_moverse(y - 1, x)&&puede_moverse(y - 1, x - 1)&&puede_moverse(y - 1, x + 1)) prota.setY(prota.getY() - 1);
        break;
	case KEY_DOWN:
		if (puede_moverse(y + 1, x)&&puede_moverse(y + 1, x -1)&&puede_moverse(y + 1, x + 1)) prota.setY(prota.getY() + 1);
		break;
	case KEY_LEFT:
		if (puede_moverse(y, x - 2)) prota.setX(prota.getX() - 1);
		break;
	case KEY_RIGHT:
		if (puede_moverse(y, x + 2)) prota.setX(prota.getX() + 1);
		break;
	case 'z':
		Proyectiles.push_back(Proyectil(prota.getX() , prota.getY(), nivel));
		break;
	case 27:
		game_over = true;
		break;
	default:
		break;
	}
	
}

//* ///////////////////////////////////////////////////////////////////////
bool puede_moverse(int y, int x)
{								
	//Colision con paredes
    chtype ch = mvinch(y, x)&A_CHARTEXT;
    return !(ch  == (ACS_HLINE &A_CHARTEXT)
			|| ch == (ACS_VLINE &A_CHARTEXT)
			|| ch== (ACS_ULCORNER &A_CHARTEXT)
			|| ch== (ACS_LLCORNER &A_CHARTEXT)
			|| ch== (ACS_URCORNER &A_CHARTEXT)
			|| ch== (ACS_LRCORNER &A_CHARTEXT)
			|| ch== (ACS_LTEE &A_CHARTEXT)
			|| ch== (ACS_RTEE &A_CHARTEXT)
			|| ch== (ACS_BTEE &A_CHARTEXT)
			|| ch== (ACS_TTEE &A_CHARTEXT)
			|| ch == ('x' &A_CHARTEXT));
}

//* ///////////////////////////////////////////////////////////////////////
void inicializar_pares_colores()
{
	init_pair(PAR_PARED, COLOR_GREEN, COLOR_BLACK);
	init_pair(PAR_ALICIA, COLOR_CYAN, COLOR_BLACK);
	init_pair(PAR_GATO, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(PAR_CORAZONES, COLOR_RED, COLOR_BLACK);
	init_pair(PAR_CONEJO, COLOR_WHITE, COLOR_BLACK);
	init_pair(PAR_FLORES, COLOR_YELLOW, COLOR_BLACK);
	init_pair(PAR_ORUGA, COLOR_BLUE, COLOR_BLACK);
}

//* ///////////////////////////////////////////////////////////////////////
void update()
{
	//Update y colisiones de Objetos
	prota.update();	

	if (nivel == 1|| nivel == 3)						//Nivel 1 y 3
	{	
		attron (COLOR_PAIR (PAR_GATO));
		gato.update();
		gato.colision(prota, nivel);	
		attroff (COLOR_PAIR (PAR_GATO));

		for (int i = 0; i <Enemigos.size(); i++)
		{
			Enemigos[i].update();
			Enemigos[i].colision(prota);
		}			
	}
	if ((nivel == 2 || nivel == 4 )&& prota.getX() > 24)//Nivel 2 y 4
	{
		//Disparos de Alicia por input
		for (int i = 0; i < Proyectiles.size(); i++) Proyectiles[i].update();
		for (int i = 0; i < Proyectiles.size(); i++)
		{ 
			if (Proyectiles[i].colision(jefe)||Proyectiles[i].limite() )
			{
				Proyectiles.erase(Proyectiles.begin() + i);
			}
		}
		//Disparos de Jefe por aleatoriedad

		for (int i = 0; i < Balas.size(); i++) Balas[i].update();
		for (int i = 0; i < Balas.size(); i++)
		{ 
			if (Balas[i].colision(prota)||Balas[i].limite() )
			{
				Balas.erase(Balas.begin() + i);
			}
		}
				if (disparo <= 10)
		{
			for (int i = 0; i <= disparo; i++)	
			{	
				if (nivel == 2)	
				{
					Balas.push_back(Bala(jefe.getX() , jefe.getY()));
					Balas.push_back(Bala(jefe.getX() + 4 , jefe.getY()));
				}
				else if (nivel == 4) 
				{
					Balas.push_back(Bala(jefe.getX() - 1 , jefe.getY()));
					Balas.push_back(Bala(jefe.getX() + 1 , jefe.getY()));
				}
			}
			disparo = 100;
		}
		else 
		{
			if (nivel == 2)	disparo = rand() % 160;			//El nivel de la oruga
			else if (nivel == 4) disparo = rand() % 120;	//La reina roja tiene más probabilidades de disparar
		}

	 	jefe.update(prota);
	}

	if (prota.getvida() <= 0) game_over = true;

	//Si alicia pasa por la puerta del laberinto (posicion solo posible en niveles de laberinto)
	int y = prota.getY();
	int x = prota.getX();
	if ((y == 36||y == 37) && x == 117) cambio_nivel(); 

	//Si derrota aL Jefe cambia nivel
	if ((nivel == 2 || nivel == 4) && jefe.getvida() == 0) 
	{
		if (charla_2) cutscene_2(nivel);
		cambio_nivel();
	}
}

//* ///////////////////////////////////////////////////////////////////////
void draw(int alto, int ancho)
{
	erase();
	int y, x;

	if (nivel == 1|| nivel == 3)					//Dibujo de las paredes del laberinto, para nivel 1 y 3
	{
		if (nivel == 1) attron(COLOR_PAIR(PAR_PARED));		
		if (nivel == 3)	attron(COLOR_PAIR(PAR_CONEJO));
		dibujar_laberinto(ALTO, ANCHO);

		bool llave (gato.getY()==300); 	//Para desbloquear la puerta (x) hay que agarrar los 5 gatos 
		if (!llave)						//Al ser agarrado se posiciona en 300 x, 300 y.
		{
			mvvline(36,118, 'x', 2);	//Puerta
		};
		if (nivel == 1) attroff(COLOR_PAIR(PAR_PARED));
		if (nivel == 3)	attroff(COLOR_PAIR(PAR_CONEJO));

		if (nivel == 1) attron(COLOR_PAIR (PAR_FLORES));
		if (nivel == 3) attron(COLOR_PAIR (PAR_CORAZONES));

		for (int i = 0; i<Enemigos.size(); i++) Enemigos[i].draw(nivel);	

		if (nivel == 1) attroff(COLOR_PAIR(PAR_FLORES));
		if (nivel == 3) attroff(COLOR_PAIR (PAR_CORAZONES));

		if (charla_2) cutscene_2(nivel);
		if (charla) cutscene(nivel);

		attron(COLOR_PAIR (PAR_GATO));
		gato.draw();					//Gato de cheshire, hay que recolectarlo
		attroff(COLOR_PAIR (PAR_GATO));
	}
	else if (nivel == 2 || nivel == 4)				//Mapa 2 y 4 nivel
	{	
		//AGUA
		if (nivel == 2)
		{
			attron(COLOR_PAIR (PAR_ORUGA));
			mvhline(1, 1, '~', 118);

			for(int i = 2; i < 35 ; i++)
			{
				mvhline(i, 1, '~', 22);				
			}

			for(int i = 2; i < 39 ; i++)
			{
				mvhline(i, 97, '~', 22);				
			}
			attroff(COLOR_PAIR ( PAR_ORUGA));
		}
		if (nivel == 4)
		{
			attron(COLOR_PAIR (PAR_CORAZONES));
			mvhline(1, 1, '.', 118);

			for(int i = 2; i < 35 ; i++)
			{
				mvhline(i, 1, '.', 22);				
			}

			for(int i = 2; i < 39 ; i++)
			{
				mvhline(i, 97, '.', 22);				
			}
			attroff(COLOR_PAIR ( PAR_CORAZONES));
		}
		if (nivel == 2)attron(COLOR_PAIR (PAR_PARED));
		if (nivel == 4)	attron(COLOR_PAIR(PAR_CONEJO));
		
		//HORIZONTALES
		mvhline(2, 24, ACS_HLINE, 72);
		mvhline(35, 2, ACS_HLINE, 21);
		mvhline(38, 2, ACS_HLINE, 94);

		//VERTICALES
		mvvline ( 3, 23, ACS_VLINE, 32);
		mvvline ( 3, 96, ACS_VLINE, 35);

		//ESQUINAS Y UNIONES
		mvaddch ( 2, 23, ACS_ULCORNER);
		mvaddch ( 35, 1, ACS_ULCORNER);
		mvaddch ( 38, 1, ACS_LLCORNER);
		mvaddch ( 2, 96, ACS_URCORNER);
		mvaddch ( 38, 96, ACS_LRCORNER);
		mvaddch ( 35, 23, ACS_LRCORNER);
		
		//PUERTA
		bool puerta (prota.getX() > 24);			//Cuando Alicia sale del pasillo, este se cierra
		if (puerta)	mvvline (36, 23, 'x', 2);
		
		if (nivel == 2)attroff(COLOR_PAIR (PAR_PARED));
		if (nivel == 4)	attroff(COLOR_PAIR(PAR_CONEJO));

		if (charla && puerta) cutscene(nivel);		//Pasado el pasillo Alicia habla con el Jefe

		//Balas y proyectiles del jefe y alicia
		for (int i = 0; i < Proyectiles.size(); i++) Proyectiles[i].draw(); 
		for (int i = 0; i < Balas.size(); i++) Balas[i].draw(nivel);

		if (nivel == 2) attron(COLOR_PAIR (PAR_ORUGA));
		if (nivel == 4) attron(COLOR_PAIR (PAR_CORAZONES));
		jefe.draw();
		if (nivel == 2) attroff(COLOR_PAIR (PAR_ORUGA));
		if (nivel == 4) attroff(COLOR_PAIR (PAR_CORAZONES));
	}

	//BORDES
	box(stdscr, 0, 0);

	//VIDA ALICIA
	mvprintw(39, 20, "[ VIDAS:            ]");

	wchar_t w_vida = L'\u2592';
	cchar_t vida;
	setcchar(&vida, &w_vida, A_NORMAL, 0, nullptr);

	attron (COLOR_PAIR(PAR_ALICIA));
	for (int j = 0; j < prota.getvida(); j++)
	{
		mvadd_wch(39, 29 + j, &vida);
	}
	attroff (COLOR_PAIR(PAR_ALICIA));
	//VIDA JEFES
	//ORUGA
	if (nivel  == 2)
	{	
		wchar_t w_flor = L'\u2698';
		cchar_t flor;
		setcchar(&flor, &w_flor, A_NORMAL, 0, nullptr);
		mvprintw(0, 90, "[ VIDAS:      ]");
		attron (COLOR_PAIR(PAR_ORUGA));
		for (int i = 0; i < jefe.getvida(); i++)
		{
			mvadd_wch(0 , 99 + i, &flor);
		}
		attroff (COLOR_PAIR(PAR_ORUGA));
	}
	//REINA
	if( nivel == 4)
	{
		mvprintw(0, 90, "[ VIDAS:         ]");

		wchar_t w_corona = L'\u265B';
		cchar_t corona;
		setcchar(&corona, &w_corona, A_NORMAL, 0, nullptr);
		attron (COLOR_PAIR(PAR_CORAZONES));
		for (int i = 0; i < jefe.getvida(); i++)
		{
			mvadd_wch(0 , 99 + i, &corona);
		}
		attroff (COLOR_PAIR(PAR_CORAZONES));
	}

	attron(COLOR_PAIR (PAR_ALICIA));
	prota.draw();	
	attroff(COLOR_PAIR(PAR_ALICIA));
	
	refresh();
	delay_output(DELAY);
}

//* ///////////////////////////////////////////////////////////////////////
void gameover()
{
	nodelay(stdscr, false);
	for (int y = 10; y < 16; y++)
	mvhline(y, 40, ' ', 40);

	mvaddch(9, 39, ACS_ULCORNER);
	mvaddch(9, 80, ACS_URCORNER);
	mvaddch(16, 39, ACS_LLCORNER);
	mvaddch(16, 80, ACS_LRCORNER);

	mvhline(9, 40, ACS_HLINE, 40);
	mvhline(16, 40, ACS_HLINE, 40);

	mvvline(10, 39, ACS_VLINE, 6);
	mvvline(10, 80, ACS_VLINE, 6);

	mvprintw(12, 55, "PERDISTE");
	mvprintw(13, 50, "VOLVER AL MENU? (S/N)");

	int opcion = getch();

	if (opcion == 's' || opcion == 'S')
	{
		nodelay(stdscr, true);
		menu = true;	
		erase();	
	}
	else if (opcion == 'n' || opcion == 'N')
	{
		menu = true;
		salir = true;
	}
}

//* ///////////////////////////////////////////////////////////////////////
void dibujar_laberinto(int alto, int ancho)
{	
		//HORIZONTALES
		mvhline(1, 1, ACS_HLINE, ancho-2);  
		mvhline(5, 24, ACS_HLINE, 9);    
		mvhline(5, 76, ACS_HLINE, 5);     
		mvhline(5, 87, ACS_HLINE, 23);     
		mvhline(8, 48, ACS_HLINE, 16);     
		mvhline(8, 99, ACS_HLINE, 19);     
		mvhline(9, 7, ACS_HLINE, 5);    
		mvhline(9, 32, ACS_HLINE, 17);    
		mvhline(13, 1, ACS_HLINE, 12);      
		mvhline(13, 24, ACS_HLINE, 17);     
		mvhline(13, 63, ACS_HLINE, 4);       
		mvhline(13, 87, ACS_HLINE, 15);      
		mvhline(16, 8, ACS_HLINE, 23);      
		mvhline(16, 40, ACS_HLINE, 37);      
		mvhline(16, 101, ACS_HLINE, 11);   
		mvhline(19, 1, ACS_HLINE, 12);    
		mvhline(21, 26, ACS_HLINE, 15);   
		mvhline(21, 71, ACS_HLINE, 17);     
		mvhline(21, 97, ACS_HLINE, 21);    
		mvhline(23, 1, ACS_HLINE, 19);   
		mvhline(25, 19, ACS_HLINE, 12);   
		mvhline(25, 62, ACS_HLINE, 44);      
		mvhline(29, 11, ACS_HLINE, 20);      
		mvhline(29, 40, ACS_HLINE, 2);       
		mvhline(29, 50, ACS_HLINE, 3);       
		mvhline(29, 62, ACS_HLINE, 16);       
		mvhline(29, 87, ACS_HLINE, 8);       
		mvhline(29, 101, ACS_HLINE, 17);       
		mvhline(32, 1, ACS_HLINE, 18);       
		mvhline(32, 30, ACS_HLINE, 11);       
		mvhline(32, 62, ACS_HLINE, 9);       
		mvhline(32, 94, ACS_HLINE, 8);       
		mvhline(35, 9, ACS_HLINE, 3);       
		mvhline(35, 18, ACS_HLINE, 13);       
		mvhline(35, 62, ACS_HLINE, 9);       
		mvhline(35, 87, ACS_HLINE, 15);       
		mvhline(35, 108, ACS_HLINE, 10);       
		mvhline(38, 1, ACS_HLINE, ancho-2);  
		
		//VERTICALES
		mvvline(1, 1, ACS_VLINE, 19);   
		mvvline(23, 1, ACS_VLINE, 16);  
		mvvline(26, 11, ACS_VLINE, 4); 
		mvvline(35, 11, ACS_VLINE, 4); 
		mvvline(1, 12, ACS_VLINE, 6);    
		mvvline(9, 12, ACS_VLINE, 5);
		mvvline(16, 12, ACS_VLINE, 4);
		mvvline(32, 18, ACS_VLINE, 4);   
		mvvline(20, 19, ACS_VLINE, 6);   
		mvvline(5, 24, ACS_VLINE, 9);   
		mvvline(16, 26, ACS_VLINE, 6);   
		mvvline(25, 30, ACS_VLINE, 3);   
		mvvline(29, 30, ACS_VLINE, 4);   
		mvvline(5, 32, ACS_VLINE, 5);   
		mvvline(1, 40, ACS_VLINE, 6);   
		mvvline(13, 40, ACS_VLINE, 4);   
		mvvline(21, 40, ACS_VLINE, 9);   
		mvvline(32, 40, ACS_VLINE, 5);   
		mvvline(8, 48, ACS_VLINE, 6);   
		mvvline(4, 52, ACS_VLINE, 5);   
		mvvline(21, 52, ACS_VLINE, 18);   
		mvvline(12, 57, ACS_VLINE, 5);   
		mvvline(1, 62, ACS_VLINE, 6);   
		mvvline(16, 62, ACS_VLINE, 14);   
		mvvline(32, 62, ACS_VLINE, 4);   
		mvvline(8, 63, ACS_VLINE, 6);   
		mvvline(35, 70, ACS_VLINE, 4);   
		mvvline(18, 71, ACS_VLINE, 6);   
		mvvline(5, 76, ACS_VLINE, 12);   
		mvvline(29, 77, ACS_VLINE, 10);   
		mvvline(1, 87, ACS_VLINE, 21);   
		mvvline(25, 87, ACS_VLINE, 11);   
		mvvline(18, 97, ACS_VLINE, 4);   
		mvvline(13, 101, ACS_VLINE, 4);   
		mvvline(29, 101, ACS_VLINE, 4);   
		mvvline(34, 101, ACS_VLINE, 2);   
		mvvline(8, 106, ACS_VLINE, 6);   
		mvvline(21, 106, ACS_VLINE, 2);   
		mvvline(13, 111, ACS_VLINE, 4);   
		mvvline(1, 118, ACS_VLINE, 34);   

		//ESQUINAS Y UNIONES
		mvaddch(1, 1, ACS_ULCORNER);
		mvaddch(1, 118, ACS_URCORNER);
		mvaddch(38, 1, ACS_LLCORNER);
		mvaddch(38, 118, ACS_LRCORNER);

		mvaddch(1, 12, ACS_TTEE);
		mvaddch(1, 40, ACS_TTEE);
		mvaddch(1, 62, ACS_TTEE);
		mvaddch(1, 87, ACS_TTEE);
		mvaddch(8, 106, ACS_TTEE);
		mvaddch(16, 12, ACS_TTEE);
		mvaddch(16, 26, ACS_TTEE);
		mvaddch(16, 62, ACS_TTEE);
		mvaddch(21, 106, ACS_TTEE);
		mvaddch(25, 87, ACS_TTEE);

		mvaddch(5, 87, ACS_LTEE);
		mvaddch(13, 1, ACS_LTEE);
		mvaddch(13, 87, ACS_LTEE);
		mvaddch(21, 71, ACS_LTEE);
		mvaddch(25, 62, ACS_LTEE);
		mvaddch(29, 87, ACS_LTEE);
		mvaddch(32, 1, ACS_LTEE);

		mvaddch(8, 118, ACS_RTEE);
		mvaddch(9, 48, ACS_RTEE);
		mvaddch(21, 118, ACS_RTEE);
		mvaddch(23, 19, ACS_RTEE);
		mvaddch(29, 52, ACS_RTEE);
		mvaddch(29, 118, ACS_RTEE);
		mvaddch(35, 118, ACS_RTEE);
		
		mvaddch(8, 52, ACS_BTEE);
		mvaddch(16, 57, ACS_BTEE);
		mvaddch(38, 11, ACS_BTEE);
		mvaddch(38, 52, ACS_BTEE);
		mvaddch(38, 70, ACS_BTEE);
		mvaddch(38, 77, ACS_BTEE);
		
		mvaddch(5, 24, ACS_ULCORNER);
		mvaddch(5,76, ACS_ULCORNER);
		mvaddch(8, 48, ACS_ULCORNER);
		mvaddch(23, 1, ACS_ULCORNER);
		mvaddch(29, 101, ACS_ULCORNER);
		mvaddch(32, 62, ACS_ULCORNER);

		mvaddch(5, 32, ACS_URCORNER);
		mvaddch(8, 63, ACS_URCORNER);
		mvaddch(9, 12, ACS_URCORNER);
		mvaddch(13, 40, ACS_URCORNER);
		mvaddch(13, 101, ACS_URCORNER);
		mvaddch(21, 40, ACS_URCORNER);
		mvaddch(25, 30, ACS_URCORNER);
		mvaddch(29, 30, ACS_URCORNER);
		mvaddch(29, 77, ACS_URCORNER);
		mvaddch(32, 18, ACS_URCORNER);
		mvaddch(32, 40, ACS_URCORNER);
		mvaddch(35, 11, ACS_URCORNER);
		mvaddch(35, 70, ACS_URCORNER);

		mvaddch(9, 32, ACS_LLCORNER);
		mvaddch(13, 24, ACS_LLCORNER);
		mvaddch(13, 63, ACS_LLCORNER);
		mvaddch(16, 40, ACS_LLCORNER);
		mvaddch(16, 101, ACS_LLCORNER);
		mvaddch(19, 1, ACS_LLCORNER);
		mvaddch(21, 26, ACS_LLCORNER);
		mvaddch(21, 97, ACS_LLCORNER);
		mvaddch(25, 19, ACS_LLCORNER);
		mvaddch(29, 11, ACS_LLCORNER);
		mvaddch(29, 40, ACS_LLCORNER);
		mvaddch(29, 62, ACS_LLCORNER);
		mvaddch(32, 30, ACS_LLCORNER);
		mvaddch(35, 18, ACS_LLCORNER);
		mvaddch(35, 62, ACS_LLCORNER);
		mvaddch(35, 87, ACS_LLCORNER);

		mvaddch(13, 12, ACS_LRCORNER);
		mvaddch(16, 76, ACS_LRCORNER);
		mvaddch(16, 111, ACS_LRCORNER);
		mvaddch(19, 12, ACS_LRCORNER);
		mvaddch(21, 87, ACS_LRCORNER);
		mvaddch(32, 101, ACS_LRCORNER);
		mvaddch(35, 101, ACS_LRCORNER);
		mvaddch(38, 118, ACS_LRCORNER);
}

//* ///////////////////////////////////////////////////////////////////////
void cutscene (int nivel)
{	
	int x = 0;
	nodelay(stdscr, false);
	switch (nivel)
	{
		case 1:
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Despues de descender por la madriguera, Alicia se encontro en un ");
			mvprintw(12, 25, "extraño jardin subterraneo. Las flores aquí no eran como las de su jardín.");
			mvprintw(13, 25, "Sus tallos se retorcian como si estuvieran vivos, extendiendose lentamente hacia ella.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);

//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Deberia tener cuidado de las flores...");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");

			//DIBUJO ALICIA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}

			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(2000);

//* //////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Mientras avanzaba su corazón latía con fuerza.");
			mvprintw(12, 25, "Podía sentir que algo más la observaba desde entre las hojas…");
			mvprintw(13, 25, "O quizás eran las hojas mismas las que la observaban.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);

//* //////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "El suelo bajo sus pies parecía inestable y, por primera vez,");
			mvprintw(12, 25, "una duda cruzó por su mente:");
			mvprintw(13, 25, "¿Había hecho bien en seguir al Conejo?");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);

//* //////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "De repente una silueta violeta se materializo delante de ella");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);			
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_GATO));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¡Alicia, querida!");
			mvprintw(12, 25, "Si quires encontrar al conejo deberas seguir la sonrisa.");
			mvprintw(13, 25, "En las sombras más escondidas deberas buscar.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO GATO
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_gato[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_GATO));
			getch();
			delay_output(5000);
	
			break;
		case 2:
			//Narrador
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Al salir del laberinto Alicia se encontró en un enorme espacio silencioso.");
			mvprintw(12, 25, "No había señales del Conejo Blanco.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(5000);

//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ORUGA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "\"No solo hay flores peligrosas aqui\" Susurró una voz distorsionada en su mente,");
			mvprintw(12, 25, "como si proviniera de las sombras que ahora la rodeaban");
			mvprintw(13, 25, "Una enorme Oruga azul bostezaba y abria los ojos.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO ORUGA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_oruga[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_ORUGA));
			getch();
			delay_output(5000);
			
//* //////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "La Oruga y Alicia se miraron un rato en silencio.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);
			
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ORUGA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¿Quien eres tu?");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO ORUGA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_oruga[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_ORUGA));
			getch();
			delay_output(2000);
			
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Pues yo.., yo, ahora mismo, Oruga, no lo se... ");
			mvprintw(12, 25, "Si se quien era cuando esta mañana me levante, ");
			mvprintw(13, 25, "pero he debido de cambiar varias veces desde entonces.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");

			//DIBUJO ALICIA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}

			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(6000);
					
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ORUGA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Ya no importa, te voy a destruir…");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO ORUGA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_oruga[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_ORUGA));
			getch();
			delay_output(3000);
	
			break;
		case 3:
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Tras vencer a la Orugas, Alicia sintió que algo no estaba bien.");
			mvprintw(12, 25, "El aire era denso y opresivo, y, de reojo, ");
			mvprintw(13, 25, "creyó ver figuras que se movían entre las sombras.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);

//* //////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Escuchaba pasos y ruidos metálicos.");
			mvprintw(12, 25, "¿Cartas?, se preguntó. ");
			mvprintw(13, 25, "Pero cuando giraba la cabeza, no había nada.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);

//* //////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¿Alguien la estaba siguiendo? ¿O estaba perdiendo el control?");
			mvprintw(12, 25, "Su respiración se aceleró, y ya no sabía si corría por su vida ");
			mvprintw(13, 25, "o huía de sus propios pensamientos.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);

//* //////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Ahora había llegado a otra entrada,  un nuevo laberinto...");
			mvprintw(12, 25, " y el Conejo Blanco seguía sin aparecer.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);
	
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_GATO));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¡Por aquí, Alicia!");
			mvprintw(12, 25, "Sigamos jugando a las escondidas.");
			mvprintw(12, 25, "(Se rie mientras solo queda su sonrisa en el aire)");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO GATO
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_gato[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_GATO));
			getch();
			delay_output(5000);
			break;
		case 4:

			attron(COLOR_PAIR(PAR_CORAZONES));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Has llegado lejos, Alicia, pero tu viaje termina aqui. ");
			mvprintw(12, 25, "Creiste que podias escapar, pero no puedes huir de lo que eres.");
			mvprintw(13, 25, "Todo este laberinto, estas sombras y ecos… son reflejos de ti misma.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO REINA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_reina[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_CORAZONES));
			getch();
			delay_output(6000);
				
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "No te temo.");
			mvprintw(12, 25, "He enfrentado mis miedos, mis dudas, mis propias sombras.");
			mvprintw(13, 25, "No puedes controlarme.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");

			//DIBUJO ALICIA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}

			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(5000);
					
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_CORAZONES));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¿Controlarte? ¡Ja! Yo no necesito controlarte.");
			mvprintw(12, 25, "Tu ya estas atrapada aquí. ");
			mvprintw(13, 25, "¿De verdad crees que vencerme te salvara?");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO REINA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_reina[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_CORAZONES));
			getch();
			delay_output(6000);

//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_CORAZONES));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "No hay salida de este laberinto, Alicia. Tu ya estas atrapada aquí. ");
			mvprintw(12, 25, "Cada giro, cada puerta que has atravesado...");
			mvprintw(13, 25, "solo te ha llevado más profundo dentro de ti misma.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO REINA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_reina[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_CORAZONES));
			getch();
			delay_output(5000);
						
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Si este laberinto es parte de mi, entonces tengo el poder para destruirlo.");
			mvprintw(12, 25, "No necesito una salida...");
			mvprintw(13, 25, "Solo tengo que enfrentarme a ti.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");

			//DIBUJO ALICIA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}

			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(5000);
	
			break;
		
		default:
			break;
	}
	charla = false;
	nodelay(stdscr, true);
	
}

//* ///////////////////////////////////////////////////////////////////////
void cutscene_2(int nivel)
{
	int x = 0;
	nodelay(stdscr, false);
	switch (nivel)
	{
		case 1:
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "En un día ocioso, Alicia descansaba en su jardin, cuando un Conejo Blanco,");
			mvprintw(12, 25, "vestido con chaleco rojo y un reloj, paso corriendo y altero su tranquilidad.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);
//* ///////////////////////////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "\"¡Llego tarde, llego tarde!\", murmuraba con urgencia mientras corria");
			mvprintw(12, 25, "y miraba su reloj. Fascinada por lo absurdo de la situacion,");
			mvprintw(13, 25, "Alicia decidio seguir al Conejo.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);
//* ///////////////////////////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "A medida que corría tras el, el jardín ");
			mvprintw(12, 25, "a su alrededor comenzó a desvanecerse");
			mvprintw(13, 25, "y la realidad se torno borrosa.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);
//* ///////////////////////////////////////////////////////////////////////			
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Los colores vibrantes de las flores se mezclaban como en un sueño");
			mvprintw(12, 25, "y los arboles parecian moverse a su alrededor.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);
//* ///////////////////////////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Alicia observo al Conejo adentrandose en una madriguera,");
			mvprintw(12, 25, "y sin pensarlo dos veces, se apuro a seguirlo.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);
//* ///////////////////////////////////////////////////////////////////////
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¿Qué secretos esconde este misterioso lugar?");
			mvprintw(12, 25, "¿Te atreves a acompañar a Alicia en su búsqueda del Conejo Blanco?");
			mvprintw(15, 65, "Presione cualquier tecla para continuar");

			getch();
			delay_output(2000);
//* ///////////////////////////////////////////////////////////////////////
			break;	
		case 2:
			attron(COLOR_PAIR(PAR_ORUGA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Ah... al final, me has derrotado. Pero dime, Alicia...");
			mvprintw(12, 25, "¿Quién crees que eres ahora?");
			mvprintw(13, 25, "¿Crees que has cambiado al vencerme?");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO ORUGA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_oruga[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_ORUGA));
			getch();
			delay_output(5000);
			
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "No lo sé... Sigo sin entender muchas cosas.");
			mvprintw(12, 25, "Pero sé que ya no puedo quedarme quieta, ");
			mvprintw(13, 25, "esperando respuestas que nunca llegan.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");

			//DIBUJO ALICIA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}

			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(6000);

//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ORUGA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "El cambio no siempre trae respuestas, niña. A veces, solo trae más preguntas.");
			mvprintw(12, 25, "Sigue avanzando pero, recuerda, no siempre serás quien crees ser.");
			mvprintw(13, 25, "Y tal vez... eso sea lo único que debas saber.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO ORUGA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_oruga[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_ORUGA));
			getch();
			delay_output(6000);
			
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Quizas no tenga todas las respuestas, pero seguiré.");
			mvprintw(12, 25, "No puedo detenerme ahora.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");

			//DIBUJO ALICIA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}

			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(5000);
					
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ORUGA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Oh, Alicia... sigues creyendo que avanzar te llevara a algun lugar.");
			mvprintw(13, 25, "Veremos cuánto tiempo más puedes sostener esa ilusion...");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO ORUGA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_oruga[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_ORUGA));
			getch();
			delay_output(5000);
	
			break;
		case 4:
			
			attron(COLOR_PAIR(PAR_CORAZONES));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¡Que insensatez, niña desalmada! ¡Vencerme a mi, la Reina de Corazones,");
			mvprintw(12, 25, "es como pedirle a un reloj que deje de hacer tic-tac!");
			mvprintw(13, 25, "Y tu... tu eres solo el eco de un bostezo mal dormido.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO REINA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_reina[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_CORAZONES));
			getch();
			delay_output(6000);
				
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¡¡Ah!! Pero sigo aqui. Me has perseguido con cartas y orugas azules,");
			mvprintw(12, 25, "pero aun no me has quebrado. Este lugar... no tiene poder sobre mi.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");

			//DIBUJO ALICIA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}

			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(5000);
					
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_CORAZONES));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¿Poder? ¡Alicia! Este laberinto no es mio, nunca lo ha sido.");
			mvprintw(12, 25, "Todo lo que has sentido, lo que te ha mordido en los talones...");
			mvprintw(13, 25, "viene de ti.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO REINA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_reina[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_CORAZONES));
			getch();
			delay_output(5000);

//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Pero tu haces los juegos. ¿No?");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO ALICIA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(5000);

//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_CORAZONES));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¡Juegos!  Este mundo es el reflejo de tu propio caos.");
			mvprintw(12, 25, "Las tazas saltan porque tu les has dado alas.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO REINA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_reina[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_CORAZONES));
			getch();
			delay_output(5000);
						
//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_ALICIA));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "Entonces... ¿nada es real?");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO alicia
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_alicia[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_ALICIA));
			getch();
			delay_output(3000);

//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_CORAZONES));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "¿Real? ¿Que es real, Alicia? Los sapos cantores,");
			mvprintw(12, 25, "las rosas parlanchinas, el queso que huye de su propio olor...");
			mvprintw(13, 25, "Todo esta dentro de ti.");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO REINA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_reina[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_CORAZONES));
			getch();
			delay_output(5000);

//* //////////////////////////////////////////////////
			attron(COLOR_PAIR(PAR_CORAZONES));
			for (int y = 10; y < 16; y++)
			mvhline(y, 0, ' ', 120);
			mvhline(9, 0, ACS_HLINE, 120);
			mvhline(16, 0, ACS_HLINE, 120);

			mvprintw(11, 25, "No puedes huir de lo que eres, a menos que tengas zapatos muy rapidos.");
			mvprintw(12, 25, "Bienvenida, Alicia, a tu propio mundo de locura");
			mvprintw(15, 65, "Presione cualquier tecla para continuar.");
			//DIBUJO REINA
			for (int i = 0; i < 7; i++)       
			{
			mvprintw(10 + i, 5, d_reina[i].c_str());
			}
			attroff(COLOR_PAIR(PAR_CORAZONES));
			getch();
			delay_output(5000);
			break;
		default:
			break;
	}
	charla_2 = false;
	nodelay(stdscr, true);
}

//* ///////////////////////////////////////////////////////////////////////
void final()
{	
	erase();
	nodelay(stdscr, false);
    for (int y = 10; y < 16; y++)
    mvhline(y, 0, ' ', 120);

    mvhline(9, 0, ACS_HLINE, 120);
    mvhline(16, 0, ACS_HLINE, 120);
    

    mvprintw(11, 25, "Tras derrotar a la Reina de Corazones, el laberinto se transforma.");
    mvprintw(12, 25, "Las paredes se retuercen, y las sombras danzan ");
	mvprintw(13, 25, "como si fueran sus pensamientos más oscuros.");
    mvprintw(15, 65, "Presione cualquier tecla para continuar");
    
    getch();
    delay_output(5000);

	//* ///////////////////////////////////////////////////////////////////////
    for (int y = 10; y < 16; y++)
    mvhline(y, 0, ' ', 120);

    mvhline(9, 0, ACS_HLINE, 120);
    mvhline(16, 0, ACS_HLINE, 120);
    

    mvprintw(11, 25, "Alicia avanza, pero cada paso la devuelve al principio,");
    mvprintw(12, 25, "atrapada en un ciclo interminable.");
    mvprintw(15, 65, "Presione cualquier tecla para continuar");
    
    getch();
    delay_output(5000);

	//* ///////////////////////////////////////////////////////////////////////
    for (int y = 10; y < 16; y++)
    mvhline(y, 0, ' ', 120);

    mvhline(9, 0, ACS_HLINE, 120);
    mvhline(16, 0, ACS_HLINE, 120);
    

    mvprintw(11, 25, "El Gato de Cheshire aparece, flotando con su característica sonrisa.");
    mvprintw(15, 65, "Presione cualquier tecla para continuar");
    
    getch();
    delay_output(2000);

	//* ///////////////////////////////////////////////////////////////////////
	attron (COLOR_PAIR(PAR_GATO));
	for (int y = 10; y < 16; y++)
    mvhline(y, 0, ' ', 120);

    mvhline(9, 0, ACS_HLINE, 120);
    mvhline(16, 0, ACS_HLINE, 120);
    
    for (int i = 0; i < 7; i++)  mvprintw(10 + i, 5, d_gato[i].c_str());

    mvprintw(11, 25, "Bravo, Alicia. Venciste a la Reina, pero...");
    mvprintw(12, 25, "¿De verdad pensaste que podías huir?");
    mvprintw(15, 65, "Presione cualquier tecla para continuar");

    attroff (COLOR_PAIR(PAR_GATO));
    getch();
    delay_output(4000);

	//* ///////////////////////////////////////////////////////////////////////
	attron (COLOR_PAIR(PAR_GATO));
	for (int y = 10; y < 16; y++)
    mvhline(y, 0, ' ', 120);

    mvhline(9, 0, ACS_HLINE, 120);
    mvhline(16, 0, ACS_HLINE, 120);
    
    for (int i = 0; i < 7; i++)  mvprintw(10 + i, 5, d_gato[i].c_str());

    mvprintw(11, 25, "El laberinto es tu mente, querida.");
    mvprintw(12, 25, "Y la mente... no tiene puertas de salida, solo espejos que se rien.");
    mvprintw(15, 65, "Presione cualquier tecla para continuar");

    attroff (COLOR_PAIR(PAR_GATO));
    getch();
    delay_output(4000);
	//* //////////////////////////////////////////////////
	attron(COLOR_PAIR(PAR_ALICIA));
	for (int y = 10; y < 16; y++)
	mvhline(y, 0, ' ', 120);
	mvhline(9, 0, ACS_HLINE, 120);
	mvhline(16, 0, ACS_HLINE, 120);

	mvprintw(11, 25, "¡Esto no tiene sentido!");
	mvprintw(15, 65, "Presione cualquier tecla para continuar.");

	//DIBUJO ALICIA
	for (int i = 0; i < 7; i++) mvprintw(10 + i, 5, d_alicia[i].c_str());

	attroff(COLOR_PAIR(PAR_ALICIA));
	getch();
	delay_output(2000);

	//* ///////////////////////////////////////////////////////////////////////
	attron (COLOR_PAIR(PAR_GATO));
	for (int y = 10; y < 16; y++)
    mvhline(y, 0, ' ', 120);

    mvhline(9, 0, ACS_HLINE, 120);
    mvhline(16, 0, ACS_HLINE, 120);
    
    for (int i = 0; i < 7; i++)  mvprintw(10 + i, 5, d_gato[i].c_str());

    mvprintw(11, 25, "Nada tiene mas sentido que esto. ");
    mvprintw(12, 25, "¡Y que maravilla de enredo has creado!");
    mvprintw(15, 65, "Presione cualquier tecla para continuar");

    attroff (COLOR_PAIR(PAR_GATO));
    getch();
    delay_output(4000);

	//* ///////////////////////////////////////////////////////////////////////
    for (int y = 10; y < 16; y++)
    mvhline(y, 0, ' ', 120);

    mvhline(9, 0, ACS_HLINE, 120);
    mvhline(16, 0, ACS_HLINE, 120);
    

    mvprintw(11, 25, "El laberinto se cierra a su alrededor.");
	mvprintw(12, 25, "Alicia queda atrapada , mientras la risa del Gato resuena eternamente en el aire");
    mvprintw(15, 65, "Presione cualquier tecla para continuar");
    
    getch();
    delay_output(2000);
	
	//* ///////////////////////////////////////////////////////////////////////
	erase();
	attron (COLOR_PAIR(PAR_ALICIA));
   	for (int y = 10; y < 16; y++)
	mvhline(y, 40, ' ', 40);

	mvaddch(9, 39, ACS_ULCORNER);
	mvaddch(9, 80, ACS_URCORNER);
	mvaddch(16, 39, ACS_LLCORNER);
	mvaddch(16, 80, ACS_LRCORNER);

	mvhline(9, 40, ACS_HLINE, 40);
	mvhline(16, 40, ACS_HLINE, 40);

	mvvline(10, 39, ACS_VLINE, 6);
	mvvline(10, 80, ACS_VLINE, 6);

	mvprintw(12, 47, "TERMINASTE EL JUEGO");
	mvprintw(13, 50, "VOLVER AL MENU? (S/N)");
	attroff (COLOR_PAIR(PAR_ALICIA));
	int opcion = getch();

	if (opcion == 's' || opcion == 'S')
	{
		nodelay(stdscr, true);
		game_over = true;
		menu = true;	
		fin = true;
		erase();	
	}
	else if (opcion == 'n' || opcion == 'N')
	{
		game_over = true;
		menu = true;
		salir = true;
	}
		
}