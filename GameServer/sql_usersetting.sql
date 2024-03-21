-- 테스트용 아이디 삭제 쿼리
drop owned by kosh1234;
drop user kosh1234;
delete from accounts;


-- root 유저 생성 쿼리
create user kosh7707 with encrypted password 'root';
alter user kosh7707 with superuser;
alter user kosh7707 with createdb;
alter user kosh7707 with createrole;
alter user kosh7707 with replication;
alter user kosh7707 with bypassrls;
create tablespace ts_databasetermproject owner kosh7707 location 'C:\Program Files\PostgreSQL\15\data\';
create database databasetermproject owner kosh7707 tablespace ts_databasetermproject;

select items.item_id item_id, items.name name, items.score score, buffs.name buff, inventory.quantity quantity
from accounts ac
    join inventory on ac.account_id = inventory.account_id
    join items on items.item_id = inventory.item_id
    join buffs on items.buff_id = buffs.buff_id
where ac.account_id = 1
order by items.item_id;