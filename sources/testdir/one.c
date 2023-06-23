#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    int serverSocket, clientSocket;
    socklen_t clientAddressLength;
    struct sockaddr_in serverAddress, clientAddress;
    char webpage[MAX_BUFFER_SIZE];

    // 웹 페이지 HTML 내용
    snprintf(webpage, MAX_BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n"
             "Server: Linux Web Server\r\n"
             "Content-Type: text/html; charset=UTF-8\r\n\r\n"
             "<!DOCTYPE html>\r\n"
             "<html><head><title>My Web Page</title>\r\n"
             "<style>body {background-color: #FFFF00}</style></head>\r\n"
             "<body><center><h1>Hello World</h1><br>\r\n"
             "<img src=\"game.jpg\"></center></body></html>\r\n");

    // 소켓 생성
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("소켓 생성 실패");
        exit(1);
    }

    // 서버 주소 설정
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // 포트 번호를 8080으로 설정
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // 소켓과 서버 주소 연결
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("바인딩 실패");
        exit(1);
    }

    // 클라이언트 연결 대기
    listen(serverSocket, 5);
    printf("웹 서버가 실행 중입니다...\n");

    while (1) {
        clientAddressLength = sizeof(clientAddress);

        // 클라이언트로부터의 연결 요청 수락
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            perror("연결 수락 실패");
            exit(1);
        }

        // 웹 페이지 전송
        send(clientSocket, webpage, strlen(webpage), 0);

        // 이미지 파일 읽기
        FILE *imageFile = fopen("game.jpg", "rb");
        if (imageFile == NULL) {
            perror("이미지 파일 열기 실패");
            exit(1);
        }

        // 이미지 파일 전송
        char buffer[MAX_BUFFER_SIZE];
        size_t bytesRead;
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), imageFile)) > 0) {
            send(clientSocket, buffer, bytesRead, 0);
        }

        fclose(imageFile);

        // 클라이언트 소켓 종료
        close(clientSocket);
    }

    // 서버 소켓 종료
    close(serverSocket);

    return 0;
}
