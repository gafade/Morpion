import threading as th
from Packet import Packet, Action


class LinkClient:

    def __init__(self, socket_cree_pour_client, adressClient):
        self.socket = socket_cree_pour_client
        self.adress = adressClient
        self.pseudo = ""
        self.pseudoOpponent = ""
        self.inGame = False
        self.idGame = None
        self.numPlayer = None
        self.fin = False
        self.listMessageReceived = []
        self.lockReception = th.Lock()
        self.lockSending = th.Lock()
        self.listMessageSend = []
        self.threadReceiving = th.Thread(self.receivingFromClient())
        self.threadAnalysing = th.Thread(self.analyseDataReceived())
        self.threadSending = th.Thread(self.sendDataToClient())

    def receivingFromClient(self):
        print("Client connecté. Adresse " + str(self.adress[0]) + " port : " + str(self.adress[1]))
        # self.socket.send(("Serveur a client : j'utilise le PID "
        # + str(os.getpid())+" et le thread"+str(th.get_ident())).encode("utf-8"))

        while not self.fin:
            donnees_recues = self.socket.recv(4096)
            donneesDecodees = donnees_recues.decode("utf-8")
            self.lockReception.acquire()
            self.listMessageReceived.append(donneesDecodees)
            self.lockReception.release()
            print(donneesDecodees)

        print("Fin de connexion avec client :" + str(self.adress[0]) + "port : " + str(self.adress[1]))
        self.socket.close()

    def analyseDataReceived(self):

        while not self.fin:
            if len(self.listMessageReceived) > 0:
                self.lockReception.acquire()
                lastMessage = self.listMessageReceived[len(self.listMessageReceived) - 1]
                self.analyseMessage(lastMessage)
                self.listMessageReceived.pop(len(self.listMessageReceived) - 1)
                self.lockReception.release()

    def analyseMessage(self, message):

        packet = Packet()
        packet.decode(message)

        match packet.action:
            case Action.PSEUDO:
                if packet.pseudo:
                    self.pseudo = packet.pseudo
            case Action.CLOSE_CONNECTION:
                self.socket.close()
                self.fin = True

            case _:
                print("Error")

    def sendDataToClient(self):

        while not self.fin:
            if len(self.listMessageSend) > 0:
                self.lockSending.acquire()
                self.socket.send(self.listMessageSend[len(self.listMessageSend) - 1].encode("utf-8"))
                self.listMessageSend.pop(len(self.listMessageSend) - 1)
                self.lockSending.release()

    def addMessageToSend(self, messageToSend):
        self.lockSending.acquire()
        self.listMessageSend.append(messageToSend)
        self.lockSending.release()