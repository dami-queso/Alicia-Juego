#pragma once
#include <iostream>
#include <vector>
#include <ncurses.h>
#include "Alicia.h"
#include "Bala.h"

class Jefe
{
public:
    
  void setup(int nivel);
  void draw();
  void update(Alicia & rAlicia);
  
  void setX(int x);
  void setY(int y);

  void setvida(int vida);
  
  int getX();
  int getY();

  int getvida();

private:
  float m_x, m_y;
  int m_vida;
  bool m_muerte;
  int cual;
  
};