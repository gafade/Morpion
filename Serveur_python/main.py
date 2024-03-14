from Game import Game
import threading

game = Game()

mythread = threading.Thread(target=game.listenForNewClient,name='newClient',args=())
mythread.start()



#weird stuff Thread(target=function) needs staert to begin but
#             Thread(target=function()) doesn't need a .start??
print("Server started")
while True:
    #game.listenForNewClient()#is a blocking function -> get threaded
    game.checkForNewGame()
    game.updateMoves()
    game.checkForWin()