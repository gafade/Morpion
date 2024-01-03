import socket
import os
import threading as th


def instanceServeur(socket_cree_pour_client, adresse_client):
    print("Client connecté. Adresse " + str(adresse_client[0]) + " port : " + str(adresse_client[1]))
    socket_cree_pour_client.send(("Serveur a client : j'utilise le PID " + str(os.getpid())+" et le thread"+str(th.get_ident())).encode("utf-8"))

    fin = False
    while (not(fin)):
        donnees_recues = socket_cree_pour_client.recv(4096)
        donneesDecodees = donnees_recues.decode("utf-8")
        print(donneesDecodees)
        fin = (donneesDecodees.find("FIN") > 0)

    print("Fin de connexion avec client :" + str(adresse_client[0]) + "port : " + str(adresse_client[1]))
    socket_cree_pour_client.close()


adresse_socket = ("", 2100)
socket_serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_serveur.bind(adresse_socket)
socket_serveur.listen()


while True:
    socket_cree_pour_client, adresse_client = socket_serveur.accept()
    th.Thread(instanceServeur(socket_cree_pour_client, adresse_client))

socket_serveur.close()
