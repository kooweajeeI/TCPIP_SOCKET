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
        if (strncmp(request, "GET", 3) == 0) {
            // 이미지 파일 오픈
            FILE *imageFile = fopen("game.jpg", "rb");
            if (imageFile == NULL) {
                perror("Failed to open image file");
                exit(1);
            }

            // 이미지 파일 크기 계산
            fseek(imageFile, 0, SEEK_END);
            long imageSize = ftell(imageFile);
            rewind(imageFile);

            // HTTP 응답 생성 (헤더)
            snprintf(response, sizeof(response),
                     "HTTP/1.1 200 OK\r\n"
                     "Server: Linux Web Server\r\n"
                     "Content-Type: image/jpeg\r\n"
                     "Content-Length: %ld\r\n\r\n", imageSize);

            // 클라이언트에게 응답 전송 (헤더)
            send(clientSocket, response, strlen(response), 0);

            // 클라이언트에게 이미지 데이터 전송
            int bytesRead;
            while ((bytesRead = fread(response, 1, sizeof(response), imageFile)) > 0) {
                send(clientSocket, response, bytesRead, 0);
            }

            // 이미지 파일 닫기
            fclose(imageFile);
        }

        // 클라이언트 소켓 닫기
        close(clientSocket);
    }

    // 서버 소켓 닫기
    close(serverSocket);

    return 0;
}
