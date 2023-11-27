drop table if exists Accounts cascade;
create table Accounts (
    account_id SERIAL primary key,
    user_id VARCHAR(20),
    user_pw VARCHAR(20),
    created_date DATE,
    last_login DATE,
    gm_level INTEGER
);

drop table if exists Items cascade;
create table Items (
	item_id SERIAL primary key,
	item_name VARCHAR(50),
    item_description VARCHAR(300),
    item_rarity INTEGER
);

drop table if exists Equipment_items cascade;
create table Equipment_items (
    item_id INTEGER primary key references Items(item_id) on delete cascade,
    required_level INTEGER,
    item_level INTEGER
);

drop table if exists Consumable_items cascade;
create table Consumable_items (
    item_id INTEGER primary key references Items(item_id) on delete cascade,
    required_level INTEGER
);

drop table if exists Misc_items cascade;
create table Misc_items (
    item_id INTEGER primary key references Items(item_id) on delete cascade
);

drop table if exists Inventory cascade;
create table Inventory (
    account_id INTEGER references Accounts(account_id),
    item_id INTEGER references Items(item_id),
    quantity INTEGER,
    primary key (account_id, item_id)
);

create user kosh123 with password 'qwe123';