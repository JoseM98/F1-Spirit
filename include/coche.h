#ifndef COCHE_H
#define COCHE_H
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>

using namespace std;

class coche
{
    public:
        coche();
        virtual ~coche();

        void update(int teclaMovimiento, int teclaPotenciador,sf::Time timeElapsed);
        void render(double percentTick, sf::View &view);
        void draw(sf::RenderWindow *window);
        void comprobarRoturas();
        void comprobarMarchas();
        void movimientoCoche(int teclaMovimiento, sf::Time timeElapsed);
        void giroCoche(int teclaMovimiento);
        void animarChoque();
        void rozamientoSuelo();
        void comenzarAnimacionChoque(float choqueX, float choqueY);

        void setGasolina(float fuel){gasolina=fuel;}
        void setVelocidad(double vel){velocidad=vel;}
        void setPisandoCesped(bool pisando){pisandoCesped=pisando;}
        void setRepostando(bool repostando){this->repostando=repostando;}
        void setNeumDelRoto(bool del){neumaticoDelRoto=del;}
        void setNeumTrasRoto(bool tras){neumaticoTrasRoto=tras;}
        void setMotorRoto(bool mot){motorRoto=mot;}
        void setFrenosRoto(bool fren){frenosRoto=fren;}
        void setGodMode(bool modo){godMode=modo;}
        void setPotenciador(int potenciador){this->potenciador=potenciador;}
        void setPosSigX(float x){posSigX=x;}

        float getGasolina(){return gasolina;}
        double getVelocidad(){return velocidad;}
        bool getRepostando(){return repostando;}
        bool getNeumDelRoto(){return neumaticoDelRoto;}
        bool getNeumTrasRoto(){return neumaticoTrasRoto;}
        bool getMotorRoto(){return motorRoto;}
        bool getFrenosRoto(){return frenosRoto;}
        float getAceleracion();
        int getMarcha(){return marcha;}
        sf::Sprite getSprite(){return sprite;}
        bool getAnimacionChoque(){return animacionChoque;}
        bool getGodMode(){return godMode;}
        int getPotenciador(){return potenciador;}



    private:
        sf::Texture textura;
        sf::Sprite sprite;

        int angulo;
        int rotacionCoche;
        double velocidad;
        double maxVelocidad;
        float aceleracion;
        float frenado;
        float rozamiento;
        int marcha;
        float marcha_aceleracion;
        bool acelerando;
        bool pisandoCesped;

        bool neumaticoDelRoto;
        bool neumaticoTrasRoto;
        bool frenosRoto;
        bool motorRoto;
        bool repostando;
        bool godMode;

        sf::Clock retardoGiro;
        sf::Clock giroBruscoClock;
        sf::Clock animacionChoqueClock;
        sf::Clock godModeClock;
        bool giroBrusco;
        int teclaMovimientoAnterior;
        float gasolina;

        double posSigX, posSigY;
        double posAntX, posAntY;
        double percentTick;   // tiempo transcurrido desde el ultimo update
        double posFinX;  // posicion X final del objeto
        double posFinY;

        bool animacionChoque; // si se produce la animacion del coche
        float choqueX; // posicion donde recolocar el coche
        float choqueY;

        int potenciador;
        double maxvelocidadExtra;  // velocidad que se le aplica al potenciador
        float aceleracionExtra;
        sf::Clock tiempoTurbo;

};

#endif // COCHE_H
