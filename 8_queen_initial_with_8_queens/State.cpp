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

void State::Expand(vector<State> &discovered, vector<State> &explored, bool useInExp, bool onlyforward)
{
	int i = 0;
	if (onlyforward) i = last_moved_c;
	for (; i < board_size; i++)
	{
		if (!moved[i])
		{
			for (int j = 0; j < board_size; j++)
			{
				if (j != board[i])
				{
					State s = *this;
					s.board[i] = j;
					s.moved[i] = true;
					s.last_moved_c = i;
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
		}
	}
}

void State::Expand(priority_queue<State, vector<State>, StateCompare> &discovered, vector<State> &explored, bool useInExp, bool onlyforward)
{
	int i = 0;
	if (onlyforward) i = last_moved_c;
	for (; i < board_size; i++)
	{
		if (!moved[i])
		{
			for (int j = 0; j < board_size; j++)
			{
				if (j != board[i])
				{
					State s = *this;
					s.board[i] = j;
					s.moved[i] = true;
					s.last_moved_c = i;
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
	for (int i = 0; i < board_size; i++)
	{
		atked_cols[i] = false;
	}
	for (int i = 0; i < board_size; i++)
	{
		if (!atked_cols[i])
		{
			for (int j = i + 1; j < board_size; j++) // check rows can placed by queens in board
			{
				if (board[i] == board[j])
				{
					atked_cols[i] = true;
					atked_cols[j] = true;
					break;
				}
				if (i + board[i] == j + board[j]) // attacked if i + board[i] == j + board[j]
				{
					atked_cols[i] = true;
					atked_cols[j] = true;
					break;
				}
				if (board[i] - i == board[j] - j) // attacked if board[i] - i == board[j] - j
				{
					atked_cols[i] = true;
					atked_cols[j] = true;
					break;
				}
			}
		}
	}
	heuristic = 0;
	for (int i = 0; i < board_size; i++)
	{
		if (atked_cols[i])
			heuristic++;
	}
}