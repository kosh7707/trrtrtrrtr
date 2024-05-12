<meta charset="UTF-8">

# 게임 인벤토리, 경매장 구현

이 프로젝트는 서버의 성능과 안정성, 유지보수에 중점을 두고 설계된 C++ 게임 서버입니다. 
`Winsock2`를 사용한 소켓 프로그래밍을 구현하였고, DB는 `postgreSQL`을,
해당 DB의 공식 C++ 라이브러리인 `libpqxx`를 사용하였습니다.

주요 기능으로는 다음과 같습니다.
- `회원가입 및 로그인`
- `실시간 채팅`
- `인벤토리 관리`
- `경매장`

서버 구조는 던전앤파이터, 메이플스토리, 월드 오브 워크래프트를 참고하였습니다.

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

디렉토리 최상단의 `README.pdf`를 확인해주세요.

## 유저 기능 및 명령어

유저는 자신의 인벤토리에 있는 아이템의 확인, 경매장에서의 물품 등록/구매/판매가 가능합니다.<br>
기본적으로 유저는 채팅을 치거나, 명령어를 입력해서 서버와 소통합니다.<br>
채팅은 단순히 터미널에 입력하는 것으로 가능합니다.

- `!help` : 사용가능한 명령어를 출력합니다.
- `!inventoryCheck` : 현재 내 정보와 인벤토리에 있는 아이템을 확인합니다.
- `!getTestItem [item_id] [quantity]` : `item_id`에 해당하는 아이템을 `item_quantity` 만큼 얻습니다.
- `!breakItem [item_id] [quantity]` : `item_id`에 해당하는 인벤토리에 있는 아이템을 파괴하고 점수를 얻습니다.
- `!openAuction` : 경매장에 있는 물품들을 검색합니다.- 
- `!sellItem [item_id] [quantity] [buyNowPrice] [startingBidPrice]` : 현재 내 인벤토리에 있는 아이템을 판매합니다.
- `!buyNow [auction_id]` : `auction_id`에 해당하는 경매 품목을 즉시 구매합니다.
- `!bid [auction_id] [price]` : `auction_id`에 해당하는 경매 품목을 입찰합니다.
- `!moveChannel [channel_name]` : `channel_name`에 해당하는 채널로 이동합니다.
- `!moveMap [map_name]` : `map_name`에 해당하는 맵으로 이동합니다.
- `!changeChatMode 0/1/2` : 채팅 모드를 변경합니다 (0: 전체, 1: 채널, 2: 맵 )
- `!exit` : 클라이언트를 종료합니다.