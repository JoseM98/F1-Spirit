#include "CocheEnemigo.h"

const double PI = 3.14159;

CocheEnemigo::CocheEnemigo(Mapa *mapaJuego,float x, float y)
{
    mapa=mapaJuego;
    imagen=mapa->getImagen1();
    imagen2=mapa->getImagen2();

    int numSprite = int(rand()%8+2);
    if (!textura.loadFromFile("resources/cars-endurance.png"))
    {
        std::cerr << "Error cargando la imagen cars-endurance.png";
        exit(0);
    }
    //Y creo el spritesheet a partir de la imagen anterior
    sprite = sf::Sprite(textura);
    //Le pongo el centroide donde corresponde
    sprite.setOrigin(32/2,32/2);
    //Cojo el sprite que me interesa por defecto del sheet
    sprite.setTextureRect(sf::IntRect(numSprite*32, 0*32, 32, 32));
    // Lo dispongo en el centro de la pantalla
    sprite.setPosition(x, y);
    sprite.setScale(0.75,0.75);

    angulo=0;
    velocidad=0;

    aceleracion=int(rand()%4+3); // random entre 0 y 3
    maxVelocidad=int(rand()%60+180);
    desvioCentro=int(rand()%56-28); // es un random que dice cuanto se separan

    actualizarY=3040.0;
    vuelta = 4; // numero de vueltas
    mapaActual=1;
    posicionMeta=3040-842-6080*2;

    posSigX=sprite.getPosition().x;
    posSigY=sprite.getPosition().y;
    posAntX=posSigX;
    posAntY=posSigY;
    percentTick = 0.0;   // tiempo transcurrido desde el ultimo update
    posFinX=0.0;  // posicion X final del objeto
    posFinY=0.0;


    giroClock.restart();
    cout << "maxVel: " << maxVelocidad << " --- Acel: " << aceleracion << " --- Sprite: " << numSprite  << " --- Dist: " << desvioCentro << endl;
}

CocheEnemigo::~CocheEnemigo()
{
    //dtor
}


void CocheEnemigo::render(double percentTick){
    posFinX=(posSigX-posAntX)*percentTick+posAntX;
    posFinY=(posSigY-posAntY)*percentTick+posAntY;
    //std::cout << posFinX << std::endl;
    sprite.setPosition(posFinX,posFinY);
}

void CocheEnemigo::draw(sf::RenderWindow *window){
    window->draw(sprite);
}



void CocheEnemigo::update(sf::Time timeElapsed){
    velocidad+=aceleracion;
    if(velocidad>maxVelocidad)
        velocidad=maxVelocidad;

    colisionCesped();

    posAntX=posSigX;
    posAntY=posSigY;
    int anguloSig=0;
    detectarMeta(posAntX);
    int mejorDiferencia;

    double posXSigDer= posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(30*PI/180.d);
    double posYSigDer=posAntY-(velocidad/1000.d)*timeElapsed.asMilliseconds()*cos(30*PI/180.d);
    int difDer = distanciaLinea(posXSigDer,posYSigDer);
    anguloSig=30;
    mejorDiferencia=difDer;

    double posXSigIzq=posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(-30*PI/180.d);
    double posYSigIzq=posAntY-(velocidad/1000.d)*timeElapsed.asMilliseconds()*cos(-30*PI/180.d);
    int difIzq = distanciaLinea(posXSigIzq,posYSigIzq);
    if(abs(difIzq) < abs(mejorDiferencia)){
        anguloSig=-30;
        mejorDiferencia=difIzq;
    }

    double posXSigCent=posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(0*PI/180.d);
    double posYSigCent=posAntY-(velocidad/1000.d)*timeElapsed.asMilliseconds()*cos(0*PI/180.d);
    int difCent = distanciaLinea(posXSigCent,posYSigCent);
    if(abs(difCent) <= abs(mejorDiferencia)){
        anguloSig=0;
        mejorDiferencia=difCent;
    }

    double posXSigDerB= posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(60*PI/180.d);
    double posYSigDerB=posAntY-(velocidad/1000.d)*timeElapsed.asMilliseconds()*cos(60*PI/180.d);
    int difDerB = distanciaLinea(posXSigDerB,posYSigDerB);
    if(abs(difDerB) < abs(mejorDiferencia) && abs(mejorDiferencia)>40){
        anguloSig=60;
        mejorDiferencia=difDerB;
    }

    double posXSigIzqB=posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(-60*PI/180.d);
    double posYSigIzqB=posAntY-(velocidad/1000.d)*timeElapsed.asMilliseconds()*cos(-60*PI/180.d);
    int difIzqB = distanciaLinea(posXSigIzqB,posYSigIzqB);
    if(abs(difIzqB) < abs(mejorDiferencia) && abs(mejorDiferencia)>40){
        anguloSig=-60;
        mejorDiferencia=difIzq;
    }


    if(giroClock.getElapsedTime().asMilliseconds()>200){
        rotacionCoche(anguloSig);  // modifica el angulo
    }

    posSigX=posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(angulo*PI/180.d);
    posSigY=posAntY-(velocidad/1000.d)*timeElapsed.asMilliseconds()*cos(angulo*PI/180.d);
    //cout << "holaaaa" << endl;

    if(int(posSigX)+1440>=0 && int(posSigY)+actualizarY>=0){
        if(mapaActual%2==1)
            colorMapa=imagen->getPixel(int(posSigX)+1440,int(posSigY)+actualizarY);
        else
            colorMapa=imagen2->getPixel(int(posSigX)+1440,int(posSigY)+actualizarY);

    }else{
        actualizarY=actualizarY+6080.0;
        mapaActual++;
        if(mapaActual%2==1)
            colorMapa=imagen->getPixel(int(posSigX)+1440,int(posSigY)+actualizarY);
        else
            colorMapa=imagen2->getPixel(int(posSigX)+1440,int(posSigY)+actualizarY);
    }

}


void CocheEnemigo::detectarMeta(int y){
    if(posicionMeta>y){
        posicionMeta-=6080*2;
        vuelta--;
    }
}


int CocheEnemigo::distanciaLinea(int posX, int posY){
    int diferencia=3000;
    bool encontrado=false;
    float posAPonerX=0;
    sf::Color pixelChoque;

    for(int i=0;i<2880 && encontrado!=true; i+=1){
        //cout << i << " - " << yCoche+actualizarY << endl;
        if(mapaActual%2==1){
            if(int(posY)+actualizarY>=0)
                pixelChoque=imagen->getPixel(i,posY+actualizarY);
            else
                pixelChoque=imagen2->getPixel(i,posY+actualizarY+6080);
        }else{
            if(int(posY)+actualizarY>=0)
                pixelChoque=imagen2->getPixel(i,posY+actualizarY);
            else
                pixelChoque=imagen->getPixel(i,posY+actualizarY+6080);
        }

        if((static_cast<int>(pixelChoque.r)==0 && static_cast<int>(pixelChoque.g)==255 && static_cast<int>(pixelChoque.b)==255)){
            encontrado=true;
            //cout << i << " - " << yCoche << endl;
            posAPonerX = i-1440;
            //cout << posAPonerX << " -> (" << posX << " , " << posY << ") -->  "  << (posAPonerX-posX) << endl;
        }
    }
    return (posAPonerX-posX+desvioCentro);
}


void CocheEnemigo::colisionCesped(){
    sf::Color pixelChoque;
    if(mapaActual%2==1)
        pixelChoque=imagen->getPixel(sprite.getPosition().x+1440,sprite.getPosition().y+actualizarY);
    else
        pixelChoque=imagen2->getPixel(sprite.getPosition().x+1440,sprite.getPosition().y+actualizarY);

    if((static_cast<int>(pixelChoque.r)==255 && static_cast<int>(pixelChoque.g)==0 && static_cast<int>(pixelChoque.b)==0)){
        if(velocidad>150)
            velocidad=150;
    }
}


void CocheEnemigo::rotacionCoche(int rot){
    if(rot==0)
    {
        if(angulo==30){
            sprite.rotate(-30);
            angulo=0;
            giroClock.restart();
        }else if(angulo==-30){
            sprite.rotate(30);
            angulo=0;
            giroClock.restart();
        }else if(angulo==-60){
            sprite.rotate(60);
            angulo=0;
            giroClock.restart();
        }else if(angulo==60){
            sprite.rotate(-60);
            angulo=0;
            giroClock.restart();
        }
    }
    else if(rot==30)
    {
        if(angulo==0){
            sprite.rotate(30);
            angulo=30;
            giroClock.restart();
        }else if(angulo==-30){
            sprite.rotate(30);
            angulo=0;
        }else if(angulo==-60){
            sprite.rotate(60);
            angulo=0;
            giroClock.restart();
        }else if(angulo==60){
            sprite.rotate(-30);
            angulo=30;
            giroClock.restart();
        }
    }
    else if(rot==-30)
    {
        if(angulo==0){
            sprite.rotate(-30);
            angulo=-30;
            giroClock.restart();
        }else if(angulo==30){
            sprite.rotate(-30);
            angulo=0;
        }else if(angulo==-60){
            sprite.rotate(30);
            angulo=-30;
            giroClock.restart();
        }else if(angulo==60){
            sprite.rotate(-60);
            angulo=0;
            giroClock.restart();
        }
    }
    else if(rot==60)
    {
        if(angulo==0){
            sprite.rotate(60);
            angulo=60;
            giroClock.restart();
        }else if(angulo==30){
            sprite.rotate(30);
            angulo=60;
        }else if(angulo==-60){
            sprite.rotate(60);
            angulo=0;
            giroClock.restart();
        }else if(angulo==-30){
            sprite.rotate(30);
            angulo=0;
            giroClock.restart();
        }
    }
    else if(rot==-60)
    {
        if(angulo==0){
            sprite.rotate(-60);
            angulo=-60;
            giroClock.restart();
        }else if(angulo==30){
            sprite.rotate(-30);
            angulo=0;
        }else if(angulo==60){
            sprite.rotate(-60);
            angulo=0;
            giroClock.restart();
        }else if(angulo==-30){
            sprite.rotate(-30);
            angulo=-60;
            giroClock.restart();
        }
    }
}











