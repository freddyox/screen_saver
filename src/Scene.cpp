#include "../include/Scene.hh"
#include <random>

////////////////////////////////////////////////////////////////////////////////
// Set up the scene within the constructor
//
Scene::Scene(float x, float y) : fX(x), fY(y) {
  float yoffset = 100.;
  fTime = 0.0;
  std::random_device rdev{};  // seed the C++11 generator
  std::default_random_engine fRandGen{rdev()};
  std::uniform_int_distribution<int> fFlat(490,510);
  std::uniform_int_distribution<int> fFlatX(0,fX);
  int count=0;
  int nsmalltrees = 10;
  while( count < nsmalltrees ){
    Tree *temp = new Tree(sf::Vector2f(fFlatX(fRandGen), fY-fFlat(fRandGen)), // origin
			  5,                     // Iterations
			  75.0,                  // trunk length
			  10.0,                  // trunk width
			  0.6,                   // trunk trapezoidal gradient
			  45,             // fundamental tree angle
			  0.7,                   // branch length reduction factor relative to trunk
			  0.5,                   // branch width reduction factor relative to trunk
			  0.0,                   // sigma offset
			  0.0,                   // asymmetric angle offset (fAngle+=asymm)
			  2,                     // Number of branches to make at a split
			  true);                // Randomize the algorithm
    temp->setcol(sf::Color(0,0,0));
    temp->generate();                 // Must be generated before GetTree() can be called
    fNSmallTrees.push_back( temp->N() );
    fvTrees.push_back( temp->GetTree() );
    count++;
  }
  
  fTreeL = new Tree(sf::Vector2f(460,fY-170), // origin
  		    11,                     // Iterations
  		    265.0,                 // trunk length
  		    45.0,                  // trunk width
  		    0.6,                   // trunk trapezoidal gradient
  		    40.0,                  // fundamental tree angle
  		    0.7,                  // branch length reduction factor relative to trunk
  		    0.5,                   // branch width reduction factor relative to trunk
  		    0.0,                   // sigma offset
  		    0.0,                   // asymmetric angle offset (fAngle+=asymm)
  		    2,                     // Number of branches to make at a split
  		    false);                 // Randomize the algorithm
  fTreeL->setcol(sf::Color(20,20,20));
  fTreeL->generate();                 // Must be generated before GetTree() can be called  
  fvTreeL = fTreeL->GetTree();

  fTreeR = new Tree(sf::Vector2f(1300,fY-300), // origin
  		    9,                     // Iterations
  		    200.0,                 // trunk length
  		    30.0,                  // trunk width
  		    0.6,                   // trunk trapezoidal gradient
  		    40.0,                  // fundamental tree angle
  		    0.7,                   // branch length reduction factor relative to trunk
  		    0.5,                   // branch width reduction factor relative to trunk
  		    0.0,                   // sigma offset
  		    0.0,                   // asymmetric angle offset (fAngle+=asymm)
  		    2,                     // Number of branches to make at a split
  		    true);                 // Randomize the algorithm
  fTreeR->setcol(sf::Color(20,20,20));
  fTreeR->generate();                 // Must be generated before GetTree() can be called
  while( fTreeR->N() < 5 ) fTreeR->generate(); // ensure that the random tree makes more than trunk
  fvTreeR = fTreeR->GetTree();

  fSwingAnch = 2;
  fLeftVineAnch = 2;
  fRightVineAnch = 2;
  
  // Anchor points for the vines/swings (swing==left, vine ==right) -
  // Note that these positions get initilized here, and updated within simulate
  sf::Vector2f swing_start_pos = fTreeL->get_min_pos(fSwingAnch);
  sf::Vector2f left_anchor = fTreeL->get_max_pos(fLeftVineAnch);
  sf::Vector2f right_anchor = fTreeR->get_min_pos(fRightVineAnch);
  
  int n_masses = 15;
  fNMasses = n_masses;
  float mass_weight = 0.1;       // kg
  float spring_const = 70.0;       // N/m
  float spring_length = 20.0;     // m  
  float spring_frict_const = 1.; // 
  sf::Vector2f gravitation_field(0.0,400); // down is positive
  float air_frict_const = 0.05;
  float ground_repulsion_const = 100.0;
  float ground_friction_const = 0.2;
  float ground_absorption_const = 2.0;
  float ground_height = 1.0;
  fSwing = new Rope(n_masses, mass_weight, spring_const, spring_length,
  		   spring_frict_const, gravitation_field, air_frict_const,
  		   ground_repulsion_const, ground_friction_const,
  		   ground_absorption_const, ground_height);

  fSwing->setcol(sf::Color::Black);
  fvSwing = fSwing->create(swing_start_pos, sf::Vector2f(1000,250), false);

  //////////////////////////////////////////////////////////////////////
  // Generate the tire swing
  //
  if (!fTireSwingText.loadFromFile("images/tire_swing_V2.png")) {
    std::cout << "Tire image not found..." << std::endl;
  }
  fTireSwing.setTexture(fTireSwingText);
  sf::FloatRect box = fTireSwing.getLocalBounds();
  fTireSwing.setOrigin(0.5*box.width,0.0);
  fTireSwing.setPosition(fSwing->get_last_pos());

  // Generate the vine connecting the two trees
  fVineMasses = 15;
  fVine = new Rope(fVineMasses, 0.4, 200.0, 5.0,
  		    0.5, gravitation_field, 0.05,
  		    ground_repulsion_const, ground_friction_const,
  		    ground_absorption_const, ground_height);

  fvVine = fVine->create(left_anchor,right_anchor, true);

  // Make the Grass

  std::normal_distribution<double> grassL(250.0,50.0); // 0 to 5 degrees
  std::normal_distribution<double> grassW(13.0,2.0); // 0 to 5 degrees
  std::normal_distribution<double> ytemp(2.0,5); // 0 to 5 degrees
  std::uniform_int_distribution<int> r(10,80);
  std::uniform_int_distribution<int> g(112,200);
  std::uniform_int_distribution<int> b(37,80);
  fNGrass = 100;
  for(int i=0; i<fNGrass; i++) {
    Grass *grass = new Grass(sf::Vector2f(fFlatX(fRandGen),fY), 10,
			     grassL(fRandGen), grassW(fRandGen),
			     sf::Color(int(r(fRandGen)),int(g(fRandGen)),int(b(fRandGen))));
    fGrass.push_back(grass);
  }
  for(int i=0; i<13; i++){
    std::uniform_int_distribution<int> temp(1270,1330);
    Grass *grass = new Grass(sf::Vector2f(temp(fRandGen),fY-300 + ytemp(fRandGen) ), 6,
			     grassL(fRandGen)/6, grassW(fRandGen)/4,
			     sf::Color(int(r(fRandGen)),int(g(fRandGen)),int(b(fRandGen))));
    fGrass.push_back(grass);
  }
  for(int i=0; i<13; i++){
    std::uniform_int_distribution<int> temp(430,490);
    Grass *grass = new Grass(sf::Vector2f(temp(fRandGen),fY-170 + ytemp(fRandGen) ), 6,
			     grassL(fRandGen)/6, grassW(fRandGen)/4,
			     sf::Color(int(r(fRandGen)),int(g(fRandGen)),int(b(fRandGen))));
    fGrass.push_back(grass);
  }
  fIsItDark = false;

  fOwl = new Owl();
  fOwl->setx(fX);
  fOwl->sety(fY);

  // Load the sound
  if (!buffer.loadFromFile("crickets2.wav")){
    std::cout << "Error with the cricket sounds" << std::endl;
  }
  fCrickets.setBuffer(buffer);
  fCrickets.setLoop(true);
  fVol = 25.;
  fCrickets.setVolume(fVol);
  fCrickets.play();
}

Scene::~Scene(){;}

void Scene::draw( sf::RenderTarget& target, sf::RenderStates) const {
  for(unsigned int i=0; i<fvTrees.size(); i++){
    for(int j=0; j<fNSmallTrees[i]; j++) target.draw(fvTrees[i][j]);
  }
  for(unsigned int i=0; i<fTreeL->N(); i++) target.draw(fvTreeL[i]);
  for(unsigned int i=0; i<fTreeR->N(); i++) target.draw(fvTreeR[i]);
  for(int i=0; i<fNMasses-1; i++) target.draw( fvSwing[i] );
  for(int i=0; i<fVineMasses-1; i++) target.draw( fvVine[i] );
  target.draw(fTireSwing);
  target.draw( *fOwl->GetOwl() );
  for(unsigned int i=0; i<fGrass.size(); i++){
    for(int j=0; j<fGrass[i]->N(); j++) target.draw( (fGrass[i]->get_grass())[j] );
  }
}

////////////////////////////////////////////////////////////////////////////////
// Handle time and events/updating
//
void Scene::update(float dt, bool darkness, sf::Vector2f moonpos){
  fIsItDark = darkness;
  fTime += dt;
  fSwing->simulate(dt);
  sf::Vector2f nhat = fSwing->get_last_nhat();
  float angle = (180.0/3.141592)*acos(nhat.y);
  if( nhat.x>0) angle = 360.0 - angle;
  fTireSwing.setRotation(angle);
  fTireSwing.setPosition(fSwing->get_last_pos());
  fVine->simulate(dt);

  // Simulate the tree, and update anchor points of rope
  fTreeL->simulate(dt,true, 0.0);
  fTreeR->simulate(dt,false, 1.5);
  fSwing->setleftconnectpos(fTreeL->updated_min_pos());
  fVine->setleftconnectpos(fTreeL->updated_max_pos());
  fVine->setrightconnectpos(fTreeR->updated_min_pos());

  fOwl->update(dt,fVine->get_mid_pos(),darkness, moonpos);
  
  for(unsigned int i=0; i<fGrass.size(); i++){
    fGrass[i]->simulate(dt,fIsItDark, (1.5*i)/100.0);
  }

  if(darkness){
    fVol += 0.1;
    if(fVol>100) fVol=100;
  }else{
    fVol-=0.1;
    if(fVol<50.0) fVol=50;
  }
  fCrickets.setVolume(fVol);
}
