-- buff
insert into buffs(name, description) values ('Test 버프', 'Test 버프 설명');

-- item
insert into items(name, score, mana, buff_id, gadget) values ('Test 아이템', 1, 1, 1, 'a');

-- inventory test
insert into inventory (account_id, item_id, quantity)
values ( (select account_id from accounts where user_id = 'kosh7707'), '1', '1')
on conflict (account_id, item_id) do update set quantity = inventory.quantity + EXCLUDED.quantity;