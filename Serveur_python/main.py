from Game import Game

game = Game()

while True:
    game.listenForNewClient()
    game.checkForNewGame()
    game.updateMoves()
    game.checkForWin()