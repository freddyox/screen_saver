#include "../include/Tree.hh"
#include "../include/GenerateForce.hh"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
// Constructor takes many args to avoid a ton of set methods, but the names
// should be obvious. If not, see header file. Tree can be manipulated
// significantly with careful tweaking of parameters.
//
Tree::Tree(sf::Vector2f origin, int iter, float length, float thick, float topthickratio,
	   float theta, float lengthfac, float widfac, float sigma, float asymmangle,
	   int split, bool rand) :
  fOrigin(origin), fIter(iter), fLength(length), fThickness(thick),
  fTopThickRatio(topthickratio), fTheta(theta), fLengthFactor(lengthfac), fWidthFactor(widfac),
  fStartOffset(sigma), fAsymmAngle(asymmangle), fSplitFact(split), fRandomState(rand) {
  deg2rad = 3.141592/180.0;
  xhat = sf::Vector2f(1.0,0.0);
  yhat = sf::Vector2f(0.0,1.0);
  
  fColor = sf::Color::Black;
  fSwingIter = 2;
  fVineIter = 2;
  fTime = 0.0;
  fTempTime = 0.0;
  fWind = 0.0;
  fManipulateState = true;
  fMinTheta = 30.0;
  fMaxTheta = 65.0;
  fIncThetaUp = true;
  fIncThetaDown = false;
}

Tree::~Tree(){
  ;
}

////////////////////////////////////////////////////////////////////////////////
// Generate a tree - if fRandomState = true, then calling generate()
// many times results in different trees. This introduces issues when one
// wants to change the tree angle for example as a completely new tree will be
// built.
//
void Tree::generate(){
  // Set up random gen
  std::random_device rdev{};  // seed the C++11 generator
  std::default_random_engine fRandGen{rdev()};
  std::uniform_int_distribution<int> fFlat(0,10);

  // Storage containers
  std::vector<Box*>::iterator vit;
  std::vector<Box*> Branches;
  std::vector<Box*> Storage,temp;

  // Setting up the trunk and where to position the tree
  float yi = fOrigin.y;
  sf::Vector2f xi(fOrigin.x, yi);
  sf::Vector2f xf(fOrigin.x, fOrigin.y - fLength);
  Box *trunk = new Box(xi,xf,fThickness,fTopThickRatio);
  trunk->SetFillColor(fColor);

  // Initial set up of containers
  Branches.clear();
  fAllBranches.clear();
  fBranchesMap.clear();
  Branches.push_back( trunk );
  fAllBranches.push_back( trunk->GetBox() );
  fBranchesMap[0].push_back( trunk->GetBox() );
  Storage.clear();
  temp.clear();
  Storage = Branches;
  
  // Handle some of the specifics of fRandomState=ON,
  // we might want to change the # of branches being generated
  // at a node; therefore, this flag needs to be here to reset
  bool didwesplit = false;
  int tempsplitfact = fSplitFact;

  int dummy = 0; // iteration index
  while( dummy < fIter ) {
    if( dummy > 0 ){
      Storage.clear();
      Storage = temp;
      temp.clear();
    }
    // The main algorithm
    for( vit=Storage.begin(); vit!=Storage.end(); vit++ ){
      sf::Vector2f bot = (*vit)->GetStart(); // bottom pos
      sf::Vector2f top = (*vit)->GetFinal(); // top pos
      float length = (*vit)->Length();       // magnitude
      float next_length = length*fLengthFactor; // determine the next length=L_{trunk}*fac^{iter}
      sf::Vector2f D = top-bot;
      float mag = sqrt( D.x*D.x + D.y*D.y );
      sf::Vector2f nhat(D.x/mag, D.y/mag); 
      float dprod = nhat.x*xhat.x + nhat.y*xhat.y;
      float ang_wrt_x = acos(dprod)/deg2rad;
      sf::Vector2f nstart = top;

      if( fRandomState ){
	if( fFlat(fRandGen)%10 == 0 ){
	  fSplitFact++;
	  didwesplit = true;
	}
      }
      // Generate the next level of branches
      for( int i=0; i<fSplitFact; i++){
	ang_wrt_x -= fAsymmAngle;
	float tempang = fTheta*2.0 / fSplitFact;
	float fact = (fSplitFact-1) / 2.0;
	float ang = (ang_wrt_x - fact*tempang) + i*tempang;
	
	if( nhat.y > 0 ) { ang = -ang_wrt_x - fact*tempang + i*tempang;}
	
	nstart.x = nstart.x - (fStartOffset*next_length)*nhat.x;
	nstart.y = nstart.y - (fStartOffset*next_length)*nhat.y;

	if( fRandomState ){
	  if( fFlat(fRandGen)%10 == 5 ) continue; // this gives missing branch effect
	  if( fFlat(fRandGen)%5  == 0 ) {
	    std::normal_distribution<double> fGaus(0.0,0.2*next_length);
	    next_length += fGaus(fRandGen);
	  }
	  if( fFlat(fRandGen)%8 == 0 ){
	    std::normal_distribution<double> fGaus(0.0,2.5); // 0 to 5 degrees
	    //ang += fGaus(fRandGen);
	  }
	  if(fFlat(fRandGen)%4 == 0 ){
	    std::normal_distribution<double> fGaus(0.0,0.1*next_length);
	    float offset = fabs( fGaus(fRandGen) );
	    nstart.x = nstart.x - offset * nhat.x;
	    nstart.y = nstart.y - offset * nhat.y;
	  }
	}
	
       	ang = 180.0 - ang; // SFML is left-handed
	sf::Vector2f next_nhat( cos(ang*deg2rad), sin(ang*deg2rad) );
	sf::Vector2f end( nstart.x - next_length*next_nhat.x,
			  nstart.y - next_length*next_nhat.y);
	float width = fWidthFactor * fThickness / ( dummy + 1 );
	Box *branch = new Box(nstart, end, width, fTopThickRatio);
	branch->SetFillColor(fColor);
	Branches.push_back( branch );
	fAllBranches.push_back( branch->GetBox() );
	fBranchesMap[dummy+1].push_back( branch->GetBox() ); // need dummy+1 for this to work
	temp.push_back(branch);	
      }
      // This is critical - the split # needs to be reset, otherwise
      // this number grows according to flat probability distribution
      if( didwesplit ) {
	fSplitFact = tempsplitfact;
	didwesplit = false;
      }
    }
    dummy++;
  }
  return;
}

////////////////////////////////////////////////////////////////////////////////
// Simulate the tree according to fWind (needs work as of now)
//
void Tree::simulate(float dt, bool manip, float offset){
  fTime += dt;
  fWind = generate_perlin(fTime + offset,0.0, 0.0);
  float ang = 10.*3.1415/180.0;
  
  //if( manip ) manipulate();
  sf::Vector2f swingtemp(0.0,0.0);
  sf::Vector2f vinetemp(0.0,0.0);
  int global_vector_index = -1;
  for(fMit=fBranchesMap.begin(); fMit!=fBranchesMap.end(); fMit++){
    int lvl = fMit->first;         // iteration level
    float weight = 0.15*float(lvl); // wider branches should move less, trunk should move barely
    sf::Vector2f windvec(weight*fWind*cos(ang),weight*fWind*sin(ang));
    int map_vec_index = -1;

    if(lvl==fSwingIter) swingtemp = windvec; // needed info to update positions of Swings
    if(lvl==fVineIter)  vinetemp  = windvec; // needed info to update positions of swings
    
    for(unsigned int i=0; i<fMit->second.size(); i++){ // std::vector (branch level)
      global_vector_index++; // this has to be outside of vertex array loop
      map_vec_index++;
      // The indices that I want to manipulate are 1 and 2, which
      // correspond to TL and TR (see Box.cpp for how the Box arrays are made)
      // 0 and 3 are BL and BR
      for(int v=1; v<3; v++){ // vertex array level
	fAllBranches[global_vector_index][v].position += windvec;
	fBranchesMap[lvl][map_vec_index][v].position  += windvec;
      }
    }
    // Now we need to update BL and BR of the next level up
    map_vec_index = -1;
    if(lvl<fIter){
      for(unsigned int i=0; i<fBranchesMap[lvl+1].size(); i++){
	fAllBranches[global_vector_index+1+i][0].position += windvec;
	fAllBranches[global_vector_index+1+i][3].position += windvec;
	map_vec_index++;
	fBranchesMap[lvl+1][map_vec_index][0].position += windvec;
	fBranchesMap[lvl+1][map_vec_index][3].position += windvec;
      }
    }
  }
  fSwingPos += swingtemp;
  fVinePos += vinetemp;
}

////////////////////////////////////////////////////////////////////////////////
// Get left-most position within iteration level n
//
sf::Vector2f Tree::get_min_pos(int n){
  if( n<0 || n>fIter ) {
    std::cout << "get_max_pos arg is too small or big" << std::endl;
    return sf::Vector2f(0.0,0.0);
  }

  if( fBranchesMap.empty() ){
    return sf::Vector2f(0.0,0.0);
  }
  fSwingIter = n;
  float xtemp = 1.0e5, ytemp = 1.0e5;
  for(unsigned int i=0; i<fBranchesMap[n].size(); i++){
    for(int v=0; v<4; v++){
	sf::Vector2f vpos = fBranchesMap[n][i][v].position;
      if( vpos.x < xtemp ){
	xtemp = vpos.x;
	ytemp = vpos.y;
      }
    }
  }
  fSwingPos = sf::Vector2f(xtemp,ytemp);
  return sf::Vector2f(xtemp,ytemp);
}

////////////////////////////////////////////////////////////////////////////////
// Get right-most position within iteration level n
//
sf::Vector2f Tree::get_max_pos(int n){
  if( n<0 || n>fIter ) {
    std::cout << "get_max_pos arg is too small or big" << std::endl;
    return sf::Vector2f(0.0,0.0);
  }

  if( fBranchesMap.empty() ){
    return sf::Vector2f(0.0,0.0);
  }
  fVineIter = n;
  float xtemp = -1.0e5, ytemp = -1.0e5;
  for(unsigned int i=0; i<fBranchesMap[n].size(); i++){
    for(int v=0; v<4; v++){
      sf::Vector2f vpos = fBranchesMap[n][i][v].position;
      if( vpos.x > xtemp ){
	xtemp = vpos.x;
	ytemp = vpos.y;
      }
    }
  }
  fVinePos = sf::Vector2f(xtemp,ytemp);
  return sf::Vector2f(xtemp,ytemp);
}

////////////////////////////////////////////////////////////////////////////////
// Manipulate the fundamental angle of a tree with fRandomState==OFF.
// This doesn't look cool with random on...
//
void Tree::manipulate(){
  if(int(fTime)%5==0 && fManipulateState ){ // this is in seconds
    if( fTheta >= fMaxTheta ){
      fIncThetaUp = false;
      fIncThetaDown = true;
    }
    if( fTheta <= fMinTheta ){
      fIncThetaUp = true;
      fIncThetaDown = false;
    }
    if( fIncThetaUp ) fTheta++;
    if( fIncThetaDown ) fTheta--;
    
    generate(); // Attempt to call this at the bare minimum..this is heavy if fIter is large
    fManipulateState = false;
    fTempTime = fTime;
  }
  if( !fManipulateState ){
    if( fTime-fTempTime > 1.0 ){ // this always will be positive
      fTempTime = 0.0;           // reset temp time for future comparison
      fManipulateState = true;   // reset flag
    }
  }
}
