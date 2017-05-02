# ex20\_adv\_bno055\_basic.ino
* * * 
## 프로그램 내용
    Biscuit 에 Built-in 되어있는 Bosch bno055 를 이용하여
    온도, Accelometer, Gyroscope Maganetometer, EulerAngle 을 가져오는 예제.

## 사용 라이브러리
    Adafruit Unified Sensor
    Adafruit BNO055

## 코드 설명
BNO055의 RESET은 13번 핀에 연결되어 있다.
### bno055.begin();
    BNO055를 초기화 한다.
### bno055.getTemp();
    BNO055에 내장되어있는 센서의 온도를 가져온다.
### bno055.getVector();
    BNO055의 ACCELOMETER, GYROSCOPE, MAGNETOMETER, EULER 등의 값을 가져온다.

