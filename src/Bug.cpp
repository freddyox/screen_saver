#include "../include/Bug.hh"

Bug::Bug(float x, float y) : fX(x), fY(y){
  sf::CircleShape bug(1.5);
  bug.setFillColor(sf::Color::Black);
  sf::FloatRect box = bug.getLocalBounds();
  bug.setOrigin( 0.5*box.width, 0.5*box.height );

  sf::CircleShape glow(3.5);
  glow.setFillColor(sf::Color(0,0,0,0));
  box = glow.getLocalBounds();
  glow.setOrigin( 0.5*box.width, 0.5*box.height );

  std::random_device rdev{};  // seed the C++11 generator
  std::default_random_engine fRandGen{rdev()};
  std::uniform_int_distribution<int> fFlatX(0.1*fX,0.9*fX);
  std::uniform_int_distribution<int> fFlatY(550,0.9*fY);
  sf::Vector2f pos(fFlatX(fRandGen), fFlatY(fRandGen));
  std::normal_distribution<double> fGaus(7.0,4.0);
  std::normal_distribution<double> fGaus1(2.0,0.5);
  
  glow.setPosition( pos );
  bug.setPosition( pos );
  fBug.push_back( glow );
  fBug.push_back( bug );

  fMoveTime = 0.0;
  fChangeMovement = fGaus1(fRandGen);
  fFlickTime = 0.0;
  fChangeFlick = fGaus(fRandGen);
  if( fChangeFlick < 0.3 ) fChangeFlick = 1.0;
  fFlickDuration = 0.5;
  fFlicked = false;
  std::uniform_int_distribution<int> randang(1,360);
  fAngle = randang(fRandGen) * 3.141592 / 180.0;
}

Bug::~Bug(){;}

void Bug::update(float dt, bool dark){
  fTime += dt;
  fMoveTime += dt;
  fFlickTime += dt;
  std::random_device rdev{};  // seed the C++11 generator
  std::default_random_engine fRandGen{rdev()};
  if( fMoveTime > fChangeMovement ){
    std::uniform_int_distribution<int> randang(1,360);
    fAngle = randang(fRandGen) * 3.141592 / 180.0;
    fMoveTime = 0.0;
    std::normal_distribution<double> fGaus1(1.0,0.5);
    fChangeMovement = fGaus1(fRandGen);
  }
  std::normal_distribution<double> fGaus2(0.5,0.1);
  float vx = fGaus2(fRandGen);
  float vy = fGaus2(fRandGen);
  sf::Vector2f pos(vx*cos(fAngle), vy*sin(fAngle) );
  fBug[0].move(pos);
  fBug[1].move(pos);

  if( fFlickTime > fChangeFlick ){
    fBug[0].setFillColor(sf::Color(216,254,29,100));
    fBug[1].setFillColor(sf::Color(216,254,29));
    fFlickTime = 0.0;
    if( !dark ) {
      std::normal_distribution<double> fGaus(7.0,2.0);
      fChangeFlick = fGaus(fRandGen);
    } else {
      std::normal_distribution<double> fGaus(3.0,0.5);
      fChangeFlick = fGaus(fRandGen);
    }
    if( fChangeFlick < 0.05 ) fChangeFlick = 0.3;
    fFlicked = true;
  }
  // This should be OK since fFlickDuration is longer time scale
  if(fFlicked && fFlickTime > fFlickDuration ){
    fBug[0].setFillColor(sf::Color(0,0,0,0));
    fBug[1].setFillColor(sf::Color::Black);
    fFlicked = false;
    std::normal_distribution<double> fGaus(0.4,0.1);
    fFlickDuration = fGaus(fRandGen);
  }
}
