#pragma once
#include <iostream>
#include <ncurses.h>

class Alicia
{
public:
    
  void setup();
  void draw();
  void update();
  
  void setX(int x);
  void setY(int y);
  
  int getX();
  int getY();

  void setvida(int vida);
  int getvida();

private:
  int m_x, m_y;
  int m_vida;
  bool m_muerte;
};
