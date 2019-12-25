#include "timeWheel.h"



timeWheel::timeWheel(int _slotNum, int _step):slotNum(_slotNum),step(_step),slots(slotNum,NULL),cur_slot(0)
{

}


timeWheel::~timeWheel()
{
	for (int i = 0; i < slotNum; ++i)
	{
		tw_timer* tmp = slots[i];
		while (tmp)
		{
			slots[i] = tmp->next;
			delete tmp;
			tmp = slots[i];
		}
	}
}

tw_timer* timeWheel::add_timer(int timeout,Func _func)
{
	if (timeout < 0)
		return NULL;
	int ticks = 0;
	if (timeout < step)
	{
		ticks = 1;
	}
	else
	{
		ticks = timeout / step;
	}
	int rotation = ticks / slotNum;
	int ts = (cur_slot + (ticks%slotNum)) % slotNum;
	tw_timer* timer = new tw_timer(rotation, ts, _func);
	if (!slots[ts])
	{
		slots[ts] = timer;
	}
	else
	{
		timer->next = slots[ts];
		slots[ts]->prev = timer;
		slots[ts] = timer;
	}
	return timer;
}

void timeWheel::del_timer(tw_timer * timer)
{
	if (!timer)
		return;
	int ts = timer->time_slot;
	if (timer == slots[ts])
	{
		slots[ts] == slots[ts]->next;
		if (slots[ts])
		{
			slots[ts]->prev = NULL;
		}
		delete timer;
	}
	else
	{
		timer->prev->next = timer->next;
		if (timer->next)
		{
			timer->next->prev = timer->prev;
		}
		delete timer;
	}
}

void timeWheel::tick()
{
	cur_slot = ((cur_slot + 1) % slotNum);
	tw_timer* tmp = slots[cur_slot];
	while (tmp)
	{
		if (tmp->ratation > 0)
		{
			--tmp->ratation;
			tmp = tmp->next;
		}
		else
		{
			activeTimer.push_back(tmp);
			if (tmp == slots[cur_slot])
			{
				slots[cur_slot] = tmp->next;
				tmp->next = NULL;
				if (slots[cur_slot])
				{
					slots[cur_slot]->prev = NULL;
				}
				tmp = slots[cur_slot];
			}
			else
			{
				tmp->prev->next = tmp->next;
				if (tmp->next)
				{
					tmp->next->prev = tmp->prev;
				}
				tmp = tmp->next;
			}
			
		}
	}
	
}

vector<tw_timer*> timeWheel::getAvtiveTimer()
{
	vector<tw_timer*> res(std::move(activeTimer));
	activeTimer.clear();
	return res;
}

