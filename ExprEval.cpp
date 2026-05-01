#include<bits/stdc++.h>
using namespace std;
#define ll long long int
#define F false
#define T true
//const double PI = 3.14159265358979323846;
//const double E = 2.71828182845904523536;

int op_doing(char op)//优先级
{
	switch (op)
	{
		case '+':return 1;
		case '-':return 1;
		case '*':return 2;
		case '/':return 2;
		case '%':return 2;
		case '^':return 3;
		case '(':return 0;
		case ')':return 0;
		
		default:return -1;
	}
}
bool op_is_left(char op)//是左结合运算符吗？
{
	return op != '^';
}
bool is_other_op(const char& tep)
{
	if (tep == 'e')
		return T;
	if (tep == 'p')
		return T;
	else
		return F;
}
bool op_is_using(char op)
{
	return op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '^' || op == '(' || op == ')';
}
double count(double a, double b, char op)//运算
{
	switch (op)
	{
		case '+':return a + b;
		case '-':return a - b;
		case '/':
		{
			if (b == 0)
				cout << "ERROR : " << a << " / 0!";
			else
				return a / b;
		}
		case '*':return a * b;
		case '%':
		{
			if (b == 0)
				cout << "ERROR : " << a << " % 0!";
			else
				return fmod(a, b);
		}
		case '^':return pow(a, b);	
		
		default:return -1;
	}
}
bool is_minus(const string& expression, int len)//是减号还是负数？
{
	if (expression[len] != '-')
		return F;
	if (len == 0)
		return T;
	
	char B_len = expression[len - 1];

	if (B_len == '(')
		return T;
	if (B_len == '+' || B_len == '-' || B_len == '*' || B_len == '/' || B_len == '%' || B_len == '^')
		return T;
	return F;
}
string read_other_op(const string& expression, ll& i)//<- will using f()
{
	string re = "";

	if (expression[i] == 'e')//e
	{
		re = "2.71828182845904523536";
		i++;
	}

	else if (expression[i] == 'p' && expression.size() > i + 1 && expression[i + 1] == 'i')//pi
	{
		re = "3.14159265358979323846";
		i += 2;
	}

	//sin,cos,tan,lg,log...
	//will using f()
	i--;
	return re;
}
string read_string(const string& expression, ll& i)//表达式处理
{
	string num = "";
	if (expression[i] == '-' && is_minus(expression, i))//负数
	{
		num += '-';
		i++;
	}

	if (i < expression.size() && is_other_op(expression[i]))
	{
		//bool is_f = F;
		string other = read_other_op(expression, i);//<-
		//if()
		if (num == "-")
			return "-" + other;
		else
			return other;
	}
	while (i < expression.size() && isdigit(expression[i]))//整数
	{
		num += expression[i];
		i++;
	}
	if (i < expression.size() && expression[i] == '.')//小数
	{
		num += '.';
		i++;

		while(i < expression.size() && isdigit(expression[i]))
		{
			num += expression[i];
			i++;
		}
	}

	i--;
	return num;
}
vector<string> sol_string(const string& expression)
{
	vector<string> p;
	stack<char> ops;

	for (ll i = 0; i < expression.size(); i++)
	{
		char tep = expression[i];
		if (tep == ' ')
			continue;
		if (isdigit(tep) || (tep == '-' && is_minus(expression, i)) || is_other_op(tep))
		{
			string num = read_string(expression, i);
			p.push_back(num);
		}
		else if (tep == '(')
			ops.push(tep);
		else if (tep == ')')
		{
			while (!ops.empty() && ops.top() != '(')
			{
				p.push_back(string(1, ops.top()));
				ops.pop();
			}
			if (!ops.empty())
				ops.pop();
		}
		else if (op_is_using(tep))
		{
			if (tep == '-' && is_minus(expression, i))
				continue;
			
			while (!ops.empty() && ops.top() != '(')
			{
				int top_p = op_doing(ops.top());
				int next_p = op_doing(tep);
				if (tep == '^' && ops.top() == '^')
					break;
				if (top_p > next_p || (top_p == next_p && op_is_left(ops.top())))
				{
					p.push_back(string(1, ops.top()));
					ops.pop();
				}
				else
					break;

			}
			ops.push(tep);
		}
	}
	while (!ops.empty())
	{
		p.push_back(string(1, ops.top()));
		ops.pop();
	}
	return p;
}
double count_p(const vector<string>& p)
{
	stack<double> nums;
	
	for (const string &t : p)
	{
		bool is_number = F;
		if (t[0] == '-' && t.size() > 1)
		{
			is_number = T;
			for (ll i = 1; i < t.size(); i++)
				if (!isdigit(t[i]) && t[i] != '.')
				{
					is_number = F;
					break;
				}
		}
		else if (isdigit(t[0]))
			is_number = T;
		if (is_number)
			nums.push(stod(t));
		else if (t.size() == 1 && op_is_using(t[0]))
		{
			if (nums.size() < 2)
			{
				cout << "ERROR : CONT HAVE ENOUGHT NUM(S)" << endl;
				return -1;
			}
			
			double a = nums.top();
			nums.pop();
			double b = nums.top();
			nums.pop();

			double result = count(b, a, t[0]);
			nums.push(result);
		}
	}
	if (nums.size() != 1)
	{
		cout << "ERROR : U EXPRESSION IS WORNG." << endl;
		return -1;
	}
	return nums.top();
}
int main()
{
	string expression;
	vector<string> next_p;
	//file
	cout << "ENTER U EXPRESSION:" << endl;
	getline(cin, expression);
	next_p = sol_string(expression);
	double result = count_p(next_p);
	cout << expression << " = " << result;
	return 0;
}