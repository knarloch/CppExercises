#include <iostream>

struct S
{
	S() = default;
	S& operator=(const S&){ std::cout<<__func__<<" const S&\n"; }
	S(const S&){ std::cout<<__func__<<" const S&\n"; }
	S& operator=(S&&){ std::cout<<__func__<<" S(S&&)\n"; }
	S(S&&){ std::cout<<__func__<<" S(S&&)\n"; }
	void use() const{std::cout<<__func__<<"\n"; }
};
	S takesSAmpAmpReturnsMoveS(S&& s)
	{
		std::cout<<__func__<<"\n";
		return std::move(s);
	}
	S takesSAmpAmpReturnsS(S&& s)
	{
		std::cout<<__func__<<"\n";
		return s;
	}
	S takesSreturnsMoveS(S s)
	{
		std::cout<<__func__<<"\n";
		return std::move(s);
	}
	S takesSreturnsS(S s)
	{
		std::cout<<__func__<<"\n";
		return s;
	}
	S returnsS()
	{
		S s;
		std::cout<<__func__<<"\n";
		return s;
	}
	S returnsMoveS()
	{
		S s;
		std::cout<<__func__<<"\n";
		return std::move(s);
	}


int main() {
	std::cout<<"==================================\n";
	S s1 = takesSAmpAmpReturnsMoveS(S{});
	s1.use();
	std::cout<<"==================================\n";
	S s2 = takesSAmpAmpReturnsS(S{});
	s2.use();
	std::cout<<"==================================\n";
	S s3 = takesSreturnsMoveS(S{});
	s3.use();
	std::cout<<"==================================\n";
	S s4 = takesSreturnsS(S{});
	s4.use();
	std::cout<<"==================================\n";
	S s5 = returnsS();
	s5.use();
	std::cout<<"==================================\n";
	S s6 = returnsMoveS();
	s6.use();
	std::cout<<"==================================\n";
    return 0;
	
}
