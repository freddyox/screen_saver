#ifndef BACKGROUND_HH
#define BACKGROUND_HH
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "../include/Bug.hh"

// Generic class to cycle through a list of images under some time constraint
class Background : public sf::Drawable{
private:
  float fX, fY;
  sf::Sprite fBackground;    // The drawable
  sf::Texture fTexture;      // The middle-man object 
  double fTime, fTempTime;   // Control time
  float fTime_to_Change_Bkgrd; // The time to wait until a new image is brought in
  float fTime_to_Change_Bkgrd_Const;
  
  // structure is fName.fID.ppm where fID must be 4 ints (e.g. 0000 or 0054,
  // where the fID = 54 in the latter example)
  int fID;                   // Keep track of the image number (for file names)
  int fN;                    // Max number of images (so we do not get lost)
  std::string fName;         // name of image -

  bool fItsDark;
  sf::CircleShape fStar,fStarBig;
  std::vector<sf::CircleShape> fStars;
  float fAngle,fAngleMoon;
  std::vector<sf::CircleShape> fMoon;
  
  std::vector<Bug*> fBugs;
  int fNBugs;
  
public:
  Background(std::string, int, float, float, float);
  ~Background();
  void draw( sf::RenderTarget&, sf::RenderStates) const;
  sf::String make_name(int);
  void simulate(float);
  void import_image(int);
  void generate_stars(int);
  void move_stars(float);
  void move_moon();
  bool IsItDark(){ return fItsDark; }
  sf::Vector2f get_moon_pos(){return fMoon[fMoon.size()-1].getPosition();}
};
#endif
