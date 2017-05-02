# ex32\_expert\_wifimouse.ino
* * * 
## 프로그램 내용
    Biscuit을 UDP를 이용하여 mouseserver에 접속하여 마우스처럼 사용하는 예제.

## 사용 라이브러리
    Adafruit BNO055
    WiFiUdp

## 서버 프로그램 
    linux :
        modified from https://wifimouse.necta.us/
        http://wifimouse.necta.us/MSSourceForLinux.rar
    windows :
        based on http://www.binarytides.com/udp-socket-programming-in-winsock/
        process function from linux server.

## 코드 설명
### 사용 플랫폼 서버 선택
    ODROID만 사용할 경우 아래 정의를 주석처리하여 WINDOWS 코드를 비활성화 한다.
    // #define __USE_WINDOWS_SERVER__

    WINDOWS만 사용할 경우 아래 정의를 주석처리하여 ODROID 코드를 비활성화 한다.
    // #define __USE_ODROID_SERVER__

    모두 사용할 경우 둘다 주석처리 하지 않는다.
### 플랫폼 서버 IP 할당
    ODROID를 사용할 경우
    #define ODROID_SERVER_IP 에 ODROID의 IP를 입력한다.
    WINDOWS를 사용할 경우
    #define WINDOWS_SERVER_IP 에 WINDOWS의 IP를 입력한다.

### 플랫폼 모니터 해상도 설정
    ODROID를 사용하는 경우 사용중인 ODROID 해상도를 아래처럼 정의 한다.
    #define ODROID_DISPLAY_WIDTH  (1280)
    #define ODROID_DISPLAY_HEIGHT (720)

    WINDOWS를 사용하는 경우 사용중인 WINDOWS 해상도를 아래처럼 정의 한다.
    #define WINDOWS_DISPLAY_WIDTH (1920)
    #define WINDOWS_DISPLAY_HEIGHT (1080)

### WIFI SSID 및 비밀번호
    WIFI_SSID, WIFI_PASS를 입력하고,
    WINDOWS 또는 ODROID 서버 IP를 입력해준다.

### WINDOWS 서버 수정 및 빌드
    mouseserver_windows 폴더의 솔루션 파일을 Visual Studio로 열어, main.cpp 에서 사용중인 해상도를 지정 해 준다. 
    int screen_width = 1920;
    int screen_height = 1080;

    그리고 빌드 한다.

### LINUX 서버 수정 및 빌드
    ./src/session.cpp 파일을 열어 사용중인 해상도를 지정 해 준다.
    int screen_width = 1280;
    int screen_height = 720;

    mouseserver_linux 디렉토리의 builddep.sh, build-install.sh 2개 파일에 실행권한을 주고 ./build-install.sh 명령으로 빌드, 설치 한다.

    $ chmod +x builddep.sh
    $ chmod +x build-install.sh
    $ ./build-install.sh

    중간에 설치를 위한 관리자 비밀번호를 물어보는데 입력해 주면 된다.
