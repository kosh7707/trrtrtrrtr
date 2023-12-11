

select * from inventory i where i.account_id = (select account_id from accounts where user_id='kosh77') and item_id = 1 and quantity >= 3;

insert into inventory values (5, 1, 5);

update inventory set quantity = quantity - 2 where account_id = 5 and item_id = 1;

delete from inventory i where i.account_id = 5 and i.item_id = 1;
