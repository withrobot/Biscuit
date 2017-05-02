# ex06\_basic\_Ticker.ino
* * * 
## 프로그램 내용
    Ticker를 이용하여 myFuctionOnce() 함수를 한번만 실행시키고,
    myFunctionPeriodic() 함수를 0.5초 주기로 실행하는 예제.

## 사용 라이브러리
    Ticker (Built-In)

## 코드 설명
    Ticker는 설정한 함수를 한번 또는 주기적으로 반복하는 라이브러리 이다.
### \#include <Ticker.h>
    Ticker 라이브러리를 사용하기 위해 Ticker.h를 포함한다.
### Ticker tickMyTickerOnce; Ticker tickMyTickerPeriodic;
    Ticker 인스턴스를 생성한다.
### tickMyTickerOnce.once(0.1, myFunctionOnce);
    myFunctionOnce 함수를 한번 실행한다.
### tickMyTickerPeriodic.attach(0.5, myFunctionPeriodic);
    myFunctionPeriodic 함수를 0.5초 주기로 반복 실행한다.
