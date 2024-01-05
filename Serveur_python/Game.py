import socket
from LinkClient import LinkClient, th
from numpy import random as rd
from Packet import Packet, Action


class Game:
    def __init__(self):
        self.lockListClient = th.Lock()
        self.listClient = []
        self.nbGames = 0
        self.dicGames = {}
        self.adressSocket = ("", 2100)
        self.socketServer = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.initConnection()

    def initConnection(self):
        self.socketServer.bind(self.adressSocket)
        self.socketServer.listen()

    def listenForNewClient(self):
        socket_cree_pour_client, adressclient = self.socketServer.accept()
        self.listClient.append(LinkClient(socket_cree_pour_client, adressclient))

    def checkForNewGame(self):

        while True:
            if len(self.listClient) > 2:
                self.lockListClient.acquire()
                self.nbGames += 1
                indClient1 = rd.randint(0, len(self.listClient) - 1)
                indClient2 = rd.randint(0, len(self.listClient) - 1)
                while indClient2 == indClient1:
                    indClient2 = rd.randint(0, len(self.listClient) - 1)

                self.listClient[indClient1].pseudoOpponent = self.listClient[indClient2].pseudo
                self.listClient[indClient2].pseudoOpponent = self.listClient[indClient1].pseudo

                self.dicGames[self.nbGames] = [self.listClient[indClient1], self.listClient[indClient2]]
                self.listClient.pop(max(indClient1, indClient2))
                self.listClient.pop(min(indClient1, indClient2))

                for i in range(1, 3):
                    self.dicGames[self.nbGames][i - 1].idGame = self.nbGames
                    self.dicGames[self.nbGames][i - 1].numPlayer = i

                self.launchGame()
                self.lockListClient.release()

    def launchGame(self):
        for client in self.dicGames[self.nbGames]:
            packet = Packet(Action.LAUNCH_GAME, numPlayer=client.numPlayer, pseudoOpponent=client.pseudoOpponent)
            messageToSend = packet.encode()
            client.addMessageToSend(messageToSend)

    def updateMoves(self):
        pass

    def checkForWin(self):
        pass

    def updateClientGameReady(self):
        pass
