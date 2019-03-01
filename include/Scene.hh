#ifndef SCENE_HH
#define SCENE_HH
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "../include/Rope.hh"
#include "../include/Tree.hh"
#include "../include/Grass.hh"
#include "../include/Owl.hh"
#include <vector>

class Scene : public sf::Drawable, public sf::Transformable {
private:
  float fX, fY;

  Rope *fVine, *fSwing;
  Tree *fTreeL, *fTreeR;
  sf::VertexArray *fvVine,  *fvSwing;
  sf::VertexArray *fvTreeL, *fvTreeR;
  std::vector<sf::VertexArray*> fvTrees;
  std::vector<int> fNSmallTrees;
  int fNMasses,fVineMasses;
  int fSwingAnch, fLeftVineAnch, fRightVineAnch;

  sf::Texture fTireSwingText;
  sf::Sprite fTireSwing;
  double fTime;
  
  int fNGrass;
  std::vector<Grass*> fGrass;
  bool fIsItDark;

  Owl *fOwl;
  sf::Sound fCrickets;
  sf::SoundBuffer buffer;
  float fVol;
  
public:
  Scene(float,float);
  ~Scene();
  void update(float,bool,sf::Vector2f);
  void draw( sf::RenderTarget&, sf::RenderStates) const;
};
#endif
