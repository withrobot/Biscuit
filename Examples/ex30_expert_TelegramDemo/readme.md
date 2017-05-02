# ex30\_expert\_TelegramDemo.ino
* * * 
## 프로그램 내용
    Telegram Bot을 이용한 Biscuit 간단한 동작 제어, 움직임 알람 기능을 양방향 통신을 사용하는 예제.

## 사용 라이브러리
    ESP8266TelegramBot ( https://github.com/Gianbacchio/ESP8266-TelegramBot )

## 코드 설명
    https://core.telegram.org/bots#creating-a-new-bot 를 참고하여 봇을 만들도록 한다.
    이 때 만들어진 token 은 bot을 사용하기 위한 열쇠이므로 본인만 알고 있기를 권장한다.

    wifi ssid, password 를 입력하고,
    app_config.h 에 생성한 bot name, username, token을 입력하여 준다.

    Bot_RecvProc() 함수에 구현되어있는 명령을 이용하여 보도록 한다.
    명령은 슬래시를 포함하도록 작성되어 있으며, 작성되어있는 명령을 이용하면 된다.
    Biscuit에 명령을 내리는 방법은 텔레그램 bot 채팅창에 /start 를 전송하면 된다.
    다른 명령도 전송하여 확인 해 보라.

    또, 필요에 따라 명령을 추가, 삭제를 해보자.
