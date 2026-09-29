#include "Alicia.h"

void Alicia::setup()
{
  //Inicializar al costado
  m_x = 2;
  m_y = 21;

  m_vida = 10;      //Serían 3 pero hay problema con las colisiones de las balas, quitan mucha vida.
  m_muerte = false;
}

void Alicia::update()
{
    //vida baja
  if (m_vida <= 0)
	{
      m_muerte = true;
    }
}

void Alicia::draw()
{
    if(!m_muerte)
    {
    mvaddch(m_y, m_x + 1, '}');
    mvaddch(m_y, m_x, 'o');
    mvaddch(m_y, m_x - 1, '{');
    }
  else
  {
    mvaddch(m_y, m_x + 1, ' ');
    mvaddch(m_y, m_x, ' ');
    mvaddch(m_y, m_x - 1, ' ');  
    
    refresh();
    delay_output(200);

    mvaddch(m_y, m_x + 1, '-');
    mvaddch(m_y, m_x, '-');
    mvaddch(m_y, m_x - 1, '-');

    refresh();
    delay_output(200);
    
    m_muerte = false;
  }
}

void Alicia::setX(int x) { m_x = x; }
void Alicia::setY(int y) { m_y = y; }
int Alicia::getX() { return m_x; }
int Alicia::getY() { return m_y; }
void Alicia::setvida(int vida) { m_vida = vida; }
int Alicia::getvida() { return m_vida; }