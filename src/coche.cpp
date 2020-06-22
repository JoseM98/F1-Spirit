#include "coche.h"

const double PI = 3.14159;


coche::coche()
{
    rotacionCoche=30;
    velocidad=0.0;
    maxVelocidad=250.0;
    angulo=0;
    retardoGiro.restart();
    giroBruscoClock.restart();
    giroBrusco=false;
    gasolina=70.0; // hay 7 barras de gasolina
    teclaMovimientoAnterior=0;


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
    sprite.setTextureRect(sf::IntRect(0*32, 0*32, 32, 32));
    // Lo dispongo en el centro de la pantalla
    sprite.setPosition(-101, 2360);
    sprite.setScale(0.75,0.75);


    posSigX=sprite.getPosition().x;
    posSigY=sprite.getPosition().y;
    posAntX=posSigX;
    posAntY=posSigY;

    percentTick = 0.0;   // tiempo transcurrido desde el ultimo update
    posFinX=0.0;  // posicion X final del objeto
    posFinY=0.0;

    aceleracion=6.0;
    frenado = 3.0;
    rozamiento = 1.5;
    marcha=1;
    marcha_aceleracion=6.0;
    pisandoCesped=false;
    repostando=false;
    godMode=false;
    godModeClock.restart();

    neumaticoDelRoto=false;
    frenosRoto=false;
    motorRoto=false;
    neumaticoTrasRoto=false;

    acelerando=false;
    animacionChoque=false;
    animacionChoqueClock.restart();
    choqueX = posSigX;
    choqueY = posSigY;

    potenciador=0;
    maxvelocidadExtra=0.d;
    aceleracionExtra=0.0;
    tiempoTurbo.restart();

    comprobarRoturas();
}

coche::~coche()
{
    //dtor
}


void coche::render(double percentTick, sf::View &view)
{
    posFinX=(posSigX-posAntX)*percentTick+posAntX;
    posFinY=(posSigY-posAntY)*percentTick+posAntY;
    //std::cout << posFinX << std::endl;
    sprite.setPosition(posFinX,posFinY);
    view.setCenter(sprite.getPosition().x,sprite.getPosition().y);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::G) && godModeClock.getElapsedTime().asMilliseconds()>200){
        if(godMode==true)
            godMode=false;
        else{
            godMode=true;
            gasolina=70.0;
            neumaticoDelRoto=false;
            neumaticoTrasRoto=false;
            motorRoto=false;
            frenosRoto=false;
            comprobarRoturas();
        }
        godModeClock.restart();
    }
}


void coche::draw(sf::RenderWindow *window)
{
    window->draw(sprite);
}

float coche::getAceleracion(){
    if(acelerando==true)
        return aceleracion;
    else
        return 0;
}


void coche::update(int teclaMovimiento, int teclaPotenciador, sf::Time timeElapsed)
{
    int tecMovimiento = teclaMovimiento;

    if(animacionChoque==true){
        tecMovimiento=2;
        animarChoque();
    }

    posAntX=posSigX;
    posAntY=posSigY;

    rozamientoSuelo();
    comprobarMarchas();

    acelerando=false;

    if(repostando==false){
        // calcular siguiente posicion del coche
        movimientoCoche(tecMovimiento, timeElapsed);

        // comprobar rotacion del coche dependiendo de la tecla pulsada
        giroCoche(tecMovimiento);
    }else{
        giroCoche(0);
    }


    if(teclaPotenciador==1 && repostando!=true && animacionChoque!=true){
        if(potenciador==2){
            tiempoTurbo.restart();
            aceleracionExtra=5;
            maxvelocidadExtra=150;
        }
        potenciador=0;
    }
    if(tiempoTurbo.getElapsedTime().asMilliseconds()>3000){
        aceleracionExtra=0;
        maxvelocidadExtra=0;
    }
    //std::cout << aceleracion << std::endl;

    teclaMovimientoAnterior = tecMovimiento;
}


void coche::animarChoque(){
    if(animacionChoqueClock.getElapsedTime().asSeconds()<0.4){
        velocidad=110;
        sprite.setTextureRect(sf::IntRect(0*32, 1*32, 32, 32));
    }else if(animacionChoqueClock.getElapsedTime().asSeconds()<0.8){
        velocidad=95;
        sprite.setTextureRect(sf::IntRect(0*32, 2*32, 32, 32));
    }else if(animacionChoqueClock.getElapsedTime().asSeconds()<1.2){
        velocidad=70;
        sprite.setTextureRect(sf::IntRect(0*32, 3*32, 32, 32));
    }else if(animacionChoqueClock.getElapsedTime().asSeconds()<1.6){
        velocidad=45;
        sprite.setTextureRect(sf::IntRect(0*32, 4*32, 32, 32));
    }else if(animacionChoqueClock.getElapsedTime().asSeconds()<2.0){
        velocidad=30;
        sprite.setTextureRect(sf::IntRect(0*32, 5*32, 32, 32));
    }else if(animacionChoqueClock.getElapsedTime().asSeconds()<2.8){
        sprite.setTextureRect(sf::IntRect(0*32, 0*32, 32, 32));
        posSigX = choqueX;
        posSigY = choqueY;
        animacionChoque=false;
        if(godMode==false){
            gasolina-=10;
            srand(time(NULL));
            int piezaRota = rand()%5; // random entre 0 y 4
            if(piezaRota==0)
                neumaticoDelRoto=true;
            else if(piezaRota==1)
                neumaticoTrasRoto=true;
            else if(piezaRota==2)
                motorRoto=true;
            else if(piezaRota==3)
                frenosRoto=true;
        }
        animacionChoqueClock.restart();
        comprobarRoturas();
    }
}


void coche::movimientoCoche(int teclaMovimiento, sf::Time timeElapsed){
    velocidad-=double(rozamiento);

    switch(teclaMovimiento) {
        case 1:
            if(gasolina>0){
                velocidad+=double(aceleracion+aceleracionExtra);
                if(godMode==false)
                    gasolina-=0.035;
                acelerando=true;
            }
        break;

        case 2:
            velocidad-=double(frenado);
            //pelotaSiguienteY=pelotaAnteriorY+kVelY*timeElapsed.asMilliseconds();
        break;

        case 3:
            if(velocidad>0.0)
                posSigX=posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(angulo*PI/180.d);
        break;

        case 4:
            if(velocidad>0.0)
                posSigX=posAntX-(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(angulo*PI/180.d)*(-1);
        break;

        case 5:
            if(gasolina>0){
                velocidad+=double(aceleracion+aceleracionExtra);
                if(godMode==false)
                    gasolina-=0.035;
                acelerando=true;
            }
            if(velocidad>0.0)
                posSigX=posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(angulo*PI/180.d);
        break;

        case 6:
            if(gasolina>0){
                velocidad+=double(aceleracion+aceleracionExtra);
                if(godMode==false)
                    gasolina-=0.035;
                acelerando=true;
            }
            if(velocidad>0.0)
                posSigX=posAntX-(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(angulo*PI/180.d)*(-1);
        break;

        case 7:
            velocidad-=frenado;
            if(velocidad>0.0)
                posSigX=posAntX+(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(angulo*PI/180.d);
             // para que no gire cuando va muy lento
        break;

        case 8:
            velocidad-=frenado;
            if(velocidad>0.0)
                posSigX=posAntX-(velocidad/1000.d)*timeElapsed.asMilliseconds()*sin(angulo*PI/180.d)*(-1);
        break;
    }
    // no superar la velocidad
    if(velocidad<0.0)
        velocidad=0.0;
    else if(velocidad>maxVelocidad+maxvelocidadExtra)
        velocidad=maxVelocidad+maxvelocidadExtra;

    // calculo posicion ******* IMPORTANTE ********* nos permite que el coche se mueva sin pulsar tecla
    posSigY=posAntY-(velocidad/1000.d)*timeElapsed.asMilliseconds()*cos(angulo*PI/180.d);
    //std::cout << teclaMovimiento << " --> " << cos(angulo*PI/180.d) << std::endl;
}

void coche::giroCoche(int teclaMovimiento){
    int tecMovSimplificada=0;
    int tecMovAntSimplificada=0;

    // comprobar si es el giro brusco
    if(teclaMovimiento==3 || teclaMovimiento==5 || teclaMovimiento==7)
        tecMovSimplificada=3;
    else if(teclaMovimiento==4 || teclaMovimiento==6 || teclaMovimiento==8)
        tecMovSimplificada=4;
    else
        tecMovSimplificada=teclaMovimiento;

    if(teclaMovimientoAnterior==3 || teclaMovimientoAnterior==5 || teclaMovimientoAnterior==7)
        tecMovAntSimplificada=3;
    else if(teclaMovimientoAnterior==4 || teclaMovimientoAnterior==6 || teclaMovimientoAnterior==8)
        tecMovAntSimplificada=4;
    else
        tecMovAntSimplificada=teclaMovimientoAnterior;

    if(tecMovAntSimplificada==tecMovSimplificada){
        if(giroBruscoClock.getElapsedTime().asMilliseconds()>1000)
            giroBrusco=true;
    }else{
        giroBrusco=false;
        giroBruscoClock.restart();
    }


    // realizar los giros
    if(giroBrusco!=true)
    {
        if(teclaMovimiento==0 || teclaMovimiento==1 || teclaMovimiento==2)
        {
            if(angulo==30){
                sprite.rotate(-rotacionCoche);
                angulo=0;
            }else if(angulo==-30){
                sprite.rotate(rotacionCoche);
                angulo=0;
            }else if(angulo==50){
                sprite.rotate(-20);
                angulo=30;
            }else if(angulo==-50){
                sprite.rotate(20);
                angulo=-30;
            }
        }
        else if(teclaMovimiento==3 || teclaMovimiento==5 || teclaMovimiento==7)
        {
            if(angulo==0){
                sprite.rotate(rotacionCoche);
                angulo=30;
            }else if(angulo==-30){
                sprite.rotate(rotacionCoche);
                angulo=0;
            }else if(angulo==-50){
                sprite.rotate(20);
                angulo=-30;
            }else if(angulo==50){
                sprite.rotate(-20);
                angulo=30;
            }
        }
        else if(teclaMovimiento==4 || teclaMovimiento==6 || teclaMovimiento==8)
        {
            if(angulo==0){
                sprite.rotate(-rotacionCoche);
                angulo=-30;
            }else if(angulo==30){
                sprite.rotate(-rotacionCoche);
                angulo=0;
            }else if(angulo==-50){
                sprite.rotate(20);
                angulo=-30;
            }else if(angulo==50){
                sprite.rotate(-20);
                angulo=30;
            }
        }
    }else{
        if(teclaMovimiento==3 || teclaMovimiento==5 || teclaMovimiento==7)
        {
            if(angulo==0)
                sprite.rotate(rotacionCoche+20);
            else if(angulo==-30)
                sprite.rotate(rotacionCoche*2+20);
            else if(angulo==30)
                sprite.rotate(20);
            angulo=50;
        }
        else if(teclaMovimiento==4 || teclaMovimiento==6 || teclaMovimiento==8)
        {
            if(angulo==0)
                sprite.rotate(-rotacionCoche-20);
            else if(angulo==-30)
                sprite.rotate(-20);
            else if(angulo==30)
                sprite.rotate(-rotacionCoche*2-20);
            angulo=-50;
        }
    }
}




void coche::comprobarRoturas(){
    int numPiezasRotas=0;
    if(neumaticoTrasRoto==true)
        numPiezasRotas++;
    if(neumaticoDelRoto==true)
        numPiezasRotas++;
    if(motorRoto==true)
        numPiezasRotas++;
    if(frenosRoto==true){
        frenado=1; // casi no puede frenar
    }else{
        frenado=6;
    }

    switch(numPiezasRotas){
        case 0: aceleracion=6;
                maxVelocidad=250.d;
            break;
        case 1: aceleracion=5.5;
                maxVelocidad=240.d;
            break;
        case 2: aceleracion=4.5;
                maxVelocidad=210.d;
            break;
        case 3: aceleracion=3.5;
                maxVelocidad=160.d;
            break;
    }
    //cout << aceleracion << " <-a | f-> " << frenado << endl;
}


void coche::comprobarMarchas(){
    int numPiezasRotas=0;
    float aceleracionMejor=0.0;
    if(neumaticoTrasRoto==true)
        numPiezasRotas++;
    if(neumaticoDelRoto==true)
        numPiezasRotas++;
    if(motorRoto==true)
        numPiezasRotas++;
    if(numPiezasRotas==0)
        aceleracionMejor=6;
    else if(numPiezasRotas==1)
        aceleracionMejor=5.5;
    else if(numPiezasRotas==2)
        aceleracionMejor=4.5;
    else if(numPiezasRotas==3)
        aceleracionMejor=3.5;

    //cout << aceleracion << " - " << velocidad << endl;
    if(velocidad<130){
        marcha=1;
        marcha_aceleracion=6;
    }else if(velocidad>=130 && velocidad<190){
        marcha=2;
        marcha_aceleracion=5;
    }else if(velocidad>=190 && velocidad<220){
        marcha=3;
        marcha_aceleracion=4;
    }else if(velocidad>=220){
        marcha=4;
        marcha_aceleracion=3;
    }

    if(aceleracionMejor>=marcha_aceleracion)
        aceleracion=marcha_aceleracion;
    //cout << aceleracion << " <-ma | ma ->" << marcha_aceleracion << endl;
}


void coche::comenzarAnimacionChoque(float choqueX, float choqueY){
    animacionChoque=true;
    animacionChoqueClock.restart();
    this->choqueX = choqueX;
    this->choqueY = choqueY;
}


void coche::rozamientoSuelo(){
    if(pisandoCesped==false){ // entra solo una vex cuando sale del cesped
        rozamiento=1.5;
    }else{
        int numPiezasRotas=0;
        if(neumaticoTrasRoto==true)
            numPiezasRotas++;
        if(neumaticoDelRoto==true)
            numPiezasRotas++;
        if(motorRoto==true)
            numPiezasRotas++;
        if(numPiezasRotas==0){
            maxVelocidad=140;
            rozamiento=3.5;
        }else if(numPiezasRotas==1){
            maxVelocidad=130;
            rozamiento=3;
        }else if(numPiezasRotas==2){
            maxVelocidad=110;
            rozamiento=2.5;
        }else{
            maxVelocidad=90;
            rozamiento=2;
        }
    }
}












