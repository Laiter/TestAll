#include "simple_ñalculator.h"
#include <string>

namespace simplecalc {
char SimpleCalculator::calc_last_sign_ = 0;
double SimpleCalculator::calc_last_arg_ = 0;
double SimpleCalculator::calc_result_ = 0;
bool SimpleCalculator::flag_ = 0;

SimpleCalculator::SimpleCalculator()
{
	calc_last_sign_ = '=';
	calc_last_arg_ = 0;
	calc_result_ = 0;
	flag_ = 0;
}

SimpleCalculator::SimpleCalculator(char sign)
{
	calc_last_sign_ = sign;
}

SimpleCalculator::~SimpleCalculator()
{
}

double SimpleCalculator::operator()(double operand_lh, double operand_rh) //simple functor that clears old results 
{																		  //and call switch with possible operations
	calc_result_ = 0;													  //that work with 2 functor parameters
	calc_last_arg_ = 0;
	flag_ = 0;
	switch (calc_last_sign_) {
	case '+':
		calc_last_arg_ = operand_rh;
		return calc_result_ = operand_lh + operand_rh;
	case '-':
		calc_last_arg_ = operand_rh;
		return calc_result_ = operand_lh - operand_rh;
	case '*':
		calc_last_arg_ = operand_rh;
		return calc_result_ = operand_lh * operand_rh;
	case '/':
		calc_last_arg_ = operand_rh;
		return calc_result_ = operand_lh / operand_rh;
	case '^':
	{
		calc_last_arg_ = operand_rh;
		double mult = operand_lh;
		while (operand_rh-- > 1) {
			mult *= operand_lh;
		}
		return calc_result_ = mult;
	}
	default:
		std::cout << "Operation's not allowed\n";
		return 0;
	}
} //operator()(double operand_lh, double operand_rh)

double SimpleCalculator::operator()(double operand_rh) 	//simple functor that doesn't clears old results 
{														//and call switch with possible operations
	flag_ = 0;											//that work with functor parameter and class result
	switch (calc_last_sign_) {							
	case '+':
		calc_last_arg_ = operand_rh;
		return calc_result_ += operand_rh;
	case '-':
		calc_last_arg_ = operand_rh;
		return calc_result_ -= operand_rh;
	case '*':
		calc_last_arg_ = operand_rh;
		return calc_result_ *= operand_rh;
	case '/':
		calc_last_arg_ = operand_rh;
		return calc_result_ /= operand_rh;
	case '^':
	{
		calc_last_arg_ = operand_rh;
		double mult = calc_result_;
		while (operand_rh-- > 1) {
			mult *= calc_result_;
		}
		return calc_result_ = mult;
	}
	case '=':
	{
		calc_last_arg_ = calc_result_;
		return calc_result_ = operand_rh;
	}
	default:
		std::cout << "Operation's not allowed\n";
		return 0;
	}
} //operator()(double operand_rh)

double SimpleCalculator::undo() //undo last operation by opposite operation with
{								//saved result, argument and sign
	switch (calc_last_sign_) {
	case '+':
		calc_result_ -= calc_last_arg_;
		calc_last_arg_ = -calc_last_arg_;
		return calc_result_;
	case '-':
		calc_result_ += calc_last_arg_;
		calc_last_arg_ = -calc_last_arg_;
		return calc_result_;
	case '*':
		calc_result_ /= calc_last_arg_;
		calc_last_arg_ = 1 / calc_last_arg_;
		return calc_result_;
	case '/':
		calc_result_ *= calc_last_arg_;
		calc_last_arg_ = 1 / calc_last_arg_;
		return calc_result_;
	case '^':
	{
		double mult = calc_result_;
		double power = calc_last_arg_;
		if (flag_ == 0)
		{
			calc_result_ = pow(mult, (1 / power));
		}
		else
		{
			calc_result_ = pow(mult, power);
		}
		flag_ ^= 1;
		return calc_result_;
	}
	case '=':
	{
		std::swap(calc_last_arg_, calc_result_);
		return calc_result_;
	}
	default:
		std::cout << "Operation's not allowed\n";
		return 0;
	}
} //undo()

double SimpleCalculator::get_res()
{
	return calc_result_;
}

double SimpleCalculator::get_last_arg()
{
	return calc_last_arg_;
}

char SimpleCalculator::get_last_sign()
{
	return calc_last_sign_;
}

bool SimpleCalculator::get_flag()
{
	return flag_;
}

void SimpleCalculator::set_res(double result)
{
	calc_result_ = result;
}

void SimpleCalculator::set_last_arg(double last_arg)
{
	calc_last_arg_ = last_arg;
}

void SimpleCalculator::set_last_sign(char last_sign)
{
	calc_last_sign_ = last_sign;
}

void SimpleCalculator::set_flag(bool flag)
{
	flag_ = flag;
}

void call_info()
{
	std::cout << "info:" << std::endl
		<< "Enter - show info" << std::endl
		<< "Ctrl+Q or Ctrl+Z - exit" << std::endl
		<< "Possible expressions:" << std::endl
		<< "<operand><operation><operand> - ex. 10+2 (clear last result)" << std::endl
		<< "<operation><operand> - ex. +10 (save last result)" << std::endl
		<< "Possible operations:\n +, -, *, /, ^, =, undo" << std::endl;
}

void call_calc()
{
	char operation;
	double op1, op2;
	std::string expression;
	std::size_t found;
	do { // inf cycle, exit with Ctrl+Z or Ctrl+Q
		found = 0;
		expression.clear();
		std::cout << "SimpleCalculator: ";
		std::getline(std::cin, expression); //write expression in console
		if (std::cin.eof() || expression[0] == '\x11') //check exit Ctrl+Z or Ctrl+Q
		{
			break;
		}
		if (isdigit(expression[0])) //if first is digit, call functor with 2 parameters
		{
			if ((found = expression.find_first_of("+-*/^")) != std::string::npos)
			{
				operation = expression[found];
				op1 = std::stod(expression);
				if (isdigit(expression[found + 1]))
				{
					op2 = std::stod(expression.substr(found + 1));
				}
				else
				{
					continue;
				}
				std::cout << op1 << ' ' << operation << ' ' << op2 << " = "
					<< SimpleCalculator(operation)(op1, op2) << std::endl;
			}
			else // continue cycle if expression is wrong or can't be read
			{
				continue;
			}
		}
		else if ((found = expression.find_first_of("+-*/^=")) != std::string::npos) //else if found supported sign, 
		{																			//call functor with 1 parameters
			operation = expression[found];
			
			if (expression[found] == '=')
			{
				std::cout << "last result(" << SimpleCalculator::get_res() << ")";
				if (isdigit(expression[found + 1]))
				{
					op2 = std::stod(expression.substr(found + 1));
					std::cout << " " << operation << " " << SimpleCalculator(operation)(op2) << std::endl;
					continue;
				}
				std::cout << std::endl;
				continue;
				
			}
			else if (isdigit(expression[found + 1]))
			{
				op2 = std::stod(expression.substr(found + 1));
			}
			else // continue cycle if expression is wrong or can't be read
			{
				continue;
			}
			std::cout << "last result(" << SimpleCalculator::get_res() << ") " << operation << " " << op2 << " = ";
			std::cout << SimpleCalculator(operation)(op2) << std::endl;
		}
		else if (found = expression.find("undo") != std::string::npos) //else if found word "undo", call static function undo()
		{
			std::cout << "previous result(" << SimpleCalculator::undo() << ") \n";
		}
		else //else call function call_info()
		{
			call_info();
			continue;
		}
	} while (true);
} //call_calc()
} ////namespace simplecalc