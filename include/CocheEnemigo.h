#ifndef COCHEENEMIGO_H
#define COCHEENEMIGO_H

#include "Mapa.h"

class CocheEnemigo
{
    public:
        CocheEnemigo(Mapa *mapaJuego, float x, float y);
        void update(sf::Time timeElapsed);
        void render(double percentTick);
        void draw(sf::RenderWindow *window);
        virtual ~CocheEnemigo();

        void rotacionCoche(int rot);
        int distanciaLinea(int posX, int posY);
        void detectarMeta(int y);
        void colisionCesped();

        sf::Sprite getSprite(){return sprite;}



    private:
        sf::Texture textura;
        sf::Sprite sprite;
        Mapa *mapa;

        sf::Image *imagen;
        sf::Image *imagen2;
        sf::Color colorMapa;

        sf::Clock giroClock;

        int angulo;
        double velocidad;
        double aceleracion;
        double maxVelocidad;
        float actualizarY;
        int vuelta;
        int mapaActual;
        int posicionMeta;

        double posSigX, posSigY;
        double posAntX, posAntY;
        double percentTick;   // tiempo transcurrido desde el ultimo update
        double posFinX;  // posicion X final del objeto
        double posFinY;

        double desvioCentro;
};

#endif // COCHEENEMIGO_H
