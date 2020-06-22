#include "Mapa.h"

Mapa::Mapa(coche *cocheJugador)
{
    finJuego=false;
    valorFinJuego=0; // 0->nada  1->gana 2->pierde
    this->cocheJugador = cocheJugador;
    if (!textura.loadFromFile("resources/tiledMapa2.png"))
    {
        std::cerr << "Error cargando la imagen tiledMapa2.png";
        exit(0);
    }
    mapa.setTexture(textura);
    mapa.setOrigin(2880/2,6080/2);
    mapa.setTextureRect(sf::IntRect(0*2880, 0*6080, 2880, 6080));
    mapa.setPosition(0, 0);


    if (!imagen.loadFromFile("resources/mapaCalor2.png"))
    {
        std::cerr << "Error cargando la imagen mapaCalorPrueba.png";
        exit(0);
    }
    if (!imagen2.loadFromFile("resources/mapaCalor1.png"))
    {
        std::cerr << "Error cargando la imagen mapaCalorPrueba.png";
        exit(0);
    }
    if (!textura2.loadFromFile("resources/tiledMapa1.png"))
    {
        std::cerr << "Error cargando la imagen tiledMapa2.png";
        exit(0);
    }
    mapa2.setTexture(textura2);
    mapa2.setOrigin(2880/2,6080/2);
    mapa2.setTextureRect(sf::IntRect(0*2880, 0*6080, 2880, 6080));
    mapa2.setPosition(0, -6080);


    if (!texturaRepostar.loadFromFile("resources/spritePitlane.png"))
    {
        std::cerr << "Error cargando la imagen spritePitlane.png";
        exit(0);
    }
    imagenRepostar.setTexture(texturaRepostar);
    imagenRepostar.setScale(0.77, 0.77);
    imagenRepostar.setOrigin(357/2,520/2);
    imagenRepostar.setTextureRect(sf::IntRect(0*357, 0*520, 357, 520));
    imagenRepostar.setPosition(0,0);


    actualizarY=3040.0;
    vuelta = 4; // numero de vueltas
    mapaActual=1;

    colorMapa.r=0;
    colorMapa.g=255;
    colorMapa.b=0;

    posicionMeta=3040-842-6080*2;
    choqueReloj.restart();

    entrarPitlaneReloj.restart();
    repostarReloj.restart();
    recalcularRozamiento=false;
    ultimaVueltaRepostada=6;
    repostando=false;
    excesoVelRepostando=false;
    tiempoRepostadoFinal=0.0;

    fuente.loadFromFile("resources/barcade-brawl.ttf");
    textoTiempoPit.setString("00:00:00");
    textoTiempoPit.setFont(fuente);
    textoTiempoPit.setCharacterSize(12);
    textoTiempoPit.setStyle(sf::Text::Bold);
    textoTiempoPit.setColor(sf::Color::White);
    textoTiempoPit.setPosition(0,0);

    textoTiempo.setString("00:00:00");
    textoTiempo.setFont(fuente);
    textoTiempo.setCharacterSize(10);
    textoTiempo.setStyle(sf::Text::Bold);
    textoTiempo.setColor(sf::Color::White);
    textoTiempo.setPosition(0,0);

    rectTextoTiempo.setSize(sf::Vector2f(100, 16));
    rectTextoTiempo.setFillColor(sf::Color::Black);
    rectTextoTiempo.setPosition(0, 0);
    mostrarTiempo=false;
    tiempoVuelta.restart();
    tiempoMostrandoTexto.restart();

    vectorCajas.clear();
    float lugarCaja = 0;
    srand(time(NULL));
    for(int i=0;i<vuelta;i++){
        CajaSorpresa *caja1 = new CajaSorpresa(-690,-4640+lugarCaja);
        CajaSorpresa *caja2 = new CajaSorpresa(-620,-4640+lugarCaja);
        vectorCajas.push_back(caja1);
        vectorCajas.push_back(caja2);
        lugarCaja-=12160;
    }
    potenciadorAnterior=0;
}

Mapa::~Mapa()
{
    //dtor
}


void Mapa::render(sf::RenderWindow *window){
    window->draw(mapa);
    window->draw(mapa2);

    if(mostrarTiempo==true){
        rectTextoTiempo.setPosition(cocheJugador->getSprite().getPosition().x-50,cocheJugador->getSprite().getPosition().y-120);
        window->draw(rectTextoTiempo);
        textoTiempo.setPosition(cocheJugador->getSprite().getPosition().x-50+10,cocheJugador->getSprite().getPosition().y-120+3);
        window->draw(textoTiempo);
    }

    for(int i=0;i<vectorCajas.size();i++){
        if(vectorCajas[i]!=nullptr)
            vectorCajas[i]->render(window);
    }
}

void Mapa::renderPitlane(sf::RenderWindow *window){
    if(repostando==true){
        window->draw(imagenRepostar);
        if(tiempoRepostadoFinal==0.0)
            window->draw(textoTiempoPit);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && repostarReloj.getElapsedTime().asMilliseconds()>500){
            repostando=false;
            cocheJugador->setRepostando(false);
            cocheJugador->setVelocidad(50);
            cocheJugador->comprobarRoturas();
            textoTiempo.setString(tiempoToString(repostarReloj));
            tiempoMostrandoTexto.restart();
            mostrarTiempo=true;
        }
    }
}


void Mapa::update(){
    int xCoche = cocheJugador->getSprite().getPosition().x;
    int yCoche = cocheJugador->getSprite().getPosition().y;


    if(xCoche+1440>=0 && yCoche+actualizarY>=0){
        if(mapaActual%2==1)
            colorMapa=imagen.getPixel(xCoche+1440,yCoche+actualizarY);
        else
            colorMapa=imagen2.getPixel(xCoche+1440,yCoche+actualizarY);

    }else{
        actualizarY=actualizarY+6080.0;
        mapaActual++;
        if(mapaActual%2==1)
            colorMapa=imagen.getPixel(xCoche+1440,yCoche+actualizarY);
        else
            colorMapa=imagen2.getPixel(xCoche+1440,yCoche+actualizarY);
    }
    //cout << "(" << xCoche << ", " << yCoche << ")" << endl;
    //cout << static_cast<int>(colorMapa.r) << " - " << static_cast<int>(colorMapa.g) << " - " << static_cast<int>(colorMapa.b) << endl;


    // cambio de imagen
    cambioImagen(yCoche);

    // detectar meta
    detectarMeta(yCoche);

    // detectar salida de pista
    salirPista(xCoche,yCoche);

    // detectar cesped
    detectarCesped();

    // detectar pitlane
    detectarPitlane();
    if(repostando==true)
        gestionRespostar();

    // comprobar si finaliza la carrera y el juego
    if(valorFinJuego==0)
        finCarrera();
    if(delayFinJuego.getElapsedTime().asMilliseconds()>2000 && valorFinJuego!=0){
        finJuego=true;
    }

    if(tiempoMostrandoTexto.getElapsedTime().asMilliseconds()>3000){
        mostrarTiempo=false;
    }

    cogerPotenciador();
    potenciadorPosicion(xCoche, yCoche);
    potenciadorAnterior=cocheJugador->getPotenciador();
}


void Mapa::cambioImagen(int yMapa){
    if(mapaActual%2==0 && mapa.getPosition().y > mapa2.getPosition().y && yMapa+actualizarY<4000){
        mapa.setPosition(0,-actualizarY-3040.0);
    }

    if(mapaActual%2==1 && mapa2.getPosition().y > mapa.getPosition().y && yMapa+actualizarY<4000){
        mapa2.setPosition(0,-actualizarY-3040.0);
    }
}


void Mapa::detectarMeta(int y){
    if(posicionMeta>y){
        posicionMeta-=6080*2;
        vuelta--;
        textoTiempo.setString(tiempoToString(tiempoVuelta));
        tiempoMostrandoTexto.restart();
        mostrarTiempo=true;
        tiempoVuelta.restart();
        //cout << vuelta << endl;
    }
}


void Mapa::detectarCesped(){
    if(static_cast<int>(colorMapa.r)==255 && static_cast<int>(colorMapa.g)==0 && static_cast<int>(colorMapa.b)==0){
        cocheJugador->setPisandoCesped(true);
        recalcularRozamiento=true;
        //cout << "pisa cesped" << endl;
    }else if(recalcularRozamiento==true){
        cocheJugador->setPisandoCesped(false);
        cocheJugador->comprobarRoturas();
        recalcularRozamiento=false;
        //cout << "entraaa" << endl;
    }
}



void Mapa::detectarPitlane(){
    if(static_cast<int>(colorMapa.r)==0 && static_cast<int>(colorMapa.g)==0 && static_cast<int>(colorMapa.b)==255){
        if(entrarPitlaneReloj.getElapsedTime().asMilliseconds()>500 && ultimaVueltaRepostada>vuelta && cocheJugador->getAnimacionChoque()==false && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            if(cocheJugador->getVelocidad()>100)
                excesoVelRepostando=true;
            cocheJugador->setRepostando(true);
            cout << "empieza a repostar" << endl;
            ultimaVueltaRepostada=vuelta;
            imagenRepostar.setTextureRect(sf::IntRect(0*357, 0*520, 357, 520));
            repostando=true;
            tiempoRepostadoFinal=0.0;
            textoTiempoPit.setString("00:00:00");
            cocheJugador->setVelocidad(0.d);
            repostarReloj.restart();
            repostarPiezaReloj.restart();
        }
    }else{
        entrarPitlaneReloj.restart();
    }
}


// gestion del tiempo en pitlane, ir actualizando la gasolina y arreglar las piezas
void Mapa::gestionRespostar(){
    imagenRepostar.setPosition(cocheJugador->getSprite().getPosition().x,cocheJugador->getSprite().getPosition().y);
    textoTiempoPit.setPosition(cocheJugador->getSprite().getPosition().x+10,cocheJugador->getSprite().getPosition().y-65);
    // actualizar el reloj

    textoTiempoPit.setString(tiempoToString(repostarReloj));

    if(repostarPiezaReloj.getElapsedTime().asMilliseconds()>2000 && tiempoRepostadoFinal==0.0 && excesoVelRepostando==true){
        excesoVelRepostando=false;
        repostando=false;
        cocheJugador->setRepostando(false);
        cocheJugador->setVelocidad(150);
    }else if(repostarPiezaReloj.getElapsedTime().asMilliseconds()>2000 && tiempoRepostadoFinal==0.0){
        float gasolina=cocheJugador->getGasolina();
        if(gasolina+10.0<=70.0){
            cocheJugador->setGasolina(gasolina+10.0);
        }else if(gasolina+10.0>70.0 && gasolina+10.0<80.0){
            cocheJugador->setGasolina(70.0);
        }else{
            if(cocheJugador->getNeumDelRoto()==true)
                cocheJugador->setNeumDelRoto(false);
            else if(cocheJugador->getNeumTrasRoto()==true)
                cocheJugador->setNeumTrasRoto(false);
            else if(cocheJugador->getFrenosRoto()==true)
                cocheJugador->setFrenosRoto(false);
            else if(cocheJugador->getMotorRoto()==true)
                cocheJugador->setMotorRoto(false);
            else{
                imagenRepostar.setTextureRect(sf::IntRect(1*357, 0*520, 357, 520));
                tiempoRepostadoFinal=repostarReloj.getElapsedTime().asMilliseconds();
                cout << tiempoRepostadoFinal << endl;
            }
        }
        repostarPiezaReloj.restart();
    }
}


void Mapa::finCarrera(){
    if(vuelta==0){
        // acabar bien carrera
        valorFinJuego=1;
        delayFinJuego.restart();
    }else if(cocheJugador->getGasolina()<=0.0 && cocheJugador->getVelocidad()<=0 && repostando==false){
        // acabar mal carrera
        valorFinJuego=2;
        delayFinJuego.restart();
    }
    // en caso de que active god mode justo al acabar la gasolina
    if(vuelta>0 && valorFinJuego!=0 && cocheJugador->getGodMode()==true)
        valorFinJuego=0;
}


string Mapa::tiempoToString(sf::Clock relojTiempo){
    long tiempoPitActual=relojTiempo.getElapsedTime().asSeconds();
    long tiempoPitActualMil=relojTiempo.getElapsedTime().asMilliseconds();
    int segundos = int(tiempoPitActual%60);
    int divMin = tiempoPitActual/60.0;
    int minutos = int(divMin%60);

    int divSeg = int(tiempoPitActualMil%1000);
    int milisegundos = divSeg/10;
    string minu,seg,mil,tiempo="";
    if(minutos<10)
        minu="0"+to_string(minutos);
    else
        minu=to_string(minutos);
    if(segundos<10)
        seg="0"+to_string(segundos);
    else
        seg=to_string(segundos);
    if(milisegundos<10)
        mil="0"+to_string(milisegundos);
    else
        mil=to_string(milisegundos);
    tiempo = minu+":"+seg+":"+mil;
    return tiempo;
}


void Mapa::salirPista(int xCoche, int yCoche){
    if(static_cast<int>(colorMapa.r)==255 && static_cast<int>(colorMapa.g)==255 && static_cast<int>(colorMapa.b)==255){
        // animacion choque
        if(cocheJugador->getAnimacionChoque()==false && choqueReloj.getElapsedTime().asSeconds()>3){
            choqueReloj.restart();
            bool encontrado=false;
            float posBuena=130; // posicion que se deplaza al chocar
            float posAPonerX=0;
            float posAPonerY=0;
            sf::Color pixelChoque;

            for(int i=xCoche+1440;i<2880 && encontrado!=true; i+=16){
                //cout << i << " - " << yCoche+actualizarY << endl;
                if(mapaActual%2==1){
                    if(yCoche+actualizarY-posBuena>=0)
                        pixelChoque=imagen.getPixel(i,yCoche+actualizarY-posBuena);
                    else
                        pixelChoque=imagen2.getPixel(i,yCoche+actualizarY-posBuena+6080);
                }else{
                    if(yCoche+actualizarY-posBuena>=0)
                        pixelChoque=imagen2.getPixel(i,yCoche+actualizarY-posBuena);
                    else
                        pixelChoque=imagen.getPixel(i,yCoche+actualizarY-posBuena+6080);
                }

                if((static_cast<int>(pixelChoque.r)==0 && static_cast<int>(pixelChoque.g)==255 && static_cast<int>(pixelChoque.b)==0) || (static_cast<int>(pixelChoque.r)==255 && static_cast<int>(pixelChoque.g)==0 && static_cast<int>(pixelChoque.b)==255)){
                    encontrado=true;
                    //cout << i << " - " << yCoche << endl;
                    posAPonerX = i-1440+16;
                    posAPonerY = yCoche-posBuena;
                }
            }
            if(encontrado==false){
                for(int i=xCoche+1440;i>=0 && encontrado!=true; i-=16){
                    if(mapaActual%2==1){
                        if(yCoche+actualizarY-posBuena>=0)
                            pixelChoque=imagen.getPixel(i,yCoche+actualizarY-posBuena);
                        else
                            pixelChoque=imagen2.getPixel(i,yCoche+actualizarY-posBuena+6080);
                    }else{
                        if(yCoche+actualizarY-posBuena>=0)
                            pixelChoque=imagen2.getPixel(i,yCoche+actualizarY-posBuena);
                        else
                            pixelChoque=imagen.getPixel(i,yCoche+actualizarY-posBuena+6080);
                    }

                    if((static_cast<int>(pixelChoque.r)==0 && static_cast<int>(pixelChoque.g)==255 && static_cast<int>(pixelChoque.b)==0) || (static_cast<int>(pixelChoque.r)==255 && static_cast<int>(pixelChoque.g)==0 && static_cast<int>(pixelChoque.b)==255)){
                        encontrado=true;
                        posAPonerX = i-1440-16;
                        posAPonerY = yCoche-posBuena;
                        //cout << i << " - " << yCoche << endl;
                    }
                }
            }

            cocheJugador->comenzarAnimacionChoque(posAPonerX,posAPonerY);
        }
        //cout << "se estrella" << endl;
    }
}


void Mapa::cogerPotenciador(){
    for(int i=0;i<vectorCajas.size();i++){
        if(vectorCajas[i]!=nullptr && cocheJugador->getSprite().getGlobalBounds().intersects(vectorCajas[i]->getSprite().getGlobalBounds())){
            if(cocheJugador->getPotenciador()==0){
                cocheJugador->setPotenciador(vectorCajas[i]->getNumPotenciador());
            }
            delete(vectorCajas[i]);
            vectorCajas[i]=nullptr;
        }
    }
}


void Mapa::potenciadorPosicion(int posX, int posY){
    if(cocheJugador->getPotenciador()!=1 && potenciadorAnterior==1 && cocheJugador->getAnimacionChoque()!=true && cocheJugador->getRepostando()!=true){
        bool encontrado=false;
        float posAPonerX=posX;
        sf::Color pixelChoque;

        for(int i=0;i<2880 && encontrado!=true; i+=1){
            //cout << i << " - " << yCoche+actualizarY << endl;
            if(mapaActual%2==1){
                if(posY+actualizarY>=0)
                    pixelChoque=imagen.getPixel(i,posY+actualizarY);
                else
                    pixelChoque=imagen2.getPixel(i,posY+actualizarY+6080);
            }else{
                if(posY+actualizarY>=0)
                    pixelChoque=imagen2.getPixel(i,posY+actualizarY);
                else
                    pixelChoque=imagen.getPixel(i,posY+actualizarY+6080);
            }

            if((static_cast<int>(pixelChoque.r)==0 && static_cast<int>(pixelChoque.g)==255 && static_cast<int>(pixelChoque.b)==255)){
                encontrado=true;
                //cout << i << " - " << yCoche << endl;
                posAPonerX = i-1440;
                //cout << posAPonerX << " -> (" << posX << " , " << posY << ") -->  "  << (posAPonerX-posX) << endl;
            }
        }
        cocheJugador->setPosSigX(float(posAPonerX));
    }
}









