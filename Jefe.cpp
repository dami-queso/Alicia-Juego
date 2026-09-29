#include "Jefe.h"
void Jefe::setup(int nivel)
{
    m_x = 55;
    m_y = 7;
    m_muerte = false;
    cual = nivel;
    if (nivel == 2) m_vida = 5;
    if (nivel == 4) m_vida = 8;
}

//* ///////////////////////////////////////////////////////////////////////
void Jefe::draw()
{   
    if (cual == 2)
    {
        if (!m_muerte)
        {
            mvaddch(m_y, m_x, 'o');
            mvaddch(m_y, m_x + 1, '_');
            mvaddch(m_y, m_x + 2, '_');
            mvaddch(m_y, m_x + 3, ')');
            mvaddch(m_y - 1, m_x + 2, '_');
            mvaddch(m_y - 1, m_x + 1, '(');
            mvaddch(m_y - 2, m_x + 2, '_');
            mvaddch(m_y - 2, m_x + 3, '_');
            mvaddch(m_y - 2, m_x + 4, '_');
        }
        else
        {
            mvaddch(m_y, m_x, 'x');
            mvaddch(m_y, m_x + 1, '-');
            mvaddch(m_y, m_x + 2, '-');
            mvaddch(m_y, m_x + 3, '-');
            mvaddch(m_y - 1, m_x + 2, '-');
            mvaddch(m_y - 1, m_x + 1, '-');
            mvaddch(m_y - 2, m_x + 2, '-');
            mvaddch(m_y - 2, m_x + 3, '-');
            mvaddch(m_y - 2, m_x + 4, '-');
            refresh();
            delay_output(200);
            mvaddch(m_y, m_x, 'x');
            mvaddch(m_y, m_x + 1, ' ');
            mvaddch(m_y, m_x + 2, ' ');
            mvaddch(m_y, m_x + 3, ' ');
            mvaddch(m_y - 1, m_x + 2, ' ');
            mvaddch(m_y - 1, m_x + 1, ' ');
            mvaddch(m_y - 2, m_x + 2, ' ');
            mvaddch(m_y - 2, m_x + 3, ' ');
            mvaddch(m_y - 2, m_x + 4, ' ');
        }
    }
    else if (cual == 4)
    {
        wchar_t w_corazon = L'\u2764';
        cchar_t corazon;
        setcchar(&corazon, &w_corazon, A_NORMAL, 0, nullptr);
            if (!m_muerte)
        {
            mvadd_wch(m_y, m_x, &corazon);
            mvaddch(m_y, m_x + 1, '-');
            mvaddch(m_y, m_x - 1, '-');
            mvaddch(m_y - 1, m_x, '0');
            mvaddch(m_y - 2, m_x, 'W');
            mvaddch(m_y - 2, m_x + 1, '/');
            mvaddch(m_y - 2, m_x - 1, '\\');
        }
        else
        {
            mvaddch(m_y, m_x, 'M');
            mvaddch(m_y, m_x + 1, '-');
            mvaddch(m_y, m_x - 1, '-');
            mvaddch(m_y - 1, m_x, 'x');
            mvaddch(m_y - 2, m_x, '-');
            mvaddch(m_y - 2, m_x + 1, '/');
            mvaddch(m_y - 2, m_x - 1, '\\');
            refresh();
            delay_output(200);
            mvaddch(m_y, m_x, 'M');
            mvaddch(m_y, m_x + 1, '-');
            mvaddch(m_y, m_x - 1, '-');
            mvaddch(m_y - 1, m_x, 'x');
            mvaddch(m_y - 2, m_x,'W') ;
            mvaddch(m_y - 2, m_x + 1, '/');
            mvaddch(m_y - 2, m_x - 1, '\\');
        }
    }
}

//* ///////////////////////////////////////////////////////////////////////
void Jefe::update(Alicia &rAlicia)     
{
    //Se mueve siguiendo a Alicia
    if (m_x < rAlicia.getX()+ 0.5 && m_x < 91 )
    {
        m_x += 0.2f;
    }
    else if (m_x > rAlicia.getX() + 0.5 && m_x > 25) 
    {
        m_x -= 0.2f;
    }
    if (m_vida <= 0)
	{
      m_muerte = true;
    }
}

//* ///////////////////////////////////////////////////////////////////////  
void Jefe::setX(int x) {m_x = x;}
void Jefe::setY(int y) {m_y = y;}
void Jefe::setvida(int vida) { m_vida = vida; }
  
//* ///////////////////////////////////////////////////////////////////////
int Jefe::getX() {return m_x;}
int Jefe::getY() {return m_y;}
int Jefe::getvida() {return m_vida;}
