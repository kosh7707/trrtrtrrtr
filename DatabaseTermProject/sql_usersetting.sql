-- 테스트용 아이디 삭제 쿼리
drop owned by kosh77;


-- root 유저 생성 쿼리
create user kosh7707 with encrypted password 'root';
alter user kosh7707 with superuser;
alter user kosh7707 with createdb;
alter user kosh7707 with createrole;
alter user kosh7707 with replication;
alter user kosh7707 with bypassrls;
create tablespace ts_databasetermproject owner kosh7707 location 'C:\Program Files\PostgreSQL\15\data\';
create database databasetermproject owner kosh7707 tablespace ts_databasetermproject;
