#ifndef HUD_H
#define HUD_H
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "coche.h"
#include "Mapa.h"
#include "CocheEnemigo.h"


class Hud
{
    public:
        Hud(coche *cocheJugador, Mapa *mapa, std::vector<CocheEnemigo*> vectorEne);
        void update();
        void render(sf::RenderWindow *window);
        virtual ~Hud();

        void calcularCuadradosGasolina();
        void calcularVelocidad();
        void calcularCirculoMinimapa();
    protected:

    private:
        coche *cocheJugador;
        Mapa *mapa;

        sf::Sprite *spriteFondo;
        sf::Texture textura;
        sf::Font font;
        sf::Text textVelocidad;
        sf::Text textMarcha;
        sf::Text textRun;
        sf::Text vuelta;
        sf::Text modoDios;
        sf::Clock parpadeo;
        sf::Texture textPotenciador;
        sf::Sprite spritePotenciador;

        sf::Text textoPosicion;
        int posicion;
        std::vector<CocheEnemigo*> vectorEnemigos;

        sf::Texture texturaCircuito;
        sf::Sprite *spriteCircuito;
        sf::CircleShape circuloJugMapa;

        sf::RectangleShape neumDelRect;
        sf::RectangleShape neumTrasRect;
        sf::RectangleShape motorRect;
        sf::RectangleShape frenoRect;

        vector<sf::RectangleShape> vectorGasolina;
        vector<sf::RectangleShape> vectorAceleracion;
        int cuadradosGasolina;
        int velocidad;
        int aceleracionAnterior;
        sf::Clock actualizarAceleracion;
};

#endif // HUD_H
