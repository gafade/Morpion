#include "Game.h"
#include "PacketClient.h"

int main()
{
    // Init game
    Game game;

    //tests

    PacketClient p;
    
    p.decode("1:5:278:k:m:opj");
    cout << p.encode() << endl;

    /*
    while (game.running())
    {
        // Update
        game.update();

        // Render
        game.render();
    }
    */
    return 0;
}