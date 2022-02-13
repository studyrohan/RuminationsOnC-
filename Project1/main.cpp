#include <iostream>
#include <fstream> 
using namespace std;
class Expr;
class Expr_node
{
	friend class Expr;
	friend std::ostream& operator << (std::ostream&, const Expr_node&);
	//friend std::ostream& operator << (std::ostream&, const Expr&);

	int use;
public:

	//why protected print not work?
	virtual void print(std::ostream&) const = 0;
protected:
	Expr_node() : use(0) {}
	virtual ~Expr_node() =default;

};


std::ostream& operator<<(std::ostream& o, const Expr_node& e)
{
	e.print(o);
	return o;
}


class Int_node :public Expr_node {
	friend class Expr;
	int n;
	Int_node(int k) :n(k) {}
	void print(std::ostream& o)const {
		o << n;
	}
};

class Unary_node :public Expr_node {
	friend class Expr;
	std::string op;
	Expr_node* opnd;
	Unary_node(const std::string& a, Expr_node* b)
		:op(a), opnd(b)
	{

	}
	void print(std::ostream& o)const
	{
		o << "(" << *opnd << op << ")";
	}
};
class Binary_node :public Expr_node {
	friend class Expr;
	std::string op;
	Expr_node* left;
	Expr_node* right;
	Binary_node(const string& a, Expr_node* b, Expr_node* c)
		:op(a), left(b), right(c)
	{

	}
	void print(ostream& o)const
	{
		o << "(" << *left << op << *right << ")";
	}
};

class Expr {
	/*friend std::ostream& operator<<(std::ostream& o, const Expr& t);*/
	friend std::ostream& operator << (std::ostream&, const Expr&);
	Expr_node* p;
public:
	Expr(int);
	Expr(const std::string&, Expr);
	Expr(const std::string&, Expr, Expr);
	Expr(const Expr&);
	Expr& operator=(const Expr&);
	~Expr() {
		delete p;
	}
};
std::ostream& operator<<(std::ostream& o, const Expr& e)
{
	e.p->print(o);
	return o;
}
Expr::Expr(int n)
{
	p = new Int_node(n);
}
Expr::Expr(const std::string& op, Expr t)
{
	p = new Unary_node(op, t.p);
}
Expr::Expr(const std::string& op, Expr left, Expr right)
{
	p = new Binary_node(op, left.p, right.p);
}
Expr::Expr(const Expr& t)
{
	p = t.p;
	++p->use;
}
Expr& Expr::operator=(const Expr& t)
{
	t.p->use++;
	if (p->use-- == 0)
	{
		delete p;
	}
	p = t.p;
	return *this;
}
int main()
{
	Expr t = Expr("*", Expr("-", 5), Expr("+", 3, 4));
	cout << t << std::endl;
	t = Expr("*", t, t);
	cout << t << endl;
	return 0;
}