#pragma once
#include <string>
#include <format>
#include<iostream>

using namespace std;


enum Action {
    PSEUDO = 0,
LAUNCH_GAME,
MOVE_PLAYED,
GAME_FINISHED,
REPLAY_GAME,
CLOSE_CONNECTION,
};


/*
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

*/


class PacketClient {
private:
    Action action;
    int move;
    int numPlayer;
    string pseudo;
    string pseudoOpponent;

public:

    PacketClient() {
        this->action = PSEUDO;
        this->move = -1;
        this->numPlayer = -1;
        this->pseudo ="None";
        this->pseudoOpponent = "None";


    }
    PacketClient(Action action,int move = 0,int numPlayer = 0, string pseudo= "None", string pseudoOpponent="None") {
        this->action = action;
        this->move = move;
        this->numPlayer = numPlayer;
        this->pseudo = pseudo;
        this->pseudoOpponent = pseudoOpponent;

    }

    


    string encode() {
        return to_string(this->action ) + " :"+ to_string(this->move) +" : "+ to_string(this->numPlayer)  +" : "  + this->pseudo + " : " + this->pseudoOpponent;
    }

    void decode(const char* messageReceived) {
        
        //avoir une copie de la str, +1 car charactere de fin "\0"
        char* mod=new char[strlen(messageReceived)+1];
        strcpy_s(mod, strlen(messageReceived)+1, messageReceived);
        //separer le msg
        char* messageSplit[10];
        char* reste = NULL;
        int i = 0;
        messageSplit[i] = strtok_s(mod, ":",&reste);
        while (messageSplit[i]!=NULL and i<10) {//10 est le nb max de nombre
            i++;
            // break the string when it encounters empty space
            messageSplit[i] = strtok_s(NULL, ":",&reste);

    }


        this->action = Action( atoi(messageSplit[0]) );
        this->move = atoi(messageSplit[1]);
        this->numPlayer = atoi(messageSplit[2]);
        this->pseudo = messageSplit[3];
        this->pseudoOpponent = messageSplit[4];
        
    }
    void decode(string messageReceived) {
        this->decode(messageReceived.c_str());

    }

    
};