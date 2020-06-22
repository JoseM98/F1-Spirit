#include "CajaSorpresa.h"


CajaSorpresa::CajaSorpresa(float posX, float posY){
    if (!textura.loadFromFile("resources/caja.png"))
    {
        std::cerr << "Error cargando la imagen caja.png";
        exit(0);
    }
    sprite = sf::Sprite(textura);
    sprite.setOrigin(64/2,64/2);
    sprite.setTextureRect(sf::IntRect(0*64, 0*64, 64, 64));
    sprite.setPosition(posX, posY);
    sprite.setScale(0.3,0.3);


    numPotenciador=int(rand()%2+1);
}


CajaSorpresa::~CajaSorpresa(){
    // dtor
}


void CajaSorpresa::render(sf::RenderWindow *window){
    window->draw(sprite);
}



void CajaSorpresa::update(){



}
