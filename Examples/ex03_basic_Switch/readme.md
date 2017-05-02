# ex03\_basic\_Switch.ino
* * * 
## 프로그램 내용
    SWITCH가 눌렸으면(Pressed) LED를 켜고,
    눌리지 않았으면(Released) LED를 끄는 예제.

## 사용 라이브러리
    없음

## 코드 설명
### pinMode(PIN\_SWITCH, INPUT\_PULLUP);
    Biscuit의 SWITCH는 2번 핀에 연결 되어있다.
    PIN_SWITCH를 입력모드로 설정한다.
    별도의 회로를 구성하지 않고 사용하기 위해 내부풀업저항(internal pull-up resistor)을 사용하도록 한다.
    따라서, INPUT이 아닌 INPUT_PULLUP으로 설정한다.

### switchStatus = digitalRead(PIN_SWITCH);
    PIN_SWITCH의 현재 값을 읽어온다.
    LOW일 경우 SWITCH가 눌려있는(Pressed) 상태이다.
    HIGH일 경우 눌리지 않은(Released) 상태이다.
