#include "pch.h"

int client_socket;
bool connected = true;

// 서버로부터 메시지 수신, 별도 스레드에서 실행
void receive_messages(){
    char buffer[1024];
    while(connected){
        int bytes = recv(client_socket, buffer, sizeof(buffer)-1, 0);
        if (bytes<=0) {
            std::cout << "Server disconnected" << std::endl;
            connected = false;
            break;
        connected = false;
        break;
    }
    buffer[bytes] = '\0';
    std::cout << "echo: " << buffer; // 서버에서 받은 에코메시지
    }
}

int main(int argc, char* argv[]){
    if (argc != 3){
        std::cout << "echo-client:" << std::endl;
        std::cout << "syntax : echo-client <ip> <port>"<< std::endl;
        std::cout << "sample : echo-client 192.168.10.2 1234"<< std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    int server_port = atoi(argv[2]);

    // 소켓 생성
    client_socket = socket(AF_INET,SOCK_STREAM,0);
    if(client_socket == -1){
        std::cout << "Socket creation failed" << std::endl;
        return 1;
    }

    // 서버 주소 설정
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    // 서버 연결
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr))==-1){
        std::cout << "connection failed" << std::endl;
        return 1;
    }
    std::cout << "connected to server " << server_ip << ":" << server_port << std::endl;
    std::cout << "message : " << std::endl;

    //메시지 수신용 스레드 시작
    std::thread receive_thread(receive_messages);
    receive_thread.detach();

    //메인 스레드에서 사용자 입력을 처리
    std::string message;
    while(connected){
        std::cout<<"> ";
        std::getline(std::cin,message);
        if(message=="q")break;
        if(!message.empty()){
            message += "\n";
            send(client_socket,message.c_str(),message.length(),0);
        }
    }
    // 연결 종료
    connected = false;
    close(client_socket);
    return 0;
}