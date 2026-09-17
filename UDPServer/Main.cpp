#include <iostream>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>
#include <stdlib.h>
#include <thread>
#include <chrono>

void Server(); /// Prototyping


int main(int argc, char*argv[]){
	Server();
	return 1;
}


Uint8 dataBuffer[512]; /// This is just a byte array


void Server(){
	
	NET_DatagramSocket *sd; /// Socket descriptor 
	NET_Datagram *p;       /// Pointer to packet memory */
	p = nullptr;
	bool isRunning;
 
	/// Initialize SDL_net
	if (NET_Init() == false) {
		std::cerr << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
 
	/// Open a socket and bind the port to the OS
	if (!(sd = NET_CreateDatagramSocket(NULL,0x1A2B, 0))) {
		std::cerr << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
 
	///Make space for the packet
	p = new NET_Datagram{};
	p->buf = dataBuffer;
	
 
	/// Main loop
	isRunning = true;
	std::cout << "Listening: " << std::endl;
	
	while (isRunning) {
		if (NET_ReceiveDatagram(sd, &p) == true){ /// This always returns true unless failure
			if (p == nullptr) {
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				continue;
			}

			strcpy_s((char*)dataBuffer,sizeof(dataBuffer), (char*)p->buf);

			std::cout << "UDP Packet incoming" << std::endl;
			std::cout << "Data: " <<  p->buf << std::endl;


			std::cout << "Len: " <<  std::dec << p->buflen << std::endl;
			std::cout << "Address: " <<  std::hex << p->addr << ": " << std::hex << p->port << std::endl << std::endl;
			
 
			if(strcmp((char*)p->buf, "quit") == 0){
				isRunning = false;
				std::cout << "Bye" << std::endl;
			}
		}
		if(p) {
			NET_DestroyDatagram(p);
		}
	}
 
	/// Clean up and exit the program 
	NET_Quit();
	return;
}