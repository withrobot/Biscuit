# ex01\_basic\_LED.ino
* * * 
## 프로그램 내용
    LED를 켜는 예제.

## 사용 라이브러리
    없음

## 코드 설명
### pinMode(PIN_LED, OUTPUT);
    Biscuit의 LED는 12번 핀에 연결 되어있다.
    PIN_LED를 출력모드로 설정한다.

### digitalWrite(PIN_LED, LOW);
    LED를 켠다.
    PIN_LED의 출력을 LOW로 낼 경우 LED가 켜지고,
    PIN_LED의 출력을 HIGH로 낼 경우 LED가 꺼진다.
