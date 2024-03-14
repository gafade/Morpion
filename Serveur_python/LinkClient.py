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
        self.numPlayer = None#need to be assigned even
        self.lastmove =-1#added by GA : receiing the lastclick, int between 0 and 9
        self.fin = False
        self.listMessageReceived = []
        self.lockReception = th.Lock()
        self.lockSending = th.Lock()
        self.listMessageSend = []
        self.threadReceiving = th.Thread(target=self.receivingFromClient)#LES PARENTHESES SIGNI>IE EXECUTER 1 FOIS
        self.threadAnalysing = th.Thread(target=self.analyseDataReceived)
        self.threadSending = th.Thread(target=self.sendDataToClient)
        #IL NE FAUT PAS OUBLIER LES START
        self.threadReceiving.start()
        self.threadAnalysing.start()
        self.threadSending.start()

    def receivingFromClient(self):
        print("Client connecté. Adresse " + str(self.adress[0]) + " port : " + str(self.adress[1]),flush=True)
        # self.socket.send(("Serveur a client : j'utilise le PID "
        # + str(os.getpid())+" et le thread"+str(th.get_ident())).encode("utf-8"))

        while not self.fin:
            try :
                donnees_recues = self.socket.recv(4096)
                donneesDecodees = donnees_recues.decode("utf-8")#utf-8 crée des smileys
                self.lockReception.acquire() 
                self.listMessageReceived.append(donneesDecodees)
                self.lockReception.release()
                print("Received from client : " + donneesDecodees)
            except:
                 self.fin=True
        print("Fin de connexion avec client :" + str(self.adress[0]) + "port : " + str(self.adress[1]))
        self.socket.close()

    def analyseDataReceived(self):

        while not self.fin: 
            if len(self.listMessageReceived) > 0:
                self.lockReception.acquire()
                lastMessage = self.listMessageReceived[len(self.listMessageReceived) - 1]
                if lastMessage.find(":")!=-1:
                    self.analyseMessage(lastMessage)
                else:
                    print("analyseDataReveiced failed : non-formatted string",flush=True)
                self.listMessageReceived.pop(len(self.listMessageReceived) - 1)
                self.lockReception.release()
                 
    def analyseMessage(self, message):

        packet = Packet()
        packet.decode(message)

        if packet.action== Action.PSEUDO:
                if packet.pseudo:
                    self.pseudo = packet.pseudo
        elif packet.action== Action.CLOSE_CONNECTION:
            self.socket.close()
            self.fin = True
        elif packet.action==Action.MOVE_PLAYED:
            self.lastmove=packet.move
        else:
            print("Error",flush=True)

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