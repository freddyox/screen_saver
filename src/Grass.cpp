#include "../include/Grass.hh"
#include <random>

Grass::Grass(sf::Vector2f pos, int n, float l,float w, sf::Color col) :
  fOrigin(pos), fNpts(n), fL(l), fW(w), fRandColor(col) {
  if( fNpts < 4 ) fNpts = 4;
  if( fNpts > 15 ) {
    std::cout << "Grass blades are being built with more than 15 pts which is unneccessary FYI."
	      << " Setting it to 10." << std::endl;
    fNpts = 10;
  }
  fdL = fL / fNpts;
  fGradFact = 0.8;
  deg = 180.0 / 3.141592;
  // Set up the blade characterists:
  std::random_device rdev{}; 
  std::default_random_engine fRandGen{rdev()};
  float a0 = 270.0; // deg
  std::normal_distribution<double> fGaus(a0, 4.0);

  // working in rads now
  a0 /= deg;
  fAngle = fGaus(fRandGen)/deg;
  fDir = (fAngle - a0) > 0.0 ? 1 : 0; // which direction is the bias
  fdAngle = a0-fAngle; // offset relative to starting position
  fItsDark= false;
  fDarkFlag = false;
  fTempTime = 0.0;
  fDarkenTime = 0.05;
  fTime = 0.0;
  add_gradient();
  create();
}

Grass::~Grass(){;}

void Grass::create(){
  sf::Vector2f start = fOrigin;
  sf::Vector2f end(start.x + fdL*cos(fAngle), start.y + fdL*sin(fAngle));
  Box b0(start, end, fW, fGradFact);

  b0.SetFillColor(fColor[0]);
  fGrassBox.clear();
  fGrassBlade.clear();
  fGrassBox.push_back(b0);
  fGrassBlade.push_back( b0.GetBox() ); // starting position to get the ball rolling
  for(int i=1; i<fNpts; i++){
    float ang = fAngle + 2.0*i*fdAngle; 
    sf::Vector2f prev_final = fGrassBox[i-1].GetFinal(); // end pos of Box
    start = prev_final;
    end = start +  sf::Vector2f(fdL*cos(ang), fdL*sin(ang));
    
    if( i==fNpts-1 ) fGradFact = 0.01; // End the grass blade
    
    Box b(start,end,fGrassBox[i-1].TopWidth(), fGradFact);
    b.SetFillColor(fColor[i]);
    fGrassBox.push_back(b);
    fGrassBlade.push_back(b.GetBox()); // starting position to get the ball rolling
  }
}

void Grass::add_gradient(){
  sf::Color c1(0,101,0);
  sf::Color c2 = fRandColor;
  for(int i=0; i<fNpts; i++){
    float t = float(i) / float(fNpts-1);
    float newr = fabs( c1.r + ( c2.r - c1.r ) * t );
    float newg = fabs( c1.g + ( c2.g - c1.g ) * t );
    float newb = fabs( c1.b + ( c2.b - c1.b ) * t );
    float rmin = newr/5.0;
    float gmin = newg/5.0;
    float bmin = newb/5.0;
    fMinColor.push_back( sf::Color(int(rmin),int(gmin),int(bmin)) );
    fColor.push_back( sf::Color(int(newr),int(newg),int(newb)) );
  }
}

void Grass::simulate(float dt, bool darkness, float offset){  
  fItsDark = darkness;
  fTime += dt;
  fWind = generate_perlin(fTime+offset,0.0,0.0);
  float ang = 10.*3.1415/180.0;
  for(unsigned int i=0; i<fGrassBlade.size(); i++){
    float weight = 0.15 * i;
    sf::Vector2f windvec(weight*fWind*cos(ang),weight*fWind*sin(ang));
    for(int v=1; v<3; v++){
      fGrassBlade[i][v].position += windvec;
    }
    if(i<fGrassBlade.size()-1){
      fGrassBlade[i+1][0].position += windvec;
      fGrassBlade[i+1][3].position += windvec;
    }
  }
  if(fItsDark){
    fTempTime += dt;
    if( fTempTime > fDarkenTime ){
      reduce_color();
      fTempTime = 0.0;    
    }
  }
  if( !fItsDark ){
    fTempTime += dt;
    if( fTempTime > fDarkenTime ){
      increase_color();
      fTempTime = 0.0;
    }
  }
}

// simulate darkness
void Grass::reduce_color(){
  for(unsigned int i=0; i<fGrassBlade.size(); i++){
    for(int v=0; v<4; v++){
      sf::Color temp = fGrassBlade[i][v].color;
      int newr = temp.r - 1;
      int newg = temp.g - 1;
      int newb = temp.b - 1;
      if(newr<=fMinColor[i].r) newr = fMinColor[i].r;
      if(newg<=fMinColor[i].g) newg = fMinColor[i].g;
      if(newb<=fMinColor[i].b) newb = fMinColor[i].b;
      sf::Color newcol(newr,newg,newb);
      fGrassBlade[i][v].color = newcol;
    }
  }
}

// Put the color back to original
void Grass::increase_color(){
  for(unsigned int i=0; i<fGrassBlade.size(); i++){
    for(int v=0; v<4; v++){
      sf::Color temp = fGrassBlade[i][v].color;
      int newr = temp.r + 1;
      int newg = temp.g + 1;
      int newb = temp.b + 1;
      if(newr>=fColor[i].r) newr = fColor[i].r;
      if(newg>=fColor[i].g) newg = fColor[i].g;
      if(newb>=fColor[i].b) newb = fColor[i].b;
      sf::Color newcol(newr,newg,newb);
      fGrassBlade[i][v].color = newcol;
    }
  }
}
