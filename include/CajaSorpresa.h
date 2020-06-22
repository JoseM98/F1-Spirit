#ifndef CAJASORPRESA_H
#define CAJASORPRESA_H
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>


class CajaSorpresa
{
    public:
    CajaSorpresa(float posX, float posY);
    void update();
    void render(sf::RenderWindow *window);
    virtual ~CajaSorpresa();

    int getNumPotenciador(){return numPotenciador;}
    sf::Sprite getSprite(){return sprite;}

    private:
        sf::Texture textura;
        sf::Sprite sprite;

        int numPotenciador;

};


#endif
