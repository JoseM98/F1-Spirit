#ifndef JUEGO_H
#define JUEGO_H
#include "coche.h"
#include "Hud.h"
#include "Mapa.h"
#include "CocheEnemigo.h"

using namespace std;

using namespace sf;

class juego
{
    public:
        juego();
        juego(int resolucionX, int resolucionY, std::string  titulo);
        void dibujarPantalla();
        void gameloop();
        void keyloop();
        virtual ~juego();
        int tecla();

        void mostrarPantallaFin(Mapa *mapa);
        void resetearJuego(coche *cocheJugador, Mapa *mapa, Hud *hud);

    protected:

    private:
        sf::View viewJuego;
        sf::View viewHud;
        sf::View viewfinJuego;
        sf::RenderWindow * window;

        sf::Text textoFin;
        sf::Font fuenteFin;

        sf::Clock reiniciarJuego;

        std::vector<CocheEnemigo*> vectorEnemigos;
};

#endif // JUEGO_H
