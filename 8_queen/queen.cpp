#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "State.h"
#include <ctime>

using namespace std;

const int board_size = 8;
State RandomState();
void IDS(State start);
bool DLS(State start, int limit);
bool R_DLS(State node, int limit, vector<State> &explored, int &states_num, int &max_states_num);
void UCS(State start);
void GFS(State start);
void AStar(State start);
void RBFS(State start);
int R_RBFS(State node, int limit, vector<State> &explored, int &states_num, int &max_states_num);


int main(int argc, char **argv)
{
	int k;

	int sta[board_size] = {0};
	for (int i = 1; i < board_size; i++) sta[i] = -1;
 
	State start = State(sta);
	while(true)
	{
		cout << "Please input k\n"
			<< "(k = -1) Input by hand\n"
			<< "(k = 0) Random\n";
		cin >> k;
		if (k == -1)
		{
			int r, c;
			while (true)
			{
				cout << "Please row and column of queen\n";
				cin >> r >> c;
				if (r >= 0 && r < board_size && c >= 0 && c < board_size)
				{
					for (int i = 0; i < board_size; i++) sta[i] = -1;
					sta[c] = r;
					start = State(sta);
					break;
				}
			}
			break;
		}
		else if (k == 0)
		{
			start = RandomState();
			break;
		}
	}

	while (true)
	{
		start.Print();
		cout << "Please input k\n"
			 << "(0) IDS\n"
			 << "(1) UCS\n"
			 << "(2) Greedy BFS\n"
			 << "(3) A*\n"
			 << "(4) RBFS\n"
			 << "(otherwise) Exit\n";
		cin >> k;
		if (k == 0)
			IDS(start);
		else if (k == 1)
			UCS(start);
		else if (k == 2)
			GFS(start);
		else if (k == 3)
			AStar(start);
		else if (k == 4)
			RBFS(start);
		else if (k == 5)
		{
			while(true)
			{
				cout << "Please input k\n"
					<< "(k = -1) Input by hand\n"
					<< "(k = 0) Random\n";
				cin >> k;
				if (k == -1)
				{
					int r, c;
					while (true)
					{
						cout << "Please row and column of queen\n";
						cin >> r >> c;
						if (r >= 0 && r < board_size && c >= 0 && c < board_size)
						{
							for (int i = 0; i < board_size; i++) sta[i] = -1;
							sta[c] = r;
							start = State(sta);
							break;
						}
					}
					break;
				}
				else if (k == 0)
				{
					start = RandomState();
					break;
				}
			}
			start.Print();
		}
		else
			break;
	}
}

State RandomState()
{
	try
	{
		int board[board_size] = { 0 };
		for (int i = 0; i < board_size; i++) board[i] = -1;
		srand(time(0));
		int r = rand() % board_size;
		int c = rand() % board_size;
		board[c] = r;
		State start = State(board);
		return start;
	}
	catch (const std::exception&)
	{
		cout << "error";
	}
}

void IDS(State start)
{
	bool finded = false;
	int limit = 0;
	while (!finded)
	{
		cout << limit << '\n';
		finded = DLS(start, limit);
		limit++;
	}
}

bool DLS(State start, int limit)
{
	vector<State> explored = vector<State>();

	int states_num = 0;
	int max_states_num = 0;

	return R_DLS(start, limit, explored, states_num, max_states_num);
}

bool R_DLS(State node, int limit, vector<State> &explored, int &states_num, int &max_states_num)
{
	explored.push_back(node);
	states_num++;
	if (states_num > max_states_num)
	{
		max_states_num = states_num;
	}
	if (node.IsGoal())
	{
		node.PrintPath(explored);
		cout << "number of movement : " << node.depth << '\n';
		cout << "maximum number of states : " << max_states_num << '\n';
		return true;
	}
	if (node.depth >= limit)
	{
		explored.pop_back();
		states_num--;
		return false;
	}

	vector<State> successors = vector<State>();
	node.Expand(successors, explored);
	states_num += successors.size();
	if (states_num > max_states_num)
	{
		max_states_num = states_num;
	}

	while (!successors.empty())
	{
		State cur = successors.back();
		successors.pop_back();
		states_num--;

		bool result = R_DLS(cur, limit, explored, states_num, max_states_num);

		if (result)
			return result;
	}
	explored.pop_back();
	states_num--;
	return false;
}

void UCS(State start)
{
	priority_queue<State, vector<State>, StateCompare> discovered(StateCompare(0));
	vector<State> explored = vector<State>();
	discovered.push(start);

	int cur_depth = 0;
	int max_states_num = 0;

	while (discovered.size() > 0)
	{
		if (discovered.size() + explored.size() > max_states_num)
		{
			max_states_num = discovered.size() + explored.size();
		}

		State cur = discovered.top();
		discovered.pop();
		explored.push_back(cur);

		if (cur_depth < cur.depth)
		{
			cur_depth = cur.depth;
			cout << cur_depth << "\n";
		}

		if (cur.IsGoal())
		{
			cur.PrintPath(explored);
			cout << "number of movement : " << cur.depth << '\n';
			cout << "maximum number of states : " << max_states_num << '\n';
			return;
		}
		cur.Expand(discovered, explored);
	}
}

void GFS(State start)
{
	priority_queue<State, vector<State>, StateCompare> discovered(StateCompare(1));
	vector<State> explored = vector<State>();
	discovered.push(start);

	int cur_depth = 0;
	int max_states_num = 0;

	while (discovered.size() > 0)
	{
		if (discovered.size() + explored.size() > max_states_num)
		{
			max_states_num = discovered.size() + explored.size();
		}

		State cur = discovered.top();
		discovered.pop();
		explored.push_back(cur);

		if (cur_depth < cur.depth)
		{
			cur_depth = cur.depth;
			cout << cur_depth << "\n";
		}

		if (cur.IsGoal())
		{
			cur.PrintPath(explored);
			cout << "number of movement : " << cur.depth << '\n';
			cout << "maximum number of states : " << max_states_num << '\n';
			return;
		}
		cur.Expand(discovered, explored);
	}
}

void AStar(State start)
{
	priority_queue<State, vector<State>, StateCompare> discovered(StateCompare(2));
	vector<State> explored = vector<State>();
	discovered.push(start);

	int cur_depth = 0;
	int max_states_num = 0;

	while (discovered.size() > 0)
	{
		if (discovered.size() + explored.size() > max_states_num)
		{
			max_states_num = discovered.size() + explored.size();
		}

		State cur = discovered.top();
		discovered.pop();
		explored.push_back(cur);

		if (cur_depth < cur.depth)
		{
			cur_depth = cur.depth;
			cout << cur_depth << "\n";
		}

		if (cur.IsGoal())
		{
			cur.PrintPath(explored);
			cout << "number of movement : " << cur.depth << '\n';
			cout << "maximum number of states : " << max_states_num << '\n';
			return;
		}
		cur.Expand(discovered, explored);
	}
}

void RBFS(State start)
{
	int states_num = 0;
	int max_states_num = 0;
	vector<State> explored = vector<State>();
	R_RBFS(start, INT_MAX, explored, states_num, max_states_num);
}

int R_RBFS(State node, int limit, vector<State> &explored, int &states_num, int &max_states_num)
{
	explored.push_back(node);
	states_num++;
	if (states_num > max_states_num)
	{
		max_states_num = states_num;
	}
	if (node.IsGoal())
	{
		node.PrintPath(explored);
		cout << "number of movement : " << node.depth << '\n';
		cout << "maximum number of states : " << max_states_num << '\n';
		return 0;
	}

	priority_queue<State, vector<State>, StateCompare> successors(StateCompare(2));

	node.Expand(successors, explored);
	states_num += successors.size();
	if (states_num > max_states_num)
	{
		max_states_num = states_num;
	}
	while (!successors.empty())
	{
		//find best cost successor
		State best = successors.top();
		successors.pop();
		states_num--;
		if (best.cost > limit)
		{
			states_num -= successors.size();
			return best.cost;
		}

		//set new limit
		int alternative;
		if (!successors.empty())
		{
			alternative = min(limit, successors.top().cost);
		}
		else
		{
			alternative = limit;
		}
		int result = R_RBFS(best, alternative, explored, states_num, max_states_num);
		if (result != 0)
		{
			best.cost = result;
			successors.push(best);
			explored.pop_back();
		}
		else
		{
			return result;
		}
	}
}
