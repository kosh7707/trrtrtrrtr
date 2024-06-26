drop table if exists accounts cascade;
create table accounts (
    account_id      serial primary key,
    user_id         varchar(20) unique,
    user_pw         varchar(20),
    balance         int default 10000 check (balance >= 0),
    score           int default 0,
    created_date    timestamp default current_timestamp,
    last_login      timestamp default current_timestamp
);

drop table if exists items cascade;
create table items (
    item_id         serial primary key,
    name            varchar(255) not null,
    score           int not null
);

drop table if exists inventory cascade;
create table inventory (
    account_id      int not null,
    item_id         int not null,
    quantity        int not null check (quantity > 0),
    primary key (account_id, item_id),
    foreign key (account_id) references accounts(account_id) on delete cascade,
    foreign key (item_id) references items(item_id) on delete cascade
);

drop table if exists auctions cascade;
create table auctions (
    auction_id            serial primary key,
    item_id               int not null,
    item_quantity         int not null,
    seller_id             int not null,
    start_time            timestamp default current_timestamp not null,
    end_time              timestamp not null,
    current_price         int not null,
    current_bidder_id     int,
    buy_now_price         int not null,
    foreign key (item_id) references items(item_id) on delete cascade,
    foreign key (seller_id) references accounts(account_id) on delete cascade,
    foreign key (current_bidder_id) references accounts(account_id) on delete cascade,
    check (item_quantity >= 1),
    check (start_time <= end_time),
    check (current_price <= buy_now_price)
);

create view inventory_view as
    select iv.account_id as account_id, iv.item_id as item_id, i.score as score, iv.quantity as quantity
    from inventory iv, items i
    where iv.item_id = i.item_id;

-- item
insert into items(name, score) values ('Test item1', 1);
insert into items(name, score) values ('Test item1', 2);
insert into items(name, score) values ('Test item1', 3);

