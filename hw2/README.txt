Problem 1
- window 환경에서 intelliJ로 작성하였음

* build 방법
1. Zookeeper 실행
bin\windows\zookeeper-server-start.bat config\zookeeper.properties

2. Server 실행
bin\windows\kafka-server-start.bat config\server.properties

3. 소스 파일 중 Cacao.java Run

* 실행 예시 (콘솔 창에서 실행)
1. Login Window에서 
- Log in(1) 선택 후 ID 입력하여 로그인
- Exit(2) 선택하면 프로그램 종료

2. Chatting Window에서
- List(1)로 사용자의 Chatting room list 확인
- Make(2)로 Chatting room 생성
- Join(3)으로 List에 있는 chatting room에 접속
- Log out(4)하면 Login Window로 돌아감

3. Chat Room Window에서
- Read(1)로 해당 Chat room에서 마지막으로 읽은 메세지 이후의 새로운 메세지 확인
- Write(2)로 해당 Chat room에 접속한 사용자들에게 메세지 전송
- Reset(3)한 후 Read(1)하면 첫 메세지부터 다 확인할 수 있음
- Exit(4)하면 Chatting Window로 복귀

* ID와 chat list는 file로 관리된다. (cacaotalk\history.txt)