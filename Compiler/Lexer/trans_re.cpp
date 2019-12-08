/*
	BY HTTO 2019
	Input: RE
	Output: A Lexer
	Thompson algorithm
*/
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <memory>
#include <vector>

namespace htto_lexer
{
	using StateIndex = std::shared_ptr<int>;
	// first -> the state the node represents, to, the node points to
	using TwoArms = std::pair<StateIndex, StateIndex>;
	StateIndex make_si(int i) { return std::make_shared<int>(i); }

	struct trans_map
	{
		std::map<char, StateIndex> m;
		StateIndex& operator[](char n)
		{
			return m[n];
		}
		bool has_epsilon() { return !epsilon_edges.empty(); }
		std::vector<StateIndex> epsilon_edges;
		void display()
		{
			for (auto a : m)
			{
				std::cout << a.first << ":" << *(a.second) << ",";
			}
			for (auto b : epsilon_edges)
			{
				std::cout << "epsilon:" << *b << ",";
			}
		}
	};
	auto IndexCompare = [](const StateIndex& p1, const StateIndex& p2) {
		return *p1 < *p2;
	};
	std::map<StateIndex, trans_map, decltype(IndexCompare)> NFA(IndexCompare);
	int state_index = 0;
	int cur_pos = 0;
	std::string input;
	void skip_spacing()
	{
		while (cur_pos < input.size() && input[cur_pos] == ' ')
			cur_pos++;
	}
	StateIndex build_nfa_node(StateIndex from, char ch, StateIndex to)
	{
		NFA[from][ch] = to;
		return to;
	}
	void match(char ch)
	{
		skip_spacing();
		if (input[cur_pos++] == ch)
			return;
		throw std::runtime_error("RE representation error!");
	}
	StateIndex build_unit();
	TwoArms build_re_closure();
	StateIndex build_trans_table()
	{
		skip_spacing();
		auto left = build_re_closure();
		if (cur_pos == input.size())
		{
			return nullptr;
		}

		switch (input[cur_pos])
		{
		case '|':
		{
			match('|');
			auto right = build_re_closure();
			auto s = make_si(state_index++);
			auto ldest = make_si(state_index++);
			auto rdest = make_si(state_index++);
			*right.second = *rdest;
			*left.second = *ldest;
			NFA[s].epsilon_edges.push_back(right.first);
			NFA[s].epsilon_edges.push_back(left.first);
			auto e = make_si(state_index++);
			NFA[ldest].epsilon_edges.push_back(e);
			NFA[rdest].epsilon_edges.push_back(e);
			return e;
		}
		default:
		{
			auto right = build_re_closure();
			*left.second = *right.first;
			right.second = make_si(-3);
			return right.second;
		}
		}
	}
	StateIndex build_unit(StateIndex from)
	{
		skip_spacing();
		switch (input[cur_pos])
		{
		case '(':
		{
			cur_pos++;
			auto ret = build_trans_table();
			match(')');
			return ret;
		}
		default:
		{
			match('\'');
			StateIndex to = make_si(-1);
			auto ret = build_nfa_node(from, input[cur_pos++], to);
			match('\'');
			return to;
		}
		}
	}

	TwoArms build_re_closure()
	{
		StateIndex content_index = make_si(state_index++);
		auto content_to = build_unit(content_index);
		if (input[cur_pos] != '*')
			return { content_index, content_to };
		match('*');
		StateIndex pre_node = make_si(state_index++);
		StateIndex s = make_si(state_index++);
		StateIndex e = make_si(state_index++);
		*content_to = *e;
		NFA[pre_node].epsilon_edges.push_back(s);
		NFA[s].epsilon_edges.push_back(content_index);
		NFA[s].epsilon_edges.push_back(e);
		NFA[e].epsilon_edges.push_back(s);
		StateIndex ret = make_si(-1);
		NFA[e].epsilon_edges.push_back(ret);
		return { s, ret };
	}

} // namespace htto_lexer

int main()
{
	try
	{
		std::getline(std::cin ,htto_lexer::input);
		htto_lexer::build_trans_table();
		for (auto& t : htto_lexer::NFA)
		{

			std::cout << *(t.first) << "->";
			t.second.display();
			std::cout << std::endl;
		}

	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << '\n';
	}
	catch (...)
	{
		std::cout << "DAMN IT";
	}
	return 0;
}