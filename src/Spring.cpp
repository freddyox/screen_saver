#include "../include/Spring.hh"
#include <cmath>
#include <iostream>

Spring::Spring(Mass* m1, Mass* m2, float k, float l, float mu) :
  fM1(m1), fM2(m2), fSpringConst(k), fSpringLength(l), fSpringFriction(mu) {
  ;
}

Spring::~Spring(){
  delete fM1;
  delete fM2;
}

void Spring::solve(){
  sf::Vector2f r_vec = fM1->pos() - fM2->pos();
  float length = sqrt( r_vec.x * r_vec.x + r_vec.y * r_vec.y );
  sf::Vector2f r_hat( r_vec.x / length, r_vec.y / length );
  sf::Vector2f force(0.0,0.0);
  if( length != 0 ){
    force.x += -(r_hat.x) * (length - fSpringLength) * fSpringConst;
    force.y += -(r_hat.y) * (length - fSpringLength) * fSpringConst;
  }
  force += -(fM1->vel() - fM2->vel()) * fSpringFriction;
  fM1->applyForce(force);
  fM2->applyForce(-force);
}
