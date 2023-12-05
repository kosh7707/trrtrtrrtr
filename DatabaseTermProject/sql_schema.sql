drop table if exists accounts cascade;
create table accounts (
                          account_id      serial primary key,
                          user_id         varchar(20) unique,
                          user_pw         varchar(20),
                          role            varchar(20),
                          balance         int default 10000 check (balance >= 0),
                          mana            int default 0 check (mana >= 0),
                          created_date    timestamp default current_timestamp,
                          last_login      timestamp default current_timestamp
);

drop table if exists buffs cascade;
create table buffs (
                       id              serial primary key,
                       name            varchar(255) not null,
                       description     varchar(255) not null
);

drop table if exists items cascade;
create table items (
                       id              serial primary key,
                       name            varchar(255) not null,
                       score           int not null,
                       mana            int not null,
                       buff_id         int not null,
                       gadget          varchar(255) not null,
                       foreign key (buff_id) references buffs(id) on delete cascade
);

drop table if exists inventory cascade;
create table inventory (
                           id              serial primary key,
                           account_id      int not null,
                           item_id         int not null,
                           quantity        int not null,
                           foreign key (account_id) references accounts(account_id) on delete cascade,
                           foreign key (item_id) references items(id) on delete cascade
);

drop table if exists auctions cascade;
create table auctions (
                          id                  serial primary key,
                          item_id             int not null,
                          seller_id           int not null,
                          start_time          timestamp default current_timestamp not null,
                          end_time            timestamp not null,
                          starting_price      int not null,
                          current_price       int,
                          current_bidder_id   int,
                          foreign key (item_id) references items(id) on delete cascade,
                          foreign key (seller_id) references accounts(account_id) on delete cascade,
                          foreign key (current_bidder_id) references accounts(account_id) on delete cascade,
                          check (start_time < end_time)
);

drop role if exists merchant;
create role merchant;
revoke all privileges on all tables in schema public from merchant;
grant select (id, name, score) on table items to merchant;
grant select on table auctions to merchant;

drop role if exists mage;
create role mage;
revoke all privileges on all tables in schema public from mage;
grant select (id, name, mana, buff_id) on table items to mage;
grant select on table auctions to mage;

drop role if exists hacker;
create role hacker;
revoke all privileges on all tables in schema public from hacker;
grant select (id, name, gadget) on table items to hacker;
grant select on table auctions to hacker;