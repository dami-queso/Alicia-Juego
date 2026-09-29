#include "Enemigo.h"

//* ///////////////////////////////////////////////////////////////////////
Enemigo::Enemigo(int y, int x, int z) 
{
  m_x = x;
  m_y = y;
  m_z = z;                            // 1 = HORIZONTAL, 2 = VERTICAL
}

//* ///////////////////////////////////////////////////////////////////////
void Enemigo::update()
{
  chtype ch;
  if (m_z == 2)                       // 2 = VERTICAL
  {
    m_y = m_y + movimiento;           //Se mueve
    ch = mvinch(m_y + movimiento, m_x) &A_CHARTEXT; //ve si puede moverse
  }
  else if (m_z == 1)                  // 1 = HORIZONTAL
  {
    m_x = m_x + movimiento;           //Se mueve
    ch = mvinch(m_y , m_x + movimiento) &A_CHARTEXT;//ve si puede moverse
  }
    
  if ( ch  == (ACS_HLINE &A_CHARTEXT)
    || ch == (ACS_VLINE &A_CHARTEXT)
    || ch== (ACS_ULCORNER &A_CHARTEXT)
    || ch== (ACS_LLCORNER &A_CHARTEXT)
    || ch== (ACS_URCORNER &A_CHARTEXT)
    || ch== (ACS_LRCORNER &A_CHARTEXT)
    || ch== (ACS_LTEE &A_CHARTEXT)
    || ch== (ACS_RTEE &A_CHARTEXT)
    || ch== (ACS_BTEE &A_CHARTEXT)
    || ch== (ACS_TTEE &A_CHARTEXT)) 
    movimiento *= (-1);               //Colisiona y cambia de direccion
}

//* ///////////////////////////////////////////////////////////////////////
void Enemigo::draw(int nivel)
{ wchar_t w_enemigo;

  if (nivel == 1)  w_enemigo = L'\u273F';
  else if (nivel == 3) w_enemigo = L'\u2665';
  cchar_t enemigo;
  setcchar(&enemigo, &w_enemigo, A_NORMAL, 0, nullptr);
  mvadd_wch (m_y, m_x, &enemigo);
 // mvaddch (m_y, m_x, '*');
}

//* ///////////////////////////////////////////////////////////////////////
void Enemigo::colision( Alicia &rAlicia )
{ 
  if (m_x >= rAlicia.getX() - 1  && m_x <= rAlicia.getX() + 1 && m_y <= rAlicia.getY() + 0.9f && m_y >= rAlicia.getY() - 0.01f) //area de colision
  {
    rAlicia.setvida(rAlicia.getvida() - 1);
    if( m_z == 1) m_x += (movimiento * 30);     
    else          m_y += (movimiento * 10);
  }
}

//* ///////////////////////////////////////////////////////////////////////
int Enemigo::getX() { return m_x; }
int Enemigo::getY() { return m_y; }
