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

## 초보자 도움말
    1. 이 예제를 사용하기 위해서는 Telegram 계정이 필요합니다.
	Telegram 가입은 안드로이드, 아이폰 등의 앱을 설치하거나 단순하게 웹 브라우저 만으로도 할 수 있습니다.
	"https://telegram.org/" 를 참고하세요.
	2. 이 예제는 Telegram의 Bot을 이용합니다. Bot은 Telegram에서 "BotFather"와 대화를 통해 "/newbot" 요청으로 만들 수 있습니다. 자세한 것은 https://core.telegram.org/bots#creating-a-new-bot 를 참고하세요.
	이렇게 만들어진 Bot의 "Token", "Name", "UserName" 등은 예제에 포함된 "app_config.h"에 기록해 주어야 합니다.
	3. Biscuit을 사용하는 무선랜 환경의 "SSID"와 "Password" 등을 예제에 포함된 "app_config.h"에 기록해 주어야 합니다.
	4. Biscuit을 이용하여 충격을 감지하려면 "/start", "/rememberme"의 순서로 Bot을 통해 Biscuit에 명령을 내리고, 충격감지의 감도을 높이려면 "/impact_low"으로 하고, 낮추려면 "/impact_high"로 하면 됩니다.
