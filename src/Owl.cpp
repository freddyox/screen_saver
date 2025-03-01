#include "../include/Owl.hh"
#include <cmath>

Owl::Owl(){
  if (!fOwlSheet.loadFromFile("images/Bird_Owl_noBG.png")) {
    std::cout << "Owl sheet not found..." << std::endl;
  }
    
  fChangeSprite = 0.1; // sec
  fTime = 0.0;
  fTempTime = 0.0;
  fAlphaTime = 0.25;
  fAlphaTimeTemp = 0.0;
  fHootOnce = true;
  
  // Make it easy to read
  fOrientation["facing"]   = 0;
  fOrientation["left"]     = 1;
  fOrientation["farleft"]  = 2;
  fOrientation["awayleft"] = 3;
  fOrientation["away"]     = 4;
  fOrientation["misc"]     = 5;
  fScale = 0.0;
  int ncol = 8;
  int nrow = 6;
  int dcol = 760 / ncol; // hard-coded dimensions of the texture
  int drow = 587 / nrow;
  for(int col=0; col<ncol; col++){
    for(int row=0; row<nrow; row++){
      if(row==nrow-1 && col==5) break; // sprite ends here
      int left = col*dcol;
      int top = row*drow;
      sf::Sprite test(fOwlSheet,sf::IntRect(left,top,dcol,drow));
      test.setColor(sf::Color(150,150,150,200)); // initialize to zero
      test.setScale(0.0,0.0);
      sf::FloatRect box = test.getLocalBounds();
      test.setOrigin(0.5*box.width,0.5*box.height);
      fOwlMovesMap[row].push_back( test );
      if(row==0) fOwlMoves.push_back( test );
    }
  }
  fIndex = 0;
  fDirIndex=0;

  if (!buffer.loadFromFile("owl.wav")){
    std::cout << "Error with the owl sounds" << std::endl;
  }
  fHoot.setBuffer(buffer);
  fHoot.setVolume(50);
}

Owl::~Owl(){
  ;
}

void Owl::update(float dt, sf::Vector2f pos, bool dark, sf::Vector2f moonpos){
  fTime += dt;
  fTempTime += dt;
  fAlphaTimeTemp += dt;
  
  fItsDark = dark;
  
  sf::Vector2f D = pos-moonpos;
  float R = sqrt( D.x*D.x + D.y*D.y );
  if( fItsDark ){
    for(unsigned int i=0; i<fOwlMovesMap[fDirIndex].size(); i++) {
      if( moonpos.x > 0.2*fX && moonpos.x<0.5*fX){
	if(fAlphaTimeTemp>fAlphaTime) {
	  fDirIndex=0;
	  fScale+=0.03;
	  fAlphaTimeTemp = 0.0;
	  if(fScale>1.0) fScale=1.0;
	}
      }
      
      if( moonpos.x > 0.5*fX && moonpos.x < 0.55*fX && fHootOnce){
	fHoot.play();
	fHootOnce = false;
      }
      if( moonpos.x>0.55*fX) fHootOnce = true;
	
      if( moonpos.x > 0.6*fX ){
	if(fAlphaTimeTemp>fAlphaTime) {
	  fDirIndex=4;
	  fScale-=0.02;
	  fAlphaTimeTemp = 0.0;
	  if(fScale<0.1) fScale=0.0;
	}
      }
      fOwlMovesMap[fDirIndex][i].setScale(fScale,fScale);
      fOwlMovesMap[fDirIndex][i].setPosition(sf::Vector2f(pos.x, pos.y-30));
    }
    if( fTempTime > fChangeSprite ){
      fIndex++;
      if( fIndex==fOwlMovesMap[fDirIndex].size() ) fIndex = 0;
      fTempTime = 0.0;
    }
  }
}
