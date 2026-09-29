#pragma once
#include <iostream>
#include <ncurses.h>
#include "Alicia.h"


using namespace std;

class Bala
{
public:
  Bala(int x, int y);
  
  void update();
  void draw(int nivel);
  
  bool limite();
  bool colision(Alicia &rAlicia);

  int getX();
  int getY();

private:
  int m_x;
  float m_y;
  bool borrar = false; 
  
};