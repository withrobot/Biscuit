# ex23\_adv\_wifi\_CurrentTime.ino
* * * 
## 프로그램 내용
    Biscuit 에 Built-in 되어있는 Bosch bno055 를 이용하여
    온도, Accelometer, Gyroscope Maganetometer, EulerAngle 을 가져오는 예제.

## 사용 라이브러리
    없음

## 코드 설명
### int timezone = 9;
    자신이 사용할 timezone offset 을 설정한다. ex. Seoul, Korea (+9h)
### int dst = 0;
    daylight savings time offset 을 설정한다. 
### const char* ssid = "yourApSsid";
    자신의 무선네트워크의 ssid 문자열을 입력한다.
### const char* password = "yourApPassword";
    자신의 무선네트워크 ssid의 비밀번호를 입력한다.
### WiFi.begin(ssid, password);
    위에서 설정한 ssid, password를 이용하여 무선 공유기에 접속한다.
### while (WiFi.status() != WL_CONNECTED) {}
    WiFi 상태를 확인한다. while문을 이용하여 1초간격으로 상태를 확인하며 WL_CONNECTED(연결됨)이 반환될때까지 반복한다.
### configTime(timezone * 3600, dst, "pool.ntp.org", "time.nist.gov");
    위에서 설정한 timezone, dst 를 기반으로 ntp 서버에서 시간정보를 가져온다.