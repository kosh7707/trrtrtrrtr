-- buff
insert into buffs(name, description) values ('Test 버프', 'Test 버프 설명');

-- item
insert into items(name, score, mana, buff_id, gadget) values ('Test 아이템', 1, 1, 1, 'a');

select ac.role 직업, ac.balance 소지금, ac.mana 마나 from accounts ac where user_id = 'kosh77';

select items.name 아이템이름, items.score 점수, items.mana 마나, items.gadget 가젯, buffs.name 버프이름, inventory.quantity 수량
from accounts ac, items, inventory, buffs
where ac.user_id = 'kosh77'
  and ac.account_id = inventory.account_id
  and items.id = inventory.item_id
  and items.buff_id = buffs.id
order by items.id;
