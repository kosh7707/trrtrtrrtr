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
                           account_id      int not null,
                           item_id         int not null,
                           quantity        int not null,
                           primary key (account_id, item_id),
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
                          current_price       int not null,
                          current_bidder_id   int,
                          foreign key (item_id) references items(id) on delete cascade,
                          foreign key (seller_id) references accounts(account_id) on delete cascade,
                          foreign key (current_bidder_id) references accounts(account_id) on delete cascade,
                          check (start_time <= end_time),
                          check (current_price >= starting_price)
);

create view AuctionsView as
    select a.id                 as auction_id,
           i.name               as item_name,
           i.score              as item_score,
           i.mana               as item_mana,
           i.buff_id            as item_buff,
           i.gadget             as item_gadget,
           ac1.user_id          as seller,
           a.start_time         as start_time,
           a.end_time           as end_time,
           a.current_price      as current_price,
           ac2.user_id          as current_bidder
    from auctions a
    left join items i on a.item_id = i.id
    left join accounts ac1 on a.seller_id = ac1.account_id
    left join accounts ac2 on a.current_bidder_id = ac2.account_id;

drop role if exists merchant;
create role merchant;
revoke all privileges on all tables in schema public from merchant;
grant select (auction_id, item_name, item_score, seller, start_time, end_time, current_price, current_bidder) on AuctionsView to merchant;

drop role if exists mage;
create role mage;
revoke all privileges on all tables in schema public from mage;
grant select (auction_id, item_name, item_mana, item_buff, seller, start_time, end_time, current_price, current_bidder) on AuctionsView to mage;

drop role if exists hacker;
create role hacker;
revoke all privileges on all tables in schema public from hacker;
grant select (auction_id, item_name, item_gadget, seller, start_time, end_time, current_price, current_bidder) on AuctionsView to hacker;

-- TODO Auctions Table schema 다시 짜야함, 그에 따른 AuctionsView도 다시 짜야하고, 각종 명령어들 처리도 해줘야 함.
-- TODO PermisionStore의 개념을 도입해야함.