#include "../include/Mass.hh"

Mass::Mass(){
  fMass = 1.0;
  fVel = sf::Vector2f(0.0,0.0);
  reset_force();
}

Mass::Mass(float m, int id) : fMass(m), fID(id) {
  fVel = sf::Vector2f(0.0,0.0);
  reset_force();
}


void Mass::applyForce(sf::Vector2f force){
  fForce.x += force.x;
  fForce.y += force.y;
}

void Mass::reset_force(){
  fForce = sf::Vector2f(0.0,0.0);
}

void Mass::simulate(float dt){
  fVel.x += (fForce.x / fMass) * dt;
  fVel.y += (fForce.y / fMass) * dt;
  fPos.x += fVel.x * dt;
  fPos.y += fVel.y * dt;
}
