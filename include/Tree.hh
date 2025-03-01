#ifndef TREE_HH
#define TREE_HH
#include "../include/Box.hh"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <map>
#include <random>

////////////////////////////////////////////////////////////////////////////////
// Class to generate a realistic tree with no texture
//    -Requires Box.hh header in order to make trapezoids oriented
//    -at some angle.
//
// Obrecht - June 2018
//
class Tree{
private:
  float deg2rad;
  sf::Vector2f xhat,yhat;
  
  sf::Vector2f fOrigin; // Where to put the trunk
  int fIter;            // Number of iterations
  float fLength;        // Length of trunk
  float fThickness;     // Trunk thickness
  float fTopThickRatio; // Trapezoidal gradient of trunk
  float fLengthFactor;  // Branch length reduction factor relative to trunk
  float fWidthFactor;   // Branch width  reduction factor relative to trunk
  float fTheta;         // Fundamental angle of tree
  float fStartOffset;   // A scaler to offset the starting position
  float fAsymmAngle;    // Bias the fundamental angle
  int fSplitFact;       // # of branches to generate at a split
  bool fRandomState;    // Flag to turn on the random tree generator
  sf::Color fColor;     // Color of the tree

  // fSwingPos, fVinePos are initialized in get_min_pos and get_max_pos methods,
  // and the wind velocity updates the position within simulate
  sf::Vector2f fSwingPos, fVinePos; // Left/right-most position within a particular iteration
  int fSwingIter, fVineIter;        // Which level of the tree to place the swings
  
  // Time sensitive information
  double fTime, fTempTime;  // The global and temp time 
  float fWind;              // libnoise wind velocity
  bool fManipulateState;    // Control how often manipulation occurs
  bool fIncThetaUp, fIncThetaDown; // Control how theta gets incremented
  float fMinTheta, fMaxTheta;      // Control the limits

  ////////////////////////////////////////////////////////////////////////////////
  // Meaty storage containers - houses the results
  //
  std::vector<sf::VertexArray> fAllBranches; // The tree in vertex array form

  // Key is branch level where trunk is defined to be 0
  // Therefore the key runs from [0,fIter], and the value
  // is the list of branches in sf::VertexArray form (the most convenient)
  std::map<int,std::vector<sf::VertexArray> > fBranchesMap;
  std::map<int,std::vector<sf::VertexArray> >::iterator fMit;
  
public:
  // See .cpp for information on the args
  Tree(sf::Vector2f, int, float, float, float, float,
       float, float, float, float, int, bool);     
  ~Tree();
  void generate();           // generate a tree
  void simulate(float,bool,float); // move tree according to fWind, manipulate if true
  void manipulate();         // Manipulate the angle of a tree with fRandomState=OFF
  void setcol(sf::Color c){fColor=c;} // This must be called before generate, otherwise fColor=black
  
  int N(){return fAllBranches.size();}                   // Return size for pointer use
  sf::VertexArray* GetTree(){ return &fAllBranches[0]; } // Pointer to vector array of branches

  sf::Vector2f get_min_pos(int);  // Get left-most  position in level n
  sf::Vector2f get_max_pos(int);  // Get right-most position in level n
  sf::Vector2f updated_min_pos(){ return fSwingPos; } // return wind-velocity updated swing pos
  sf::Vector2f updated_max_pos(){ return fVinePos;  } // return wind-velocity updated swing pos
  bool get_manipulate_state(){ return fManipulateState; } // are in the act of tuning angles?
};
#endif
