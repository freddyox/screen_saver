#ifndef SPRING_HH
#define SPRING_HH

#include <SFML/Graphics.hpp>
#include "../include/Mass.hh"

class Spring{
private:
  Mass* fM1, *fM2;
  float fSpringConst, fSpringLength, fSpringFriction;
  
public:
  Spring(Mass*,Mass*,float,float,float);
  ~Spring();
  void solve();
};
#endif
