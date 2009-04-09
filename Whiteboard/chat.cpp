#include <stdio.h>
#include <string.h>
#include "Raknet/RakNetworkFactory.h"
#include "Raknet/RakPeerInterface.h"
#include "Raknet/MessageIdentifiers.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

// Moved out of main - needs global scope
RakPeerInterface *peer;
bool isServer;

void PrintMessage(RPCParameters *rpcParameters)
{
	printf("%s\n", rpcParameters->input);

	if (isServer){
		peer->RPC("PrintMessage",(const char*)rpcParameters->input, rpcParameters->numberOfBitsOfData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID, 0);
	}
}

int main(void)
{
	char str[512];
	//RakPeerInterface *peer = RakNetworkFactory::GetRakPeerInterface();
	//bool isServer;
	Packet *packet;

	peer = RakNetworkFactory::GetRakPeerInterface();

	printf("(C) or (S)erver?\n");
	gets(str);
	if ((str[0]=='c')||(str[0]=='C'))
	{
		peer->Startup(1,30,&SocketDescriptor(), 1);
		isServer = false;
	} else {
		peer->Startup(MAX_CLIENTS, 30, &SocketDescriptor(SERVER_PORT,0), 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	} else {
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0]==0){
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		peer->Connect(str, SERVER_PORT, 0,0);
	}

	REGISTER_STATIC_RPC(peer, PrintMessage);

	while (1)
	{

		if (!isServer){
			printf("Enter a string to show on the server: ");
			gets(str);
			// Two tricky things here.  First, you have to remember to send the NULL terminator so you need strlen(str)+1
			// Second, if you didn't read the docs you might not realize RPC takes the number of bits rather than the number of bytes.
			// You have to multiply the number of bytes by 8
			// Don't forget that RPC takes bits as the data length, not bytes!
			if (str[0])
				peer->RPC("PrintMessage",str, (strlen(str)+1)*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}

		packet=peer->Receive();

		while(packet)
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
				//printf("Enter a string to show on the server: ");
				//gets(str);
				// Two tricky things here.  First, you have to remember to send the NULL terminator so you need strlen(str)+1
				// Second, if you didn't read the docs you might not realize RPC takes the number of bits rather than the number of bytes.
				// You have to multiply the number of bytes by 8
				// Don't forget that RPC takes bits as the data length, not bytes!
				//peer->RPC("PrintMessage",str, (strlen(str)+1)*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				break;

			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer){
					printf("A client has disconnected.\n");
				} else {
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer){
					printf("A client lost the connection.\n");
				} else {
					printf("Connection lost.\n");
				}
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}

			peer->DeallocatePacket(packet);

			// Stay in the loop as long as there are more packets.
			packet = peer->Receive();
		}
	}

	RakNetworkFactory::DestroyRakPeerInterface(peer);

	return 0;
}
