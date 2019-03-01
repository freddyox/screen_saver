#ifndef BUG_HH
#define BUG_HH
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
class Bug{
private:
  float fX, fY;
  std::vector<sf::CircleShape> fBug; // it will contain two circles
  float fMoveTime, fChangeMovement;  // generate some fundamental random time for flickering
  float fFlickTime, fChangeFlick;
  bool fFlicked;
  float fFlickDuration;
  float fAngle;
  float fTime;
  
public:
  Bug(float,float);
  ~Bug();
  void update(float,bool); // this will contain all the movement info
  sf::CircleShape* get_bug(){return &fBug[0];}
  int N(){ return fBug.size(); }
};
#endif
