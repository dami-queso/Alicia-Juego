#include "Bala.h"
//* ///////////////////////////////////////////////////////////////////////
Bala::Bala(int x, int y)
{
  m_x = x;
  m_y = y;
}

//* ///////////////////////////////////////////////////////////////////////
void Bala::update() { if (m_y >= 1) m_y += 0.5f; }

//* ///////////////////////////////////////////////////////////////////////
void Bala::draw(int nivel)
{
  //Origa dispara ° y Reina ❣
  if (nivel == 2) mvaddch(m_y, m_x, ACS_DEGREE); 
  else 
  {
    wchar_t w_balaC = L'\u2763';
    cchar_t balaC;
    setcchar(&balaC, &w_balaC, A_NORMAL, 0, nullptr);
    mvadd_wch(m_y, m_x, &balaC); 
  }
}

//* ///////////////////////////////////////////////////////////////////////
bool Bala::limite()
{
  if( m_y > 38 ) return true;
  return false;
}

//* ///////////////////////////////////////////////////////////////////////
bool Bala::colision(Alicia &rAlicia)
{
  if (m_x >= rAlicia.getX() - 1  && m_x <= rAlicia.getX() + 1 && m_y <= rAlicia.getY() + 0.01f && m_y >= rAlicia.getY() - 0.01f )
  {
    if(!borrar) 
    {
      rAlicia.setvida(rAlicia.getvida() - 1);
      borrar = true;        //Verifica que colisione 1 vez y no reste multiples vidas
    }
    return true;
  }
  return false;
}

//* ///////////////////////////////////////////////////////////////////////
int Bala::getX() { return m_x; }
int Bala::getY() { return m_y; }
