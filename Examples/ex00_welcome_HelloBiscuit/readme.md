# ex00_welcome_HelloBiscuit.ino
* * * 
## 프로그램 내용
    1초 간격으로 Hello, Biscuit! 을 출력하는 예제.

## 사용 라이브러리
    없음

## 코드 설명
### setup() { }
    Biscuit 부팅시 한번만 실행되는 함수로 주로 초기화 관련 코드가 위치 한다.
### loop() { }
    전원이 꺼지기 전까지 매번 반복실행되는 함수로 주 코드가 위치하게 된다.
### Serial.begin(115200);
    Serial의 baudrate(통신속도)를 설정한다.
### delay(1000);
    1000ms. 즉, 1초 기다린다.
