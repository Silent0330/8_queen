#include "State.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

StateCompare::StateCompare(int functiontype)
{
	if (functiontype >= 0 && functiontype <= 2)
		type = functiontype;
	else
		type = 0;
}

bool StateCompare::operator()(const State &lhs, const State &rhs) const
{
	switch (type)
	{
	case 0:
		return lhs.depth > rhs.depth;
		break;
	case 1:
		return lhs.heuristic > rhs.heuristic;
		break;
	case 2:
		return lhs.cost > rhs.cost;
	}
}

State::State(int board[board_size])
{
	for (int i = 0; i < board_size; i++)
	{
		this->board[i] = board[i];
	}
	this->depth = 0;
	this->HeuristicFunction();
	this->cost = depth + heuristic;
	this->parent = -1;
}

bool State::operator==(const State &r) const
{
	for (int i = 0; i < board_size; i++)
	{
		if (board[i] != r.board[i])
			return false;
	}
	return true;
}

void State::Expand(vector<State> &discovered, vector<State> &explored, bool useInExp)
{
	for (int i = 0; i < board_size; i++)
	{
		if (board[i] == -1)
		{
			bool empty_rows[board_size] = { 1,1,1,1,1,1,1,1 };
			for (int j = 0; j < board_size; j++) // check rows can placed by queens in board
			{
				int r = board[j];
				int atkr;
				if (r != -1)
				{
					atkr = r;
					if (atkr >= 0 && atkr < board_size)
						empty_rows[atkr] = false;
					atkr = r + j - i;
					if (atkr >= 0 && atkr < board_size)
						empty_rows[atkr] = false;	// attacked if atkr + i == j + r
					atkr = r - j + i;
					if (atkr >= 0 && atkr < board_size)
						empty_rows[atkr] = false;	// attacked if atkr - i == r - j
				}
			}
			for (int j = 0; j < board_size; j++)
			{
				if (empty_rows[j])
				{
					State s = *this;
					s.board[i] = j;
					s.depth++;
					s.parent = explored.size() - 1;
					s.HeuristicFunction();
					s.cost = s.depth + s.heuristic;
					if (useInExp)
					{
						if (!s.InExplored(explored))
							discovered.push_back(s);
					}
					else
					{
						discovered.push_back(s);

					}
				}
			}
			break;
		}
	}
}

void State::Expand(priority_queue<State, vector<State>, StateCompare> &discovered, vector<State> &explored, bool useInExp)
{
	for (int i = 0; i < board_size; i++)
	{
		if (board[i] == -1)
		{
			bool empty_rows[board_size] = {1, 1, 1, 1, 1, 1, 1, 1};
			for (int j = 0; j < board_size; j++) // check rows can placed by queens in board
			{
				int r = board[j];
				int atkr;
				if (r != -1)
				{
					atkr = r;
					if (atkr >= 0 && atkr < board_size)
						empty_rows[atkr] = false;
					atkr = r + j - i;
					if (atkr >= 0 && atkr < board_size)
						empty_rows[atkr] = false;	// attacked if atkr + i == j + r
					atkr = r - j + i;
					if (atkr >= 0 && atkr < board_size)
						empty_rows[atkr] = false;	// attacked if atkr - i == r - j
				}
			}
			for (int j = 0; j < board_size; j++)
			{
				if (empty_rows[j])
				{
					State s = *this;
					s.board[i] = j;
					s.depth++;
					s.parent = explored.size() - 1;
					s.HeuristicFunction();
					s.cost = s.depth + s.heuristic;
					if (useInExp)
					{
						if (!s.InExplored(explored))
							discovered.push(s);
					}
					else
					{
						discovered.push(s);

					}
				}
			}
			break;
		}
	}
}

void State::Print()
{
	for (int i = 0; i < board_size; i++)
	{
		cout << setw(3) << board[i];
	}
	cout << '\n';
}

void State::PrintGoal()
{
	int cols[board_size];
	for (int i = 0; i < board_size; i++)
	{
		cols[board[i]] = i;
	}
	for (int i = 0; i < board_size; i++)
	{
		int r = cols[i];
		for (int j = 0; j < board_size; j++)
		{
			if (j != r)
			{
				cout << setw(2) << 0;
			}
			else
			{
				cout << setw(2) << 1;
			}
		}
		cout << '\n';
	}
	cout << '\n';
}

void State::PrintPath(vector<State> &explored)
{
	int i = parent;
	vector<State> path = vector<State>();
	path.push_back(*this);
	while (i != -1)
	{
		path.push_back(explored[i]);
		i = explored[i].parent;
	}
	while (path.size() > 0)
	{
		path.back().Print();
		path.pop_back();
	}
	PrintGoal();
}

bool State::IsGoal()
{
	return heuristic == 0;
}

bool State::InExplored(vector<State> &explored)
{
	for (int i = 0; i < explored.size(); i++)
	{
		if (*this == explored[i])
		{
			return true;
		}
	}
	return false;
}

void State::HeuristicFunction()
{
	heuristic = board_size - depth - 1;
}