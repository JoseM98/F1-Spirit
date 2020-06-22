#include "juego.h"


const double VELOCIDAD_ENEMIGO_1 = 800.0;
const double UPDATE_TICK_TIME=1000.0/15.0;

juego::juego()
{
    //ctor
}


juego::juego(int resolucionX, int resolucionY, std::string  titulo)
{
    window = new sf::RenderWindow(sf::VideoMode(resolucionX, resolucionY), titulo);
    window->setFramerateLimit(60);

    //view(sf::FloatRect(0, 0, resolucionX, resolucionY));
    viewHud.setViewport(sf::FloatRect(0.5, 0.0, 1.0, 1.0));
    viewHud.setCenter(resolucionX/2, resolucionY/2);
    viewHud.setSize(resolucionX, resolucionY);

    viewJuego.setViewport(sf::FloatRect(0.025, 0.025, 0.475, 0.95));
    viewJuego.setCenter(resolucionX/2, resolucionY/2);
    viewJuego.setSize(resolucionX*(4.75/10.d)*0.7, resolucionY*(9.5/10.d)*0.7);
    //window->setView(viewJuego);

    // view fin partida
    viewfinJuego.setViewport(sf::FloatRect(0.0, 0.0, 1.0, 1.0));
    viewfinJuego.setCenter(resolucionX/2, resolucionY/2);
    viewfinJuego.setSize(resolucionX, resolucionY);
    fuenteFin.loadFromFile("resources/barcade-brawl.ttf");
    textoFin.setString("0");
    textoFin.setFont(fuenteFin);
    textoFin.setCharacterSize(20);
    textoFin.setStyle(sf::Text::Bold);
    textoFin.setColor(sf::Color::White);
    textoFin.setPosition(200,200);

    window->setView(viewHud);
    //dibujarPantalla();
    gameloop();
}


void juego::dibujarPantalla()
{
    //window->clear();
    //window->display();
}



void juego::gameloop()
{
    sf::Clock updateClock;
    sf::Time timeElapsed;



    int teclaMovimiento=0;
    int teclaPotenciador=0;
    double percentTick = 0.0;   // tiempo transcurrido desde el ultimo update

    updateClock.restart();
    coche *cocheJugador = new coche();

    Mapa *mapa = new Mapa(cocheJugador);
    cout << "Coches enemigos: " << endl;
    srand(time(NULL));
    CocheEnemigo *cocheEnemigo1 = new CocheEnemigo(mapa,-155, 2310);
    vectorEnemigos.push_back(cocheEnemigo1);
    CocheEnemigo *cocheEnemigo2 = new CocheEnemigo(mapa,-101, 2295);
    vectorEnemigos.push_back(cocheEnemigo2);
    CocheEnemigo *cocheEnemigo3 = new CocheEnemigo(mapa,-155, 2247);
    vectorEnemigos.push_back(cocheEnemigo3);
    CocheEnemigo *cocheEnemigo4 = new CocheEnemigo(mapa,-101, 2233);
    vectorEnemigos.push_back(cocheEnemigo4);

    Hud *hud = new Hud(cocheJugador, mapa, vectorEnemigos);



    while(window->isOpen())
    {

        /*** LEER INPUT ****/
        sf::Event event;
        while (window->pollEvent(event))
        {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                window->close();
            }
        }
        teclaMovimiento=tecla();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
            teclaPotenciador=1;
        else
            teclaPotenciador=0;

        if(mapa->getFinJuego()!=true){
            /***** UPDATE INTERPOLADO ****/
            if(updateClock.getElapsedTime().asMilliseconds() > UPDATE_TICK_TIME){
                timeElapsed=updateClock.restart();  // este valor hay que usarlo para calcular el movimiento de forma correcta timeElapsed.asSeconds()
                cocheJugador->update(teclaMovimiento, teclaPotenciador,timeElapsed);
                hud->update();
                for(int i=0; i<vectorEnemigos.size();i++)
                    vectorEnemigos[i]->update(timeElapsed);

                teclaMovimiento=0;

                mapa->update();
            }

            /***** RENDER **********/
            percentTick = std::min(1.0,(updateClock.getElapsedTime().asMilliseconds())/(UPDATE_TICK_TIME*1.d));
            cocheJugador->render(percentTick, viewJuego);
            for(int i=0; i<vectorEnemigos.size();i++)
                vectorEnemigos[i]->render(percentTick);



            window->clear();
            window->setView(viewHud);
            hud->render(window);
            window->setView(viewJuego);
            mapa->render(window);
            //window->setView(viewHud);
            for(int i=0; i<vectorEnemigos.size();i++)
                vectorEnemigos[i]->draw(window);
            cocheJugador->draw(window);
            mapa->renderPitlane(window);
            reiniciarJuego.restart();
        }else{
            mostrarPantallaFin(mapa);

            if(reiniciarJuego.getElapsedTime().asMilliseconds()>2000){
                delete(cocheJugador);
                cocheJugador=nullptr;
                cocheJugador = new coche();
                delete(mapa);
                mapa=nullptr;
                mapa = new Mapa(cocheJugador);

                for(int i=0;i<vectorEnemigos.size();i++){
                    delete(vectorEnemigos[i]);
                    vectorEnemigos[i]=nullptr;
                }
                vectorEnemigos.clear();

                cout << vectorEnemigos.size() << endl;
                srand(time(NULL));
                cocheEnemigo1 = new CocheEnemigo(mapa,-155, 2310);
                vectorEnemigos.push_back(cocheEnemigo1);
                cocheEnemigo2 = new CocheEnemigo(mapa,-101, 2295);
                vectorEnemigos.push_back(cocheEnemigo2);
                cocheEnemigo3 = new CocheEnemigo(mapa,-155, 2247);
                vectorEnemigos.push_back(cocheEnemigo3);
                cocheEnemigo4 = new CocheEnemigo(mapa,-101, 2233);
                vectorEnemigos.push_back(cocheEnemigo4);

                delete(hud);
                hud=nullptr;
                hud = new Hud(cocheJugador, mapa, vectorEnemigos);
                updateClock.restart();
                reiniciarJuego.restart();
            }
        }




        window->display();
    }
}


int juego::tecla()
{
    int valTecMov = 0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        valTecMov = 0;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        valTecMov = 1;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        valTecMov = 2;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        valTecMov = 3;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        valTecMov = 4;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        valTecMov = 0;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        valTecMov = 0;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        valTecMov = 5;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        valTecMov = 6;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        valTecMov = 7;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        valTecMov = 8;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        valTecMov = 1;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        valTecMov = 2;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        valTecMov = 3;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        valTecMov = 4;

    return valTecMov;
}

void juego::keyloop()
{

}


juego::~juego()
{
    //dtor
}


void juego::mostrarPantallaFin(Mapa *mapa){
    window->clear();
    window->setView(viewfinJuego);
    if(mapa->getvalorFinJuego()==1){
        textoFin.setPosition(150,220);
        textoFin.setString("Has acabado la carrera :P");
    }else{
        textoFin.setPosition(130,220);
        textoFin.setString("Fin del juego: has perdido");
    }
    window->draw(textoFin);
}

void juego::resetearJuego(coche *cocheJugador, Mapa *mapa, Hud *hud){

    cocheJugador = new coche();

    mapa = new Mapa(cocheJugador);

    for(int i=0;i<vectorEnemigos.size();i++){
        delete(vectorEnemigos[i]);
        vectorEnemigos.erase(vectorEnemigos.begin()+i);
    }
    cout << vectorEnemigos.size() << endl;
    srand(time(NULL));
    CocheEnemigo *cocheEnemigo1 = new CocheEnemigo(mapa,-155, 2310);
    vectorEnemigos.push_back(cocheEnemigo1);
    CocheEnemigo *cocheEnemigo2 = new CocheEnemigo(mapa,-101, 2295);
    vectorEnemigos.push_back(cocheEnemigo2);
    CocheEnemigo *cocheEnemigo3 = new CocheEnemigo(mapa,-155, 2247);
    vectorEnemigos.push_back(cocheEnemigo3);
    CocheEnemigo *cocheEnemigo4 = new CocheEnemigo(mapa,-101, 2233);
    vectorEnemigos.push_back(cocheEnemigo4);

    hud = new Hud(cocheJugador, mapa, vectorEnemigos);
}







