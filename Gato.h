#pragma once
#include <iostream>
#include <ncurses.h>
#include <string>
#include "Alicia.h"

using namespace std;

class Gato
{
public:
  void setup();  
  void draw();
  void update();
  
  int getX();
  int getY();

  void colision(Alicia &rAlicia, int nivel);

  string frases [10] [2]
  {
    {"Falta mucho pata llegar al Conejo Blanco, Alicia.", "¿Escuchas esos pasos, Alicia? ¿O solo los imaginas?"},
    {"Mira que esto es un laberinto muy largo, con muchos peligros.", "Quizás hay algo siguiéndote… o quizás solo está en tu cabeza."},
    {"Sigue al Conejo si te atreves, pero recuerda,","Mira bien, Alicia. ¿Esas sombras tienen forma de carta… o son solo juegos de luz?"},
    {"no todo lo que persigues es real.","Quizás nunca lo sabrás."},
    {"El Conejo se esconde en las sombras,","Las cartas, sí, siempre vigilantes. Pero dime, ¿las has visto realmente?"},
    {"pero este no es el unico secreto en el camino.","A veces, lo que más tememos es lo que creamos nosotros mismos."},
    {"Cuanto mas profundo desciendes, Alicia,","Puede que te estén persiguiendo, o puede que no. Pero te diré un secreto:"},
    {"mas dificil es volver. ¿Estas segura de seguir?","La mente es un laberinto mucho más peligroso que cualquier carta."},
    {"Ahora ya hay vuelta atrás, toma esta llave para seguir.","No tengas miedo, Alicia. Si son reales, te atraparán. Y si no lo son…"},
    {"Espero volver a verte pronto. (el gato se rie)","Entonces ya te han atrapado de todos modos. Toma la llave, la Reina te espera."}
  };
  string dibujo [7]
  {
    "  |\\________/|",
    "  {  @    @  }",
    " ={  \\__I__/ }=",
    "  {   \\WWW/  }",
    "  {          }",
    " {           }",
    "{             }"
  };

private:
  int m_x, m_y;
  int cheshire; //contador
  int cual;     //nivel
};