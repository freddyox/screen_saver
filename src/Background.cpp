#include "../include/Background.hh"
#include <stdio.h>
#include <random>

Background::Background(std::string s, int n, float c, float x, float y) :
  fName(s), fN(n), fTime_to_Change_Bkgrd(c), fX(x), fY(y) {
  fID = 0;
  // initialize
  fTime_to_Change_Bkgrd_Const = fTime_to_Change_Bkgrd;
  fTime = 0.0;
  fTempTime = 0.0;
  import_image(fID);
  fStar.setRadius(1.0);
  fStar.setFillColor(sf::Color::White);
  sf::FloatRect box = fStar.getLocalBounds();
  fStar.setOrigin( 0.5*box.width, 0.5*box.height );
  generate_stars(100);
  fAngle = 110.0 * 3.141592 / 180.0;
  fAngleMoon = 100.0 * 3.141592 / 180.0;
  fItsDark = false;
  int N = 100;
  float R = 500.0;
  float dR = R/(N-1);
  int alpha = 1;
  for(int i=0; i<N; i++){
    float Rtemp = R - dR*i;
    if( i%25==0 ) alpha++;
    if(i==N-1){
      alpha=255;
      Rtemp=35.0;
    }
    sf::CircleShape fMoons(Rtemp);
    fMoons.setFillColor(sf::Color(253,255,175,alpha));
    sf::FloatRect box = fMoons.getLocalBounds();
    fMoons.setOrigin( 0.5*box.width, 0.5*box.height );
    fMoons.setPosition(-R,500);
    fMoon.push_back( fMoons );
  }

  fNBugs = 50;
  for(int i=0; i<fNBugs; i++){
    Bug *bug = new Bug(fX,fY);
    fBugs.push_back( bug );
  }
}

Background::~Background(){
  ;
}

void Background::draw( sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(fBackground);
  if(fItsDark ){
    for(unsigned int i=0; i<fMoon.size(); i++) target.draw(fMoon[i]);
  }
  for(unsigned int i=0; i<fStars.size(); i++) target.draw(fStars[i]);
  for(unsigned int i=0; i<fBugs.size(); i++) {
    for(int j=0; j<2; j++) target.draw((fBugs[i]->get_bug())[j]); // the 2 is hard-coded and will NOT change.
  }
}

sf::String Background::make_name(int n){
  char numstr[21]; 
  sprintf(numstr, "%04d", n);
  sf::String out = "images/" + fName + "." + numstr + ".ppm";
  return out;
}

void Background::import_image(int n){
  sf::String name = make_name(n);
  std::cout << "changing to " << name.toAnsiString()
	    << " time,temp time = " << fTime << ", " << fTempTime << std::endl;
  if( !fTexture.loadFromFile(name.toAnsiString()) ){
    std::cout << "ERROR: " << fName << " did not open properly..." << std::endl;
  }
  fBackground.setTexture( fTexture ); 
}

void Background::simulate(float dt){
  fTime += dt;
  fTempTime += dt;
  if( fTempTime > fTime_to_Change_Bkgrd ){
    import_image( fID++ );
    fTempTime = 0.0;
  }
  if( fID==110 && !fItsDark) {
    std::cout << "Dark flag! - " << fID << std::endl;
    fItsDark=true;
  }
  if( fID==153 && fItsDark ) {
    fItsDark = false;
    std::cout << "Light flag! - " << fID << std::endl; 
  }
  // Let's hold the 132nd frame longer for more darkness
  if( fID == 142 ) fTime_to_Change_Bkgrd = 40.0;
  if( fID == 143 ) fTime_to_Change_Bkgrd = fTime_to_Change_Bkgrd_Const;
    
  if( fID>=fN ) fID = 0;
  move_stars(dt);
}

void Background::generate_stars(int n){
  std::random_device rdev{};  // seed the C++11 generator
  std::default_random_engine fRandGen{rdev()};
  std::uniform_int_distribution<int> fFlatX(0,fX);
  std::uniform_int_distribution<int> fFlatY(0,500);
  std::normal_distribution<double> fGaus(2.3,1.0); // 0 to 5 degrees
  std::normal_distribution<double> fGausRed(190,20); // 0 to 5 degrees
  std::normal_distribution<double> fGaus1(200.0,50.0); // 0 to 5 degrees
  for(int i=0; i<n; i++){
    float radius = fGaus(fRandGen);
    if( radius < 0.5 ) radius = 1.0;
    float alpha = fGaus1(fRandGen);
    if( alpha<100.0 || alpha > 255 ) alpha = 200;
    fStar.setRadius(radius);
    float xtemp = fFlatX(fRandGen);
    float ytemp = fFlatY(fRandGen);
    fStar.setPosition(sf::Vector2f(xtemp,ytemp));
    alpha -= ytemp/500.0*50.0;
    fStar.setFillColor(sf::Color(255,255,255,alpha));
    if(radius>3.4){
      int gausred = int(fGausRed(fRandGen));
      fStar.setFillColor(sf::Color(255,gausred,gausred,alpha));
    }
    fStars.push_back(fStar);
  }
}

void Background::move_stars(float dt){
  sf::Vector2f dx(sin(fAngle),cos(fAngle));
 
  for(unsigned int i=0; i<fStars.size(); i++){
    sf::Vector2f x = fStars[i].getPosition();
    float starR = fStars[i].getRadius();
    float fact = starR * 10.0;
    sf::Vector2f dxhat(dx.x/fact, dx.y/fact); 
    sf::Vector2f newpos = x+dxhat;
    if( newpos.x > fX+starR  || newpos.y > fY+starR ||
	newpos.x < -starR    || newpos.y < -starR ){
      std::random_device rdev{};  // seed the C++11 generator
      std::default_random_engine fRandGen{rdev()};
      std::uniform_int_distribution<int> fFlat(0,100);
      std::uniform_int_distribution<int> fFlatY(0,500);
      newpos = sf::Vector2f(fFlat(fRandGen), fFlatY(fRandGen) );
    }
    fStars[i].setPosition(newpos);
  }
  move_moon();
  for(unsigned int i=0; i<fBugs.size(); i++){
    fBugs[i]->update(dt,fItsDark);
  }
}

void Background::move_moon(){
  float fac = 0.4 / fTime_to_Change_Bkgrd_Const;
  sf::Vector2f dx_moon(fac*sin(fAngleMoon), fac*cos(fAngleMoon));
  if( fItsDark ){
    for(unsigned int i=0; i<fMoon.size(); i++){
      fMoon[i].move( dx_moon );
    }
  }
  // 0 index is the largest 
  if(fMoon[0].getPosition().x > fX + fMoon[0].getRadius() ){
    for(unsigned int i=0; i<fMoon.size(); i++){
      fMoon[i].setPosition( sf::Vector2f(-fMoon[0].getRadius()*0.6,500) );
    }
  }
}
