#include "../include/Box.hh"
#include <iostream>
#include <cmath>

Box::Box(sf::Vector2f xo, sf::Vector2f xf, float w, float fac=1.0) :
  fS(xo), fF(xf), fBWidth(w), fFac(fac) {
  sf::Vector2f D = fF-fS;
  fLength = sqrt(D.x*D.x+D.y*D.y);
  
  sf::Vector2f npar(D.x/fLength, D.y/fLength);
  sf::Vector2f nperp( -npar.y, npar.x );

  if( fBWidth < 0.001 ) fBWidth = 0.001;
  
  fTWidth = fac*fBWidth; // set the top width as a ratio of the bottom

  // Build the corners
  sf::Vector2f tbl(0.5*fBWidth*nperp.x, 0.5*fBWidth*nperp.y);
  sf::Vector2f ttl( npar.x*fLength - 0.5*fTWidth*nperp.x, npar.y*fLength - 0.5*fTWidth*nperp.y);
  sf::Vector2f ttr(npar.x*fLength + 0.5*fTWidth*nperp.x, npar.y*fLength + 0.5*fTWidth*nperp.y);
  sf::Vector2f tbr( 0.5*fBWidth*nperp.x, 0.5*fBWidth*nperp.y);
  sf::Vector2f BL = fS - tbl;
  sf::Vector2f TL = fS + ttl;
  sf::Vector2f TR = fS + ttr;
  sf::Vector2f BR = fS + tbr;

  // insert into array for set the vertex array
  float x[] = {BL.x, TL.x, TR.x, BR.x};
  float y[] = {BL.y, TL.y, TR.y, BR.y};
  
  fBox = sf::VertexArray(sf::Quads,4);
  for(int i=0; i<4; i++){
    fBox[i].position = sf::Vector2f(x[i],y[i]);
  }
}

void Box::SetFillColor(sf::Color col){
   for(int i=0; i<4; i++){ fBox[i].color = col;}
}
