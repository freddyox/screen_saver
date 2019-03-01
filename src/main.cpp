#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include "../include/Scene.hh"
#include "../include/Background.hh"

const double gDisplayx = 1920;
const double gDisplayy = 1080;

int main() {

  sf::RenderWindow window(sf::VideoMode(gDisplayx,gDisplayy), "");
  window.setFramerateLimit(60);

  //////////////////////////////////////////////////////
  //                   Initialize                     //
  //////////////////////////////////////////////////////
  Scene *the_scene = new Scene(gDisplayx, gDisplayy);
  std::string name = "skydome";
  Background *sunset = new Background(name, 194, 0.5, gDisplayx, gDisplayy);
  
  sf::Clock deltaClock;
  while( window.isOpen() ) {
    sf::Event event;
    
    while( window.pollEvent(event) ) {
      if( event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ) {
	window.close();
      }
      // if(event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left) ){
      // 	sf::Vector2i mouse = sf::Mouse::getPosition(window);
      // 	rope.move_left_anchor( window.mapPixelToCoords( mouse ) );
      // }
    }
    window.clear(sf::Color::White);
    sf::Time dt = deltaClock.restart();
    the_scene->update(dt.asSeconds(),sunset->IsItDark(),sunset->get_moon_pos());
    sunset->simulate(dt.asSeconds());
    window.draw(*sunset);
    window.draw(*the_scene);   
    window.display();      
  }
  return 0;
}
