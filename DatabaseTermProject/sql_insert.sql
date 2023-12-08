-- buff
insert into buffs(name, description) values ('Test ����', 'Test ���� ����');

-- item
insert into items(name, score, mana, buff_id, gadget) values ('Test ������', 1, 1, 1, 'a');

select ac.role ����, ac.balance ������, ac.mana ���� from accounts ac where user_id = 'kosh77';

select items.name �������̸�, items.score ����, items.mana ����, items.gadget ����, buffs.name �����̸�, inventory.quantity ����
from accounts ac, items, inventory, buffs
where ac.user_id = 'kosh77'
  and ac.account_id = inventory.account_id
  and items.id = inventory.item_id
  and items.buff_id = buffs.id
order by items.id;
