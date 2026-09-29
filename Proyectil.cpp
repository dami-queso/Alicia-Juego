#include "Proyectil.h"
//* ///////////////////////////////////////////////////////////////////////
Proyectil::Proyectil(int x, int y, int nivel)
{
  m_x = x;
  m_y = y;
 // cual = nivel;
}

//* ///////////////////////////////////////////////////////////////////////
void Proyectil::update()  { if (m_y >= 1) m_y -= 0.4f; }

//* ///////////////////////////////////////////////////////////////////////
void Proyectil::draw()    { mvaddch(m_y, m_x, ACS_UARROW); }

//* ///////////////////////////////////////////////////////////////////////
bool Proyectil::limite()
{
  if( m_y < 3 ) return true;
  return false;
}

//* ///////////////////////////////////////////////////////////////////////
bool Proyectil::colision(Jefe &rJefe)
{
  //if (cual == 2)
  //{
    if (m_x <= rJefe.getX() + 4 && m_x >= rJefe.getX() && m_y <= rJefe.getY()) 
    {
    rJefe.setvida(rJefe.getvida() - 1);
    return true; 
    }
  //}
  //else if (cual == 4)
  //{
  //  if (m_x <= rJefe.getX() + 1 && m_x >= rJefe.getX() - 1 && m_y <= rJefe.getY())  rJefe.setvida(rJefe.getvida() - 1);
  //  return true;
  //}
  else return false;
}

//* ///////////////////////////////////////////////////////////////////////
int Proyectil::getX() { return m_x; }
int Proyectil::getY() { return m_y; }
