# ex07\_basic\_FlexibleTicker.ino
* * * 
## 프로그램 내용
    버튼이 누르는 순간 Ticker를 이용하여 LED를 0.2초 간격으로 깜빡이기 시작하고,
    버튼이 떼는 순간 LED의 깜밖임을 멈추는 예제.

## 사용 라이브러리
    Ticker (Built-In)

## 코드 설명
### if (currSwitchStatus != prevSwitchStatus)
    누르거나 떼는 순간 한번만 수행되는 코드 이다. 
### ticker.attach(0.2, toggleLED);
    toggleLED 함수를 0.2초 주기로 반복 실행한다.
### ticker.detach();
    반복 실행되던 것을 중단 시킨다.
