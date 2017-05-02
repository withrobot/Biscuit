# ex05\_basic\_MultiplePeriodicProcessing.ino
* * * 
## 프로그램 내용
    여러 동작을 각각 설정한 주기시간 간격으로 반복하는 예제.

## 사용 라이브러리
    없음

## 코드 설명
    100ms 마다 "first job" 출력
    500ms 마다 "second job" 출력 및 LED Toggle
    1000ms 마다 "third job" 출력
    주기코드 수행에 지연이 걸리면 다른 주기코드에 영향을 주기때문에 최대한 지연이 안걸리게 작성하는 것이 바람직하다.