#pragma once
#include <iostream>
#include <ncurses.h>
#include "Jefe.h"

using namespace std;

class Proyectil
{
public:
  Proyectil(int x, int y, int nivel);
  
  void update();
  void draw();

  bool limite();
  bool colision(Jefe &rJefe);

  int getX();
  int getY();

private:
  int m_x;
  float m_y;
  int cual;
};