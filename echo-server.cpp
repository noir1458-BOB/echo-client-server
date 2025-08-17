#include "pch.h"

// 클라이언트 소켓, -e, -b
std::vector<int> clients;
bool echo = false;
bool broadcast = false;

// 클라이언트 처리(각 클라이언트마다 별도 스레드에서)
void handle_client(int client_socket){
    char buffer[1024];

    std::cout << "Received: ";
    while(true){
        //클라이언트로부터 메시지 수신
        int bytes = recv(client_socket, buffer, sizeof(buffer)-1,0);
        if (bytes<=0) break;

        buffer[bytes] = '\0';
        std::cout << buffer;

        if(echo){
            if(broadcast){
                for(auto e:clients){
                    if(e!=client_socket){
                        send(e,buffer,bytes,0);
                    }
                }
            } else {
                send(client_socket,buffer,bytes,0);
            }
        }
    }
    close(client_socket);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (*it == client_socket) {
            clients.erase(it);
            break;
        }
    }
    std::cout << "Client disconnected" << std::endl;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "echo-server:" << std::endl;
        std::cout << "syntax : echo-server <port> [-e[-b]]" << std::endl;
        std::cout << "sample : echo-server 1234 -e -b" << std::endl;
        return (1);
    }

    int port = atoi(argv[1]);
    for(int i=2;i<argc;i++){
        if(std::string(argv[i])=="-e") echo = true;
        if(std::string(argv[i])=="-b") broadcast = true;
    }

    // 소켓 생성
    // 2 = IPV4, 1 = TCP, 0 = 프로토콜 자동선택, 일반적으로 TCP에서 0 
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 서버 주소 설정
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 모든 IP에서 접속 허용
    server_addr.sin_port = htons(port);

    // 소켓과 주소 바인딩
    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));

    // 클라이언트 연결 대기
    listen(server_socket, 5);
    std::cout << "listen port " << port << std::endl;
    std::cout << "Echo: " << (echo ? "ON" : "OFF") << ", Broadcast: " << (broadcast ? "ON" : "OFF") << std::endl;

    while(true){
        // 클라이언트 연결 수락
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);

        clients.push_back(client_socket);

        // 새 스레드에서 클라이언트 처리
        std::thread(handle_client, client_socket).detach();
    }
    close(server_socket);
    return 0;
}