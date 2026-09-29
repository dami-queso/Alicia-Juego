#pragma once
#include <iostream>
#include <ncurses.h>
#include "Alicia.h"

using namespace std;

class Enemigo
{
public:

  Enemigo (int y, int x, int z);

  void update();
  void draw(int nivel);
  void colision(Alicia &rAlicia);

  int getX();
  int getY();

private:
  float m_x, m_y;
  int m_z;
  float movimiento = 0.1; 
};