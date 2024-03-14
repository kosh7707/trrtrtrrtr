<meta charset="UTF-8">

# 게임 인벤토리, 경매장, 상점 구현

`winsock2`로 서버-클라이언트간 소켓 통신하며, `postgreSQL`을 DBMS로 사용해서 게임 인벤토리, 경매장, 상점을 구현해보았습니다.

해당 프로젝트에선 `libpq` 대신 `libpqxx`를 사용했습니다.<br>
(libpqxx Docs: [
  https://libpqxx.readthedocs.io/en/7.8.1/index.html
](https://libpqxx.readthedocs.io/en/7.8.1/index.html))<br>
`libpqxx`는 C++에서 postgreSQL과 통신하기 위해 고안된 공식 오픈소스 라이브러리입니다. 성능면에서 `libpq`에 비해 더 빠르며, C++ 스타일의 코딩에 최적화 되어 있고, C의 고질적인 한글 인코딩 문제도 해결되었습니다.<br> 
`#include<pqxx/pqxx>`를 포함하는 것으로 사용할 수 있습니다.<br>

해당 프로젝트를 작성할 당시 모든 인코딩을 `UTF-8`로 하였습니다. 글자가 깨져 보일 경우 `postgreSQL`의 서버, 클라이언트 모두 인코딩을 `UTF-8`로 변경해주시고, `시스템 로케일`또한 `UTF-8`로 변경하시는 것이 권장됩니다.

## 사용된 도구 및 버전

- `OS: Windows 10 64bit`
- `IDE: CLion 2023.3`
- `File Encoding: UTF-8`
- `CMake 3.26.4`
- `MinGW 11.0 (threads: win32)`
- `C++17`
- `postgreSQL 15.5`
- `libpqxx 7.9.0`

## 빌드 방법

postgreSQL이 설치되어 있어야 합니다. 설정된 경로는 C:\Program Files\PostgreSQL\15 이며, 설치 경로가 다를 경우 환경 변수를 설정하거나, CMakeLists.txt를 직접 변경해주셔야 합니다.

현재 minGW로 빌드시 thread 관련 연산자 오버로딩에서 컴파일 에러가 발생합니다. 에러가 생기는 코드에 대해서만 주석 처리를 해주시면 정상적으로 빌드됩니다.<br>
(issue : [
  https://github.com/jtv/libpqxx/issues/424
](https://github.com/jtv/libpqxx/issues/424)) <br>
(solution: [
  https://hectorhon.blogspot.com/2018/05/building-libpqxx-on-msys2-mingw-64-bit.html
](https://hectorhon.blogspot.com/2018/05/building-libpqxx-on-msys2-mingw-64-bit.html))<br>

cmake cache를 정리 후 빌드하는 것을 추천합니다. 

## 유저 기능 및 명령어

유저는 자신의 인벤토리에 있는 아이템의 확인, 경매장에서의 물품 등록/구매/판매가 가능합니다.<br>
기본적으로 유저는 채팅을 치거나, 명령어를 입력해서 서버와 소통합니다.<br>
채팅은 단순히 터미널에 입력하는 것으로 가능합니다. 명령어는 `!command` 를 입력하시면 됩니다.

- `!getTestItem` : 테스트 아이템을 하나 얻습니다.
- `!inventoryCheck` : 현재 내 정보와 인벤토리에 있는 아이템을 확인합니다.
- `!sellItem [item_id] [quantity] [buyNowPrice] [startingBidPrice]` : 현재 내 인벤토리에 있는 아이템을 판매합니다.
- `!buyNow [auction_id]` : `auction_id`에 해당하는 경매 품목을 즉시 구매합니다.
- `!bid [auction_id] [price]` : `auction_id`에 해당하는 경매 품목을 입찰합니다.
- `!breakItem [item_id] [quantity]` : `item_id`에 해당하는 인벤토리에 있는 아이템을 부숩니다.

## 개선이 필요한 부분 / 버그
1. 현재 통신에 사용하는 버퍼 사이즈는 1024 bytes 입니다. 메시지의 크기와는 상관없이 고정이기에, 낭비되는 부분도 많고, 이 이상의 크기는 송수신이 불가합니다.
2. ServerCore는 현재 DAO와 연관 관계입니다. 덕분에 여러 DAO를 한번에 다뤄야 하는 문제가 생겨 코드에 가독성이 많이 떨어집니다. 서비스 로직을 구현해 개선이 필요합니다.
3. 이벤트 핸들링 코드가 if-else로 하드코딩되어 있습니다.
4. 이벤트를 확인하는데에 필요한 바이트가 현재 3바이트입니다. 이벤트 경우의 수를 생각했을 때 1바이트로 개선되어야 합니다.
5. I/O Thread가 서로 구분된 상태에서 std::endl을 사용해 입력 버퍼의 내용이 사라지는 경우가 종종 발생합니다.
6. 시스템 로케일을 UTF-8로 사용중이기에, 한글 채팅이 불가능합니다.