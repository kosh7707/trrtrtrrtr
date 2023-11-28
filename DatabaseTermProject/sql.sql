create user kosh7707 superuser with encrypted password 'root';

create tablespace ts_databasetermproject owner kosh7707 location 'C:\Program Files\PostgreSQL\15\data\';

create database databasetermproject owner kosh7707 tablespace ts_databasetermproject;

