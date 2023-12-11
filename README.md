<meta charset="UTF-8">

# 게임 인벤토리 및 경매장 구현

`winsock2`로 서버-클라이언트간 소켓 통신하며, `postgreSQL`을 DBMS로 사용해서 게임 인벤토리 및 경매장을 구현해보았습니다.

해당 프로젝트에선 `libpq` 대신 `libpqxx`를 사용했습니다.<br>
(libpqxx Docs: [
  https://libpqxx.readthedocs.io/en/7.8.1/index.html
](https://libpqxx.readthedocs.io/en/7.8.1/index.html))<br>
`libpqxx`는 C++에서 postgreSQL과 통신하기 위해 고안된 공식 오픈소스 라이브러리입니다.<br>
성능면에서 `libpq`에 비해 더 빠르며, C++ 스타일의 코딩에 최적화 되어 있고, C의 고질적인 한글 인코딩 문제도 해결되었습니다. 

`#include<pqxx/pqxx>`를 포함하는 것으로 사용할 수 있습니다.<br>

해당 프로젝트를 작성할 당시 모든 인코딩을 `UTF-8`로 하였습니다.<br>
글자가 깨져 보일 경우 `postgreSQL`의 서버, 클라이언트 모두 인코딩을 `UTF-8`로 변경해주시고, `시스템 로케일`또한 `UTF-8`로 변경하시는 것이 권장됩니다.

## 사용된 도구 및 버전

- `OS: Windows 10 64bit`
- `IDE: CLion 2023.3`
- `File Encoding: UTF-8`
- `CMake 3.26.4`
- `MinGW 11.0 (threads: win32)`
- `C++ 17`
- `postgreSQL 15.5`
- `libpqxx 7.9.0`

## 빌드 방법

postgreSQL이 설치되어 있어야 합니다. 설정된 경로는 C:\Program Files\PostgreSQL\15 이며, 설치 경로가 다를 경우 환경 변수를 설정하거나, CMakeLists.txt를 직접 변경해주셔야 합니다. <br>
현재 minGW로 빌드시 thread 관련 연산자 오버로딩에서 컴파일 에러가 발생합니다. 에러가 생기는 코드에 대해서만 주석 처리를 해주시면 정상적으로 빌드됩니다.<br>
(issue : [
  https://github.com/jtv/libpqxx/issues/424
](https://github.com/jtv/libpqxx/issues/424)) <br>
(solution: [
  https://hectorhon.blogspot.com/2018/05/building-libpqxx-on-msys2-mingw-64-bit.html
](https://hectorhon.blogspot.com/2018/05/building-libpqxx-on-msys2-mingw-64-bit.html))<br>


## 유저 기능 및 명령어

유저는 자신의 인벤토리에 있는 아이템의 확인, 경매장에서의 물품 등록/구매/판매가 가능합니다.<br>
시작할때 정한 직업에 따라 경매 물품 확인 시 확인 가능한 아이템 정보가 달라집니다.<br>
common: `auction_id`, `item_name`, `quantity`, `seller`, `start_time`, `end_time`, `current_price`, `current_bidder`, `buy_now_price`<br>
merchant: `score` | mage: `mana`, `buff` | hacker: `gadget`<br>

기본적으로 유저는 채팅을 치거나, 명령어를 입력해서 서버와 소통합니다.<br>
채팅은 단순히 터미널에 입력하는 것으로 가능합니다. 명령어는 `!command` 를 입력하시면 됩니다.

- `!query` : 데이터베이스에 직접 쿼리를 실행합니다. 최초 유저가 갖는 권한은 `auctionView`에 대한 `select` 입니다. 쿼리는 세미콜론(;)이 등장할 때까지 계속 입력받을 수 있습니다.
- `!getTestItem` : 테스트 아이템을 하나 얻습니다.
- `!inventoryCheck` : 현재 내 정보와 인벤토리에 있는 아이템을 확인합니다.
- `!sellItem [item_id] [quantity] [buyNowPrice] [startingBidPrice]` : 현재 내 인벤토리에 있는 아이템을 판매합니다.
- `!buyNow [auction_id]` : `auction_id`에 해당하는 경매 품목을 즉시 구매합니다.
- `!bid [auction_id] [price]` : `auction_id`에 해당하는 경매 품목을 입찰합니다.
- `!breakItem [item_id] [quantity]` : `item_id`에 해당하는 인벤토리에 있는 아이템을 부수고 마나를 획득합니다.
- `!openPermissionStore` : `권한 상점(PermissionStore)`에 있는 권한들을 불러옵니다.
- `!buyPermission [permission_store_id]` : 권한 상점에 있는 권한을 구매합니다. 마나를 소모하여 구매할 수 있습니다. 해커의 경우, 특정 가젯들을 소지해야 구매 가능합니다.

유저는 `!query`를 사용해 직접 `auctionsView`를 확인하며 `경매`를 진행합니다. `경매장(Auctions)` 사용하는 재화는 `accounts`의 `balance`에 기록됩니다. <br>
유저는 `권한 상점(PermissionStore)`에서 원하는 권한을 구매할 수 있습니다. `권한 상점`에서 사용하는 재화는 `accounts`의 `mana`에 기록됩니다.

현재는 테스트 단계로 다음 세 가지를 테스트할 수 있습니다.
1. 인벤토리 확인
2. 경매 진행
3. 권한 상점에서 권한 구매 및 부여

미구현된 항목은 다음과 같습니다.
1. 아이템 버프 부분 : 인벤토리에 아이템 소지 시 추가 효과가 발생하도록 구현하려 했으나 미구현입니다.
2. 게임 관련 기능 전반 : 게임에 관련된 모든 부분이 아직 미구현입니다. 기존에 구현하려 했던 시나리오는 다음과 같습니다.
   1. 불특정 다수의 인원이 게임하는 것을 전제로 합니다.
   2. 일정 시간마다 아이템이 경매장에 랜덤하게 등록됩니다.
   3. 유저는 역할에 따라 승리 조건이 달라집니다. (상세 조건 미구현) 
3. 클라이언트의 GUI 구현: Qt5를 사용해서 구현하려 했으나, 아직 미구현입니다.