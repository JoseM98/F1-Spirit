#ifndef MAPA_H
#define MAPA_H

#include "coche.h"
#include "CajaSorpresa.h"



class Mapa
{
    public:
        Mapa(coche *cocheJugador);
        virtual ~Mapa();
        void update();
        void render(sf::RenderWindow *window);
        void renderPitlane(sf::RenderWindow *window);

        void cambioImagen(int yMapa);
        void detectarMeta(int y);
        void salirPista(int xCoche, int yCoche);
        void detectarCesped();
        void detectarPitlane();
        void gestionRespostar();
        void finCarrera();
        string tiempoToString(sf::Clock relojTiempo);
        void cogerPotenciador();
        void potenciadorPosicion(int posX, int posY);

        int getNumVuelta(){return vuelta;}
        bool getFinJuego(){return finJuego;}
        int getvalorFinJuego(){return valorFinJuego;}
        sf::Image *getImagen1(){return &imagen;}
        sf::Image *getImagen2(){return &imagen2;}


    private:
        sf::Texture textura;
        sf::Sprite mapa;
        sf::Image imagen;
        sf::Image imagen2;
        sf::Texture textura2;
        sf::Sprite mapa2;
        sf::Texture texturaRepostar;
        sf::Sprite imagenRepostar;
        //sf::Image imagen2;
        sf::Color colorMapa;
        coche *cocheJugador;

        sf::Clock choqueReloj;
        sf::Clock entrarPitlaneReloj;
        sf::Clock repostarReloj;
        sf::Clock repostarPiezaReloj;
        sf::Text textoTiempoPit;
        sf::Font fuente;
        sf::Clock delayFinJuego;

        sf::Clock tiempoVuelta;
        sf::Clock tiempoMostrandoTexto;
        sf::Text textoTiempo;
        sf::RectangleShape rectTextoTiempo;
        bool mostrarTiempo;

        float actualizarY;
        int vuelta;
        int mapaActual;
        int posicionMeta;

        bool recalcularRozamiento; // en caso de que toque cesped
        int ultimaVueltaRepostada;
        bool repostando;
        bool excesoVelRepostando;
        float tiempoRepostadoFinal;

        bool finJuego;
        int valorFinJuego;

        std::vector<CajaSorpresa *> vectorCajas;
        int potenciadorAnterior;
};

#endif // MAPA_H
