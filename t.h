#include <string.h>
#include <iostream>
#include <string>
enum type{img,num,sym};
enum op{add,sub,divide,mul,pow,sqr};
class Complex;
class Number;
class Symbol;
typedef Complex Img;
typedef Symbol Sym;
typedef Number Num;
class Base
{	
	//static mpfr_rnd_t rnd;
	//static mpfr_prec_t prec; 
	public:
	/*static void set_rounding(mpfr_rnd_t rnd)
	{	Number::rnd=rnd;
	}
	static mpfr_rnd_t get_rounding()
	{	return rnd;
	}
	static void set_precision(mpfr_prec_t prec )
	{	Number::prec=prec;
	}*/
	virtual std::string str()=0;
	virtual enum type type()=0;	
};

class Number:public Base
{	private:
	mpfr_t v;
	public:
	Number(int v)
	{	
		init();
		mpfr_set_ui(this->v,v,MPFR_RNDN);
	}
	Number(mpfr_t v)
	{	init();	
		mpfr_set(this->v,v,MPFR_RNDN);		
	}
	Number(const Number &n)
	{	init();
		memcpy(&v,&n.v,16);
	}
	~Number()
	{	mpfr_clear(this->v);
	}
	enum type type()
	{	return num;
	}
	std::string str()
	{ char* c=new char[100];
		mpfr_sprintf(c,"%Rf",this->v);
		return std::string(c);
	}
	private:
	void init()
	{ mpfr_init(this->v);	
	}
};

class Complex:public Base
{	private:
	mpfr_t re,im;
	public:
	Complex(unsigned long int re,unsigned long int im=0)
	{	init();
		mpfr_set_ui(this->re,re,MPFR_RNDN);
		mpfr_set_ui(this->im,im,MPFR_RNDN);
	}
	Complex(mpfr_t re,mpfr_t im)
	{ init();	
		mpfr_set(this->re,re,MPFR_RNDN);
		mpfr_set(this->im,im,MPFR_RNDN);
	}
	Complex(Complex *c)
	{	Complex(c->re,c->im);	
	}
	~Complex()
	{	mpfr_clear(re);
		mpfr_clear(im);
	}
	enum type type()
	{	return img;
	}
	std::string str()
	{ char *c=new char[100];
		if(mpfr_cmp_ui(this->re,0)==0)
		{ if(mpfr_cmp_ui(this->im,1)==0)
				strcpy(c,"i");
			else
				mpfr_sprintf(c,"%Rfi",this->im); 
		}
		else
			mpfr_sprintf(c,"%Rf+%Rfi",this->re,this->im); 
		return std::string(c);
	}
	private:
	void init()
	{ mpfr_init(this->re);
		mpfr_init(this->im);
	}
};

class Symbol: public Base
{	std::string s;
	public:
	Symbol(std::string s)
	{	this->s=s;}
	Symbol(char* c)
	{	this->s=std::string(c);
	}
	Symbol(Symbol &s)
	{	this->s=s.s;
	}
	std::string str()
	{	return s; 
	}
	enum type type()
	{	return sym;}
};

class Ex
{	private:
	bool leaf;
	enum op o;
	Base *b;
	Ex* l;
	Ex* r;		
	public:
	Ex(int re,int im)	
	{	leaf=true;
		b=new Complex(re,im);
	}
	Ex(int v)
	{	leaf=true;
		b=new Number(v); 
	}
	Ex(enum op o,Ex e1,Ex e2)
	{	this->leaf=false;
		this->o=o;
		this->l=new Ex(e1);
		this->r=new Ex(e2);	
	}
	Ex(enum op o,Ex e1)
	{	this->leaf=false;
		this->o=o;
//		this->l=new Ex(e1);
		this->r=NULL;
	}
	Ex(enum op o,Complex c, Number n)
	{	this->leaf=false;
		this->o=o;
		this->l=new Ex(c);
		this->r=new Ex(n);
	}
	Ex(Number n)
	{	this->leaf=true;	
		this->b=new Number(n);
	}
	Ex(Complex &c)
	{	this->leaf=true;
		this->b=new Complex(c);
	}
	Ex(Symbol &c)
	{	this->leaf=true;
		this->b=new Symbol(c);
	}
	std::string str()
	{	if(this->leaf)
		{	return this->b->str();	
		}
		else
		{	if(this->o==sqr)
				return "";//return "sqrt("+this->b->str()+")";	
			switch(this->o)
			{	case add:return this->l->str()+"+"+this->r->str();break;
				case sub:return this->l->str()+"-"+this->r->str();break;
				case mul:return this->l->str()+"*"+this->r->str();break;
				case divide:return this->l->str()+"/"+this->r->str();break;
				case pow:return this->l->str()+"^"+this->r->str();break;
			}
		}
	}
	std::string strn()
	{	return str()+"\n";
	}
};

