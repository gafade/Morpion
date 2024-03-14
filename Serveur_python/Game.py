import socket
from LinkClient import LinkClient, th
from numpy import random as rd
from Packet import Packet, Action

class Game:
    def __init__(self):
        self.lockListClient = th.Lock()
        self.listClient = []
        self.nbGames = 0
        self.dicGames = {}#joueur1,joueur2 
        self.tour ={}#turn order, qui le tour
        self.adressSocket = ("", 2100)
        self.socketServer = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.initConnection()

    def initConnection(self):
        self.socketServer.bind(self.adressSocket)
        self.socketServer.listen()
        


    def listenForNewClient(self):
        self.socketServer.listen(10)
        while True:
            try:
                socket_cree_pour_client, adressclient = self.socketServer.accept()
                self.listClient.append(LinkClient(socket_cree_pour_client, adressclient))
                print("gotcha",flush=True)
                
            except socket.timeout:
                print("miss",flush=True)
                pass
            
        
        
    def checkForNewGame(self):

        #while True:#to remove, on the chopping block
            if len(self.listClient) >= 2:
                print("game found",flush=True)
                self.lockListClient.acquire()
                self.nbGames += 1
                #THE NUMPY.RANDOM.RANDINT(a,b) IS EXCLUSIVE ON b
                indClient1 = rd.randint(0, len(self.listClient))
                indClient2 = rd.randint(0, len(self.listClient))
                while indClient2 == indClient1:
                    indClient2 = rd.randint(0, len(self.listClient))
                    print("Attributing Inds : 1-"+str(indClient1)+"   2-"+str(indClient2),flush=True)

                self.listClient[indClient1].pseudoOpponent = self.listClient[indClient2].pseudo
                self.listClient[indClient2].pseudoOpponent = self.listClient[indClient1].pseudo

                self.dicGames[self.nbGames] = [self.listClient[indClient1], self.listClient[indClient2]]#j1,j2
                self.tour[self.nbGames]=1#,a qui le tour vaut 1 ou 2
                self.listClient.pop(max(indClient1, indClient2))
                self.listClient.pop(min(indClient1, indClient2))

                for i in range(1, 3):
                    self.dicGames[self.nbGames][i - 1].idGame = self.nbGames
                    self.dicGames[self.nbGames][i - 1].numPlayer = i#1 or 2, same as self.tour
                    #debugs
                    self.dicGames[self.nbGames][i - 1].addMessageToSend("game started against someone")

                self.launchGame()
                self.lockListClient.release()

    def launchGame(self):
        for client in self.dicGames[self.nbGames]:
            packet = Packet(Action.LAUNCH_GAME,move=0, numPlayer=client.numPlayer, pseudoOpponent=client.pseudoOpponent)
            messageToSend = packet.encode()
            client.addMessageToSend(messageToSend)

    def updateMoves(self):
        #recieving the "newonMouseClick" through Linkclient.analyseMessage/LInkClient.lastmove
        if self.nbGames !=0:
            for client in self.dicGames[self.nbGames]:
                if (client.lastmove !=-1 and client.numPlayer==self.tour[self.nbGames]):
                    packet = Packet(Action.MOVE_PLAYED, numPlayer=client.numPlayer,pseudo=client.pseudo, pseudoOpponent=client.pseudoOpponent,
                                move=client.lastmove)
                    messageToSend = packet.encode()
                    self.tour[self.nbGames]=3-self.tour[self.nbGames]
                    #sending to both clients the move:
                    for receiver in self.dicGames[self.nbGames]:
                        receiver.addMessageToSend(messageToSend)
                        client.lastmove=-1
                    break#to prevent playing both at once
            
        #sending to Game.cpp.analyseMessage

    def checkForWin(self):
        pass

    def updateClientGameReady(self):
        pass
