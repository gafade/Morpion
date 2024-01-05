from enum import Enum, auto


class Action(Enum):
    PSEUDO = 0
    LAUNCH_GAME = auto()
    MOVE_PLAYED = auto()
    GAME_FINISHED = auto()
    REPLAY_GAME = auto()
    CLOSE_CONNECTION = auto()

"""
Format (simple) d'échange d'informations entre le client et le serveur:
Exemple de string envoyé :

(Action):(coup):(num de joueur):(pseudo):(pseudo_adversaire)

Ordre d'envoi des messages :
- Lors de la connexion, le client envoie son pseudo au serveur

- Le serveur envoie au client, quand une partie se lance, une Action 
LAUNCH_GAME + le pseudo de l'adversaire + son numéro de joueur
Joueur 1 -> commence avec les ronds
Joueur 2 -> joue en 2ème avec les croix

- Puis, envoi des coups : 
    . Si le client joue en 1er, il envoie un coup puis il attend que le 
    serveur lui envoie le coup de l'adversaire en réponse
    . S'il est 2ème, il attend un coup de l'adversaire
    
- Lorsque la partie est finie, le serveur envoie une action GAME_FINISHED
avec le numéro du gagnant à l'endroit (num du joueur).
Si la partie s'est soldée par une nulle, il envoie un num du joueur = 0.

- Le client peut alors soit envoyer une action REPLAY_GAME pour espérer
rejouer contre la même personne, soit renvoyer son pseudo pour jouer 
contre quelqu'un d'autre

Le coup correspond à un nombre entre 0 et 8:
Grille de morpion avec les num de coups associés:
0 1 2
3 4 5
6 7 8

"""


class Packet:

    def __init__(self, action = None, move = None, numPlayer = None, pseudo = None, pseudoOpponent = None):
        self.action = action
        self.move = move
        self.numPlayer = numPlayer
        self.pseudo = pseudo
        self.pseudoOpponent = pseudoOpponent

    def __repr__(self):
        return "{}:{}:{}:".format(self.action.value, self.move, self.numPlayer) + self.pseudo + ":" \
            + self.pseudoOpponent

    def encode(self):
        return "{}:{}:{}:".format(self.action.value, self.move, self.numPlayer) + self.pseudo + ":" \
            + self.pseudoOpponent

    def decode(self, messageReceived):
        messageSplit = messageReceived.split(":")
        self.action = list(Action)[int(messageSplit[0])]
        self.move = int(messageSplit[1])
        self.numPlayer = int(messageSplit[2])
        self.pseudo = messageSplit[3]
        self.pseudoOpponent = messageSplit[4]

