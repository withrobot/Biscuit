# ex02\_basic\_ToggleLED.ino
* * * 
## 프로그램 내용
LED를 켜고 끄는 것을 반복하는 예제.

## 사용 라이브러리
없음

## 코드 설명
### ledStatus = digitalRead(PIN_LED);
PIN_LED의 값을 읽는다.
PIN_LED는 OUTPUT 으로 설정되있으므로 현재 OUTPUT 하고 있는 값을 읽게된다.
LOW일 경우 LED가 켜져있는 상태이고,
HIGH일 경우 LED가 꺼져있는 상태이다.
