import socket
import os


adresse_socket = ("", 8088)
socket_serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_serveur.bind(adresse_socket)
socket_serveur.listen()


while True:
    socket_cree_pour_client, adresse_client = socket_serveur.accept()
    print("Client connecté. Adresse " + str(adresse_client[0]) + " port : " + str(adresse_client[1]))
    donnees_recues = socket_cree_pour_client.recv(4096)
    print(donnees_recues.decode("utf-8"))

    socket_cree_pour_client.send(("Serveur a client : j'utilise le PID " + str(os.getpid())).encode("utf-8"))
    print("Connexion fermée")
    socket_cree_pour_client.close()

socket_serveur.close()
