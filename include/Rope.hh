#ifndef ROPE_HH
#define ROPE_HH
#include <SFML/Graphics.hpp>
#include <vector>
#include "../include/Mass.hh"
#include "../include/Spring.hh"

class Rope{
private:
  sf::VertexArray fRopeNode;           // For visualization
  std::vector<sf::VertexArray> fRope;   // visualize the rope in the form of a vertex array

  bool fAnchorRight;
  sf::Vector2f fLeftRopeConnect, fRightRopeConnect, fRopeConnectVel, fRopeNormal;
  sf::Vector2f fGravity;
  std::vector<Spring*> fSprings;
  std::vector<Mass*> fMasses;

  // Rope Parameters
  int fNMasses;                                 // N rope nodes or "masses"
  float fMassWeight;                            // Weight of each mass
  float fGroundRepulsionConst, fGroundFrictionConst, fGroundAbsorptionConst;
  float fGroundHeight, fAirFrictionConst;                    
  float fSpringConst, fSpringLength, fSpringFriction;
  float fWind;            // Wind force
  double fTime;           // keeps track of total time 
  sf::Color fColor;       // Rope color
  float fThickness;       // thickness of the rope
  
  //sf::CircleShape fWeight; // Put a mass at the end 
  sf::Vector2f fLastPos;   // Position of the last mass
  sf::Vector2f fLastNhat;  // normal vector between n and n-1 masses (needed for sprite)
  sf::Vector2f fMidPos;    // Position of the Middle Mass
public:
  Rope(int, float, float, float, float, sf::Vector2f,
       float, float, float, float, float);
  ~Rope();
  double fRand(double,double);
  //void draw(sf::RenderTarget&, sf::RenderStates) const;
  void create_masses();
  void create_springs();
  sf::VertexArray* create(sf::Vector2f, sf::Vector2f, bool);
  void solve();
  void simulate(float dt);
  void setropeconnectvel(sf::Vector2f p) {fRopeConnectVel = p; }
  void setleftconnectpos(sf::Vector2f p) {fLeftRopeConnect = p;}
  void setrightconnectpos(sf::Vector2f p){fAnchorRight=true; fRightRopeConnect=p;}
  void move_left_anchor(sf::Vector2f);
  void setcol(sf::Color col){ fColor=col;}
  void set_thickness(float t) {fThickness = t;}
  sf::Vector2f get_last_nhat(){ return fLastNhat;}
  sf::Vector2f get_last_pos(){ return fLastPos; }
  sf::Vector2f get_mid_pos(){ return fMidPos; }
};
#endif
