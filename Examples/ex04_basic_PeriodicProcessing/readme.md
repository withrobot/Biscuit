# ex04\_basic\_PeriodicProcessing.ino
* * * 
## 프로그램 내용
    매 1초 주기로 문자열을 출력하는 예제.

## 사용 라이브러리
    없음

## 코드 설명
ex00_welcome_HelloBiscuit.ino 예제와 동작은 같지만,
ex00은 출력 후 1초를 기다리고 그것을 반복하는 것이고
ex04의 경우는 마지막으로 출력한지 1초가 지났는지를 반복하면서 확인하고 1초가 지난 경우에 출력하는 차이를 보인다.
### millis();
    ESP8266이 기동되고나서 부터 증가하고 있는 내부시계(system tick)을 millisecond단위로 얻어온다.

### if ((millis() - latestMillis) > periodMillis) { }
    마지막으로 수행한 시각(latestMillis)으로부터 설정한 주기시간(periodMillis)이 지난 경우 수행한다.
    delay()는 단순 시간을 지연 시키는 함수이기 때문에 기다리는 동안 프로그램 전체에 지연이 생기게 된다.
    위 방법은 특정시간이 지났는지를 확인하여 수행하기 때문에 주기적으로 실행하는 프로그램이더라도 기다리는 동안의 지연은 발생하지 않는다.
