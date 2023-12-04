create user kosh7707 with encrypted password 'root';

alter user kosh7707 with superuser;

alter user kosh7707 with createdb;

alter user kosh7707 with createrole;

alter user kosh7707 with replication;

alter user kosh7707 with bypassrls;

create tablespace ts_databasetermproject owner kosh7707 location 'C:\Program Files\PostgreSQL\15\data\';

create database databasetermproject owner kosh7707 tablespace ts_databasetermproject;

drop table accounts;
create table accounts (
    account_id   serial primary key,
    user_id      varchar(20) unique,
    user_pw      varchar(20),
    created_date date default current_timestamp,
    last_login   date default current_timestamp
);
