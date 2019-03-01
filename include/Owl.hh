#ifndef OWL_HH
#define OWL_HH
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <string>

class Owl : public sf::Transformable{
private:
  float fX, fY;
  float fTime, fChangeSprite, fTempTime, fAlphaTime, fAlphaTimeTemp;
  std::vector<sf::Sprite> fOwlMoves;
  std::map<int,std::vector<sf::Sprite> > fOwlMovesMap;
  std::map<std::string,int> fOrientation;
  sf::Texture fOwlSheet;
   
  int fDirIndex,fIndex;
  bool fItsDark;
  float fScale;
  sf::Sound fHoot;
  sf::SoundBuffer buffer;
  bool fHootOnce;
public:
  Owl();
  ~Owl();
  sf::Sprite* GetOwl(){return &fOwlMovesMap[fDirIndex][fIndex];}
  int N(){return fOwlMoves.size();}
  void update(float,sf::Vector2f,bool,sf::Vector2f);
  void setx(float x){fX=x;}
  void sety(float y){fY=y;}
  void hoot(){fHoot.play();}
};
#endif
