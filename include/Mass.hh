#ifndef MASS_HH
#define MASS_HH
#include <SFML/Graphics.hpp>

class Mass{
private:
  float fMass;
  sf::Vector2f fPos, fVel, fForce;
  
public:
  int fID;
  
  Mass();
  Mass(float,int);
  ~Mass(){;}

  void applyForce(sf::Vector2f);
  void reset_force();
  void simulate(float);
  void setpos(sf::Vector2f p) {fPos = p;}
  void setvel(sf::Vector2f p) {fVel = p;}
  sf::Vector2f pos(){return fPos;}
  sf::Vector2f vel(){return fVel;}
  sf::Vector2f force(){return fForce;}
  float mass() {return fMass;}
};
#endif
