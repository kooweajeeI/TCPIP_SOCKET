#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char request[1024], response[4096];

    // 웹 페이지 HTML 내용
    char webpage[] = "HTTP/1.1 200 OK\r\n"
                     "Server: Linux Web Server\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html><head><title>My Web Page</title>\r\n"
                     "<style>body {background-color: #FFFF00}</style></head>\r\n"
                     "<body><center><h1>Hello World</h1><br>\r\n"
                     "<img src=\"game.jpg\"></center></body></html>\r\n";

    // 서버 소켓 생성
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // 서버 주소 설정
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);  // 원하는 포트 번호로 변경 가능

    // 서버 소켓 바인딩
    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    // 서버 리스닝
    listen(serverSocket, 5);

    printf("Server is running on port 8080...\n");

    while (1) {
        socklen_t clientAddressLength = sizeof(clientAddress);

        // 클라이언트 요청 대기
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

        // 클라이언트 요청 수신
        recv(clientSocket, request, sizeof(request), 0);

        // GET 또는 POST 요청인지 확인
        if (strncmp(request, "GET", 3) == 0 || strncmp(request, "POST", 4) == 0) {
            // 클라이언트에게 웹 페이지 응답 전송
            send(clientSocket, webpage, strlen(webpage), 0);
        }

        // 클라이언트 소켓 닫기
        close(clientSocket);
    }

    // 서버 소켓 닫기
    close(serverSocket);

    return 0;
}
