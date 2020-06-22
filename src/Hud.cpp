#include "Hud.h"

Hud::Hud(coche *cocheJugador, Mapa *mapa, std::vector<CocheEnemigo*> vectorEne)
{
    this->cocheJugador = cocheJugador;
    this->mapa = mapa;
    actualizarAceleracion.restart();
    aceleracionAnterior=0;
    vectorEnemigos=vectorEne;
    posicion=1;

    if (!textura.loadFromFile("resources/hud.png"))
    {
        std::cerr << "Error cargando la imagen hud.png";
        exit(0);
    }
    spriteFondo = new sf::Sprite(textura);
    spriteFondo->setOrigin(320/2,510/2);
    spriteFondo->setTextureRect(sf::IntRect(0*320, 0*510, 320, 510));
    spriteFondo->setScale(1,1);
    spriteFondo->setPosition(200, 300);


    if (!texturaCircuito.loadFromFile("resources/circuito2.png"))
    {
        std::cerr << "Error cargando la imagen circuito.png";
        exit(0);
    }
    spriteCircuito = new sf::Sprite(texturaCircuito);
    spriteCircuito->setOrigin(180/2,114/2);
    spriteCircuito->setTextureRect(sf::IntRect(0*180, 0*114, 180, 114));
    spriteCircuito->setScale(1,1);
    spriteCircuito->setPosition(190, 130);

    // cuadraditos gasolina
    int posXCuad = 140;
    int posYCuad = 378;
    for(int i=0;i<7;i++){
        sf::RectangleShape rectangulo;
        rectangulo.setSize(sf::Vector2f(22, 10));
        rectangulo.setFillColor(sf::Color::Red);
        rectangulo.setPosition(posXCuad, posYCuad);
        vectorGasolina.push_back(rectangulo);
        posXCuad+=27;
    }

    // cuadraditos aceleracion
    int posXAc = 140;
    int posYAc = 263;
    for(int i=0;i<8;i++){
        sf::RectangleShape rectangulo;
        rectangulo.setSize(sf::Vector2f(22, 7));
        rectangulo.setFillColor(sf::Color::Red);
        rectangulo.setPosition(posXAc, posYAc);
        vectorAceleracion.push_back(rectangulo);
        posXAc+=27;
    }

    font.loadFromFile("resources/ARCADECLASSIC.TTF");
    textVelocidad.setString("0");
    textVelocidad.setFont(font);
    textVelocidad.setCharacterSize(40);
    textVelocidad.setStyle(sf::Text::Bold);
    textVelocidad.setColor(sf::Color::White);
    textVelocidad.setPosition(170,260);


    textMarcha.setString("0");
    textMarcha.setFont(font);
    textMarcha.setCharacterSize(30);
    textMarcha.setStyle(sf::Text::Bold);
    textMarcha.setColor(sf::Color::White);
    textMarcha.setPosition(290,315);

    textRun.setString("RUN");
    textRun.setFont(font);
    textRun.setCharacterSize(24);
    textRun.setStyle(sf::Text::Bold);
    textRun.setColor(sf::Color::Red);
    textRun.setPosition(85,230);

    vuelta.setString("5");
    vuelta.setFont(font);
    vuelta.setCharacterSize(40);
    vuelta.setStyle(sf::Text::Bold);
    vuelta.setColor(sf::Color::White);
    vuelta.setPosition(320,160);

    modoDios.setString("Modo  dios  activado");
    modoDios.setFont(font);
    modoDios.setCharacterSize(16);
    modoDios.setColor(sf::Color::Yellow);
    modoDios.setPosition(130,560);

    textoPosicion.setString("5");
    textoPosicion.setFont(font);
    textoPosicion.setCharacterSize(30);
    textoPosicion.setStyle(sf::Text::Bold);
    textoPosicion.setColor(sf::Color::White);
    textoPosicion.setPosition(277,290);


    cuadradosGasolina=7;
    velocidad=0;
    parpadeo.restart();

    // Rect izq
    neumDelRect.setSize(sf::Vector2f(19, 7));
    neumDelRect.setFillColor(sf::Color::Yellow);
    neumDelRect.setOutlineColor(sf::Color::Red);
    neumDelRect.setOutlineThickness(3);
    neumDelRect.setPosition(313, 447);
    // Rect der
    frenoRect.setSize(sf::Vector2f(19, 7));
    frenoRect.setFillColor(sf::Color::Yellow);
    frenoRect.setOutlineColor(sf::Color::Red);
    frenoRect.setOutlineThickness(3);
    frenoRect.setPosition(313, 470);
    // Rect freno
    motorRect.setSize(sf::Vector2f(19, 7));
    motorRect.setFillColor(sf::Color::Yellow);
    motorRect.setOutlineColor(sf::Color::Red);
    motorRect.setOutlineThickness(3);
    motorRect.setPosition(313, 493);
    // Rect motor
    neumTrasRect.setSize(sf::Vector2f(19, 7));
    neumTrasRect.setFillColor(sf::Color::Yellow);
    neumTrasRect.setOutlineColor(sf::Color::Red);
    neumTrasRect.setOutlineThickness(3);
    neumTrasRect.setPosition(313, 516);

    // circulo jugador minimapa
    circuloJugMapa.setRadius(5);
    circuloJugMapa.setFillColor(sf::Color::White);
    circuloJugMapa.setPosition(195, 72);

    // potenciador
    if (!textPotenciador.loadFromFile("resources/potenciadores.png"))
    {
        std::cerr << "Error cargando la imagen potenciadores.png";
        exit(0);
    }
    spritePotenciador.setTexture(textPotenciador);
    spritePotenciador.setOrigin(128/2,64/2);
    spritePotenciador.setTextureRect(sf::IntRect(0*64, 0*64, 64, 64));
    spritePotenciador.setScale(0.5,0.5);
    spritePotenciador.setPosition(90, 185);
}

Hud::~Hud()
{
    //dtor
}

void Hud::render(sf::RenderWindow *window){
    window->draw(*spriteFondo);

    // gasolina
    for(int i=0;i<cuadradosGasolina;i++){
        window->draw(vectorGasolina[i]);
    }

    // aceleracion
    if(actualizarAceleracion.getElapsedTime().asMilliseconds()>300){
        if(cocheJugador->getAceleracion()/1 > aceleracionAnterior)
            aceleracionAnterior++;
        else if(cocheJugador->getAceleracion()/1 < aceleracionAnterior)
            aceleracionAnterior--;
        actualizarAceleracion.restart();
    }
    if(aceleracionAnterior>7)
        aceleracionAnterior=7;
    for(int i=0;i<aceleracionAnterior;i++){
        window->draw(vectorAceleracion[i]);
    }

    //texto velocidad
    textVelocidad.setString(to_string(velocidad));
    window->draw(textVelocidad);

    //texto marcha
    textMarcha.setString(to_string(cocheJugador->getMarcha()));
    window->draw(textMarcha);

    //texto vuelta
    vuelta.setString(to_string(mapa->getNumVuelta()));
    window->draw(vuelta);

    // roturas
    if(parpadeo.getElapsedTime().asMilliseconds()<1000.0){
        window->draw(textRun);
    }else if(parpadeo.getElapsedTime().asMilliseconds()>=1000.0 && parpadeo.getElapsedTime().asMilliseconds()<2000){
        if(cocheJugador->getNeumDelRoto()==true)
            window->draw(neumDelRect);
        if(cocheJugador->getNeumTrasRoto()==true)
            window->draw(neumTrasRect);
        if(cocheJugador->getMotorRoto()==true)
            window->draw(motorRect);
        if(cocheJugador->getFrenosRoto()==true)
            window->draw(frenoRect);
    }else if(parpadeo.getElapsedTime().asMilliseconds()>2000.0){
        parpadeo.restart();
    }

    if(cocheJugador->getGodMode()==true)
        window->draw(modoDios);

    window->draw(*spriteCircuito);
    window->draw(circuloJugMapa);
    window->draw(textoPosicion);

    if(cocheJugador->getPotenciador()==1){
        spritePotenciador.setTextureRect(sf::IntRect(0*64, 0*64, 64, 64));
        window->draw(spritePotenciador);
    }else if(cocheJugador->getPotenciador()==2){
        spritePotenciador.setTextureRect(sf::IntRect(1*64, 0*64, 64, 64));
        window->draw(spritePotenciador);
    }
}


void Hud::update(){

    calcularCuadradosGasolina();
    calcularVelocidad();
    calcularCirculoMinimapa();


    if(mapa->getNumVuelta()>0){
        posicion=1;
        for(int i=0;i<vectorEnemigos.size();i++){
            if(vectorEnemigos[i]->getSprite().getPosition().y<cocheJugador->getSprite().getPosition().y)
                posicion++;
        }
        textoPosicion.setString(to_string(posicion));
    }
}

void Hud::calcularVelocidad(){

    velocidad = int(cocheJugador->getVelocidad());
    //cout << cocheJugador->getVelocidad() << endl;

}

void Hud::calcularCuadradosGasolina(){
    if(cocheJugador->getGasolina()>60)
        cuadradosGasolina=7;
    else if(cocheJugador->getGasolina()>50)
        cuadradosGasolina=6;
    else if(cocheJugador->getGasolina()>40)
        cuadradosGasolina=5;
    else if(cocheJugador->getGasolina()>30)
        cuadradosGasolina=4;
    else if(cocheJugador->getGasolina()>20)
        cuadradosGasolina=3;
    else if(cocheJugador->getGasolina()>10)
        cuadradosGasolina=2;
    else if(cocheJugador->getGasolina()>0)
        cuadradosGasolina=1;
    else
        cuadradosGasolina=0;
}



void Hud::calcularCirculoMinimapa(){
    long yCoche=2440-cocheJugador->getSprite().getPosition().y;
    long yCocheAbs= yCoche%12160;

    //cout << yCocheAbs << endl;
    if(yCocheAbs>=631 && yCocheAbs<751) // c1
        circuloJugMapa.setPosition(235,72);
    else if(yCocheAbs>=751 && yCocheAbs<871)
        circuloJugMapa.setPosition(250,76);
    else if(yCocheAbs>=871 && yCocheAbs<991)
        circuloJugMapa.setPosition(258,78);
    else if(yCocheAbs>=991 && yCocheAbs<1111)
        circuloJugMapa.setPosition(264,84);
    else if(yCocheAbs>=1111 && yCocheAbs<1231)
        circuloJugMapa.setPosition(271,90);
    else if(yCocheAbs>=1231 && yCocheAbs<1381) //r2
        circuloJugMapa.setPosition(271,98);
    else if(yCocheAbs>=1381 && yCocheAbs<1521)
        circuloJugMapa.setPosition(271,105);
    else if(yCocheAbs>=1521 && yCocheAbs<1671)
        circuloJugMapa.setPosition(271,114);
    else if(yCocheAbs>=1671 && yCocheAbs<1971) //c2
        circuloJugMapa.setPosition(265,130);
    else if(yCocheAbs>=1971 && yCocheAbs<2261)
        circuloJugMapa.setPosition(255,137);
    else if(yCocheAbs>=2261 && yCocheAbs<2511) //r3
        circuloJugMapa.setPosition(245,137);
    else if(yCocheAbs>=2511 && yCocheAbs<2731)
        circuloJugMapa.setPosition(235,140);
    else if(yCocheAbs>=2731 && yCocheAbs<3561) // c3
        circuloJugMapa.setPosition(222,145);
    else if(yCocheAbs>=3561 && yCocheAbs<3961) // r4
        circuloJugMapa.setPosition(217,155);
    else if(yCocheAbs>=3961 && yCocheAbs<4581) // c4
        circuloJugMapa.setPosition(213,170);
    else if(yCocheAbs>=4581 && yCocheAbs<4681) // r5
        circuloJugMapa.setPosition(203,179);
    else if(yCocheAbs>=4681 && yCocheAbs<5111) // c5
        circuloJugMapa.setPosition(190,169);
    else if(yCocheAbs>=5111 && yCocheAbs<5561) // r6
        circuloJugMapa.setPosition(185,155);
    else if(yCocheAbs>=5561 && yCocheAbs<6061) // c6
        circuloJugMapa.setPosition(182,140);
    else if(yCocheAbs>=6061 && yCocheAbs<6661)
        circuloJugMapa.setPosition(170,125);
    else if(yCocheAbs>=6661 && yCocheAbs<6881) // r7
        circuloJugMapa.setPosition(158,118);
    else if(yCocheAbs>=6881 && yCocheAbs<7101)
        circuloJugMapa.setPosition(140,115);
    else if(yCocheAbs>=7101 && yCocheAbs<7321)
        circuloJugMapa.setPosition(130,115);
    else if(yCocheAbs>=7321 && yCocheAbs<8501) // c7
        circuloJugMapa.setPosition(105,107);
    else if(yCocheAbs>=8501 && yCocheAbs<8901) // r8
        circuloJugMapa.setPosition(100,97);
    else if(yCocheAbs>=8901 && yCocheAbs<9301)
        circuloJugMapa.setPosition(99,91);
    else if(yCocheAbs>=9301 && yCocheAbs<9911) // c8
        circuloJugMapa.setPosition(107,76);
    else if(yCocheAbs>=9911 && yCocheAbs<10301) // r9
        circuloJugMapa.setPosition(120,80);
    else if(yCocheAbs>=10301 && yCocheAbs<10641) // c9
        circuloJugMapa.setPosition(126,83);
    else if(yCocheAbs>=10641 && yCocheAbs<11081)
        circuloJugMapa.setPosition(140,80);
    else if(yCocheAbs>=11081 && yCocheAbs<11221) // r1
        circuloJugMapa.setPosition(145,72);
    else if(yCocheAbs>=11221 && yCocheAbs<11441)
        circuloJugMapa.setPosition(155,72);
    else if(yCocheAbs>=11441 && yCocheAbs<11641)
        circuloJugMapa.setPosition(165,72);
    else if(yCocheAbs>=11641 && yCocheAbs<11851)
        circuloJugMapa.setPosition(176,72);
    else if(yCocheAbs>=11851 && yCocheAbs<12051)
        circuloJugMapa.setPosition(187,72);
    else if(yCocheAbs>=12051 || yCocheAbs<251)
        circuloJugMapa.setPosition(198,72);
    else if(yCocheAbs>=251 && yCocheAbs<451)
        circuloJugMapa.setPosition(210,72);
    else if(yCocheAbs>=451 && yCocheAbs<631)
        circuloJugMapa.setPosition(220,72);
}








