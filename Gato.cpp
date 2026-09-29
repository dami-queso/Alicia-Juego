#include "Gato.h"

void Gato::setup() 
{
  m_y = 36;
  m_x = 5;
  cheshire = 0; //contador
}

//* ///////////////////////////////////////////////////////////////////////
void Gato::update()
{
  switch (cheshire)
  {
  case 1:
    m_y = 33;
    m_x = 64;
    break;
    case 2:
    m_y = 6;
    m_x = 27;
    break;
    case 3:
    m_y = 3;
    m_x = 89;
    break;
    case 4:
    m_y = 27;
    m_x = 64;
    break;
    case 5:
    m_y = 300;
    m_x = 300;
    break;
  default:

    break;
  }
}

//* ///////////////////////////////////////////////////////////////////////
void Gato::draw()
{
    mvaddch(m_y, m_x + 1, 'O');
    mvaddch(m_y, m_x, 'W');
    mvaddch(m_y, m_x - 1, 'O');
};

//* ///////////////////////////////////////////////////////////////////////
void Gato::colision( Alicia &rAlicia, int nivel ) 
{
  if (nivel == 1) cual = 0;
  if (nivel == 3) cual = 1;
  //Al encontrarse con Alicia el contador sube y el gato habla
  if(m_x >= rAlicia.getX() - 2  && m_x <= rAlicia.getX() + 2 && m_y == rAlicia.getY())
  {
   
   nodelay(stdscr, false);
   for (int y = 10; y < 16; y++)
    mvhline(y, 0, ' ', 120);

    mvhline(9, 0, ACS_HLINE, 120);
    mvhline(16, 0, ACS_HLINE, 120);
    
    for (int i = 0; i < 7; i++)       //Dibujo de gato
    {
      mvprintw(10 + i, 5, dibujo[i].c_str());
    }

    mvprintw(11, 25, frases[cheshire + cheshire][cual].c_str());
    mvprintw(12, 25, frases[cheshire + cheshire + 1][cual].c_str());

    mvprintw(15, 65, "Presione cualquier tecla para continuar");
    
    getch();
    delay_output(6000);

    cheshire ++;
    nodelay(stdscr, true);
  }
}

//* ///////////////////////////////////////////////////////////////////////
int Gato::getX() { return m_x; }
int Gato::getY() { return m_y; }
