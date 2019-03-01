#include "../include/GenerateForce.hh"
double generate_perlin(double x, double y, double z){
  module::Perlin fMod;
  return fMod.GetValue(x,y,z);
}
