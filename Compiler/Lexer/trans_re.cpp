/*
    BY HTTO 2019
    Input: RE
    Output: A Lexer
*/
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
// epsilon=> '\0'
/*

*/

using trans_set=std::map<char,int>;
using namespace std;
vector<trans_set> NFA(1);
int state_index=0;
int cur_pos=0;
string input;
void skip_spacing()
{
    while(cur_pos<input.size()&&input[cur_pos]==' ')
        cur_pos++;
}
void build_nfa_node(char ch,int to=state_index+1)
{
    state_index=state_index++;
    while(NFA.size()<=state_index)
    {
        NFA.push_back(trans_set());
    }
    NFA[state_index][ch]=to; 
}
void match(char ch)
{
    skip_spacing();
    if(input[cur_pos++]==ch)
        return;
    throw std::runtime_error("RE representation error!");
}
void build_unit();
void build_re_closure();
void build_trans_table()
{
    
}
void build_unit()
{
    skip_spacing();
    switch (input[cur_pos])
    {
    case '(':
        cur_pos++;
        build_trans_table();
        match(')');
        break;
    default:
        match('\'');
        build_nfa_node(input[cur_pos]);
        match('\'');
        break;
    }
}

void build_re_closure()
{
    build_unit();
    skip_spacing();
    if(input[cur_pos]=='*')
    {
        
    }
}