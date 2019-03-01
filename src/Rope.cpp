#include "../include/Rope.hh"
#include "../include/GenerateForce.hh"
#include "../include/Box.hh"

#include <iostream>
#include <cmath>

Rope::Rope(int N, float m, float k, float l, float mu, sf::Vector2f g,
	   float air, float grepulse, float gmu, float gabs, float gheight) :
  fNMasses(N), fMassWeight(m), fSpringConst(k), fSpringLength(l),
  fSpringFriction(mu), fGravity(g), fAirFrictionConst(air),
  fGroundRepulsionConst(grepulse), fGroundFrictionConst(gmu),
  fGroundAbsorptionConst(gabs), fGroundHeight(gheight) {

  fAnchorRight = false;
  fTime = 0.0;

  // fWeight.setRadius( 10.0 );
  // sf::FloatRect box = fWeight.getLocalBounds();
  // fWeight.setOrigin( 0.5*box.width, 0.5*box.height );
  // fWeight.setFillColor( sf::Color::Red );
  
  //fRopeNode = sf::VertexArray(sf::Lines,2);
  fRopeNode = sf::VertexArray(sf::Quads,4);
  fColor = sf::Color::Black;
  fThickness = 5.0;
}

Rope::~Rope(){
  // need to destroy the things created within this class
  // for( std::vector<Spring*>::iterator it=fSprings.begin(); it!=fSprings.end(); it++){
  //   delete (*it);
  // }
  // for( std::vector<Mass*>::iterator it=fMasses.begin(); it!=fMasses.end(); it++){
  //   delete (*it);
  // }
  // fSprings.clear();
  // fMasses.clear();
}

void Rope::move_left_anchor(sf::Vector2f pos){
  sf::Vector2f D = pos - fLeftRopeConnect;
  float R = sqrt( D.x*D.x + D.y*D.y );
  if( R<30.0 ) fLeftRopeConnect = pos;
}

sf::VertexArray* Rope::create(sf::Vector2f left, sf::Vector2f right,
			      bool anchor ){
  fLeftRopeConnect = left;
  fRightRopeConnect = right;
  fAnchorRight = anchor;

  sf::Vector2f temp = fRightRopeConnect - fLeftRopeConnect;
  float mag = sqrt( temp.x*temp.x + temp.y*temp.y );
  sf::Vector2f hat(temp.x/mag, temp.y/mag);
  if( fAnchorRight ) {
    fRopeNormal = hat;
    float requested_length = (fNMasses-1.0) * fSpringLength;
    std::cout << "Requested spring length = " << fSpringLength << std::endl;
    std::cout << "Anchored spring length  = " <<mag/(fNMasses-1.0) << std::endl;
    fSpringLength = mag / (fNMasses-1.0);
  } else {
    fRopeNormal = sf::Vector2f(1.0,0.0);
  }
  create_masses();
  create_springs();
  return &fRope[0];
}

void Rope::create_masses(){
  for(int i=0; i<fNMasses; i++) {
    float fact = 1.0;
    sf::Vector2f pos = fLeftRopeConnect + i*fSpringLength*fRopeNormal;
    if( i==fNMasses-1 && !fAnchorRight ) {
      fact = 5.0;
      fLastPos = pos;
    }
    Mass *mass_temp = new Mass(fact*fMassWeight,i);
    mass_temp->setpos(pos);
    //fWeight.setPosition(pos);
    fMasses.push_back( mass_temp );
  }
}

void Rope::create_springs(){
  // 0----0----0 where 0==mass, ---==spring
  // therefore, we need one less spring than masses
  for(int i=0; i<fNMasses-1; i++){
    Spring *spring_temp = new Spring(fMasses[i], fMasses[i+1], fSpringConst,
				     fSpringLength, fSpringFriction);
    fSprings.push_back( spring_temp );

    sf::Vector2f pos_n = fLeftRopeConnect + i*fSpringLength*fRopeNormal;
    sf::Vector2f pos_n1= fLeftRopeConnect + (i+1)*fSpringLength*fRopeNormal;
    Box rope_box(pos_n,pos_n1,fThickness,1.0);
    rope_box.SetFillColor(fColor);
    fRope.push_back( rope_box.GetBox() );
    
    // fRopeNode[0].position = pos_n;
    // fRopeNode[1].position = pos_n1;
    // fRopeNode[0].color = fColor;
    // fRopeNode[1].color = fColor;
    // fRope.push_back( fRopeNode );
  }
}

void Rope::solve(){
  for(int i=0; i<fNMasses; i++) fMasses[i]->reset_force();
  
  for(int i=0; i<fNMasses-1; i++) {
    fSprings[i]->solve();
  }

  for(int i=0; i<fNMasses; i++){
    fMasses[i]->applyForce(  sf::Vector2f(10.0*fWind,0.0) );
    fMasses[i]->applyForce(  fMasses[i]->mass() * fGravity );
    fMasses[i]->applyForce( -fMasses[i]->vel()  * fAirFrictionConst );
  }
}

void Rope::simulate(float dt){
  fTime += dt;
  fWind = generate_perlin(fTime,0.0,0.0);
  
  solve();
  for(int i=0; i<fNMasses; i++){
   fMasses[i]->simulate(dt);
  }

  fMasses[0]->setpos(fLeftRopeConnect);
  fMasses[0]->setvel(sf::Vector2f(0.0,0.0));
  if( fAnchorRight ) {
    fMasses[fMasses.size()-1]->setpos(fRightRopeConnect);
    fMasses[fMasses.size()-1]->setvel(sf::Vector2f(0.0,0.0));
  }
  
  //fWeight.setPosition( fMasses[fMasses.size()-1]->pos() );
  fLastPos =  fMasses[fMasses.size()-1]->pos();
  fMidPos = fMasses[(fMasses.size()-1)/2]->pos();
  
  if( fMasses.size() <= 10 ) {
    std::cout << "MOST LIKELY WILL GET SEG FAULT IN SIMULATE ROUTINE" 
	      << " DUE TO HARDCODED VECTOR ACCESS!!!!!!" << std::endl;
  }
  sf::Vector2f D = fMasses[fMasses.size()-1]->pos() - fMasses[fMasses.size()-10]->pos();
  float mag = sqrt( D.x*D.x + D.y*D.y );
  fLastNhat = sf::Vector2f(D.x/mag, D.y/mag);
  
  for(int i=0; i<fNMasses-1; i++){
    sf::Vector2f left = fMasses[i]->pos();
    sf::Vector2f right = fMasses[i+1]->pos();
    
    Box rope_box(left,right,fThickness,1.0);
    rope_box.SetFillColor(fColor);
    fRope[i] = rope_box.GetBox();
  }
  //fLeftRopeConnect.x += fRopeConnectVel.x * dt;
  //fLeftRopeConnect.y += fRopeConnectVel.y * dt;
}

double Rope::fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
