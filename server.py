
from socket import * # sockets

serverName = '' # ip do servidor (em branco)
serverPort = 20072 # porta a se conectar
serverSocket = socket(AF_INET,SOCK_STREAM) # criacao do socket TCP
serverSocket.bind((serverName,serverPort)) # bind do ip do servidor com a porta
serverSocket.listen(1) # socket pronto para "ouvir" conexoes

print "Servidor TCP esperando conexoes na porta %d ..." % (serverPort)
connectionSocket, addr = serverSocket.accept() # aceita as conexoes dos clientes

while 1:
    sentence = connectionSocket.recv(1024) # recebe dados do cliente
    if sentence == "":
        break
    if "READ" in sentence:
        print sentence
        connectionSocket.send("2.3\n")
    else:
        print sentence
        connectionSocket.send("ACK\n")


connectionSocket.close() # encerra o socket com o cliente
serverSocket.close() # encerra o socket do servidor
