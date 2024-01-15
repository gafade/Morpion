#include "Game.h"
#include "PacketClient.h"

int main()
{
    // Init game
    Game game;

    //tests

    PacketClient p;
    string pok = "2:76:98:TUVWKCUX:vtyvwbl";
    p.decode("1:5:278:k:m:opj");
    cout << p.encode() << endl;

    p.decode(pok);
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