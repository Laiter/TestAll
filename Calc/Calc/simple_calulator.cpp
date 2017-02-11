#include "simple_ñalculator.h"
#include <string>

namespace simplecalc {
char SimpleCalculator::calc_last_sign_;
double SimpleCalculator::calc_last_arg_;
double SimpleCalculator::calc_result_;

SimpleCalculator::SimpleCalculator(char sign)
{
	calc_last_sign_ = sign;
}

SimpleCalculator::~SimpleCalculator()
{
}

double SimpleCalculator::operator()(double operand_lh, double operand_rh)
{
	calc_result_ = 0;
	calc_last_arg_ = 0;
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
	case '=':
	{
		return calc_result_;
	}
	default:
		std::cout << "Operation's not allowed\n";
		return 0;
	}
} //operator()(double operand_lh, double operand_rh)

double SimpleCalculator::operator()(double operand_rh)
{
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
		return calc_result_;
	}
	default:
		std::cout << "Operation's not allowed\n";
		return 0;
	}
} //operator()(double operand_rh)

double SimpleCalculator::undo()
{
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
		static bool flag = 0;
		if (flag == 0)
		{
			calc_result_ = pow(mult, (1 / power));
		}
		else
		{
			calc_result_ = pow(mult, power);
		}
		flag ^= 1;
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

void SimpleCalculator::call_info()
{
	std::cout << "info:\n"
		<< "Enter - show info\n"
		<< "Ctrl+Q or Ctrl+Z - exit\n"
		<< "Possible expressions: \n<operand><operation><operand> - ex. 10+2 (clear last result)\n<operation><operand> - ex. +10 (save last result)"
		<< "\nPossible operations:\n +, -, *, /, ^, =, undo\n";
}

void call_calc()
{
	char operation;
	double op1, op2;
	std::string expression;
	std::size_t found;
	do {
		found = 0;
		expression.clear();
		std::cout << "SimpleCalculator: " << std::flush;
		std::getline(std::cin, expression);
		if (std::cin.eof() || expression[0] == '\x11')
		{
			break;
		}
		if (isdigit(expression[0]))
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
			else
			{
				continue;
			}
		}
		else if ((found = expression.find_first_of("+-*/^=")) != std::string::npos)
		{
			operation = expression[found];
			if (isdigit(expression[found + 1]))
			{
				op2 = std::stod(expression.substr(found + 1));
			}
			else if (expression[found] == '=')
			{
				std::cout << "last result(" << SimpleCalculator::get_res() << ") \n";
				continue;
			}
			else
			{
				continue;
			}
			std::cout << "last result(" << SimpleCalculator::get_res() << ") " << operation << " " << op2 << " = ";
			std::cout << SimpleCalculator(operation)(op2) << std::endl;
		}
		else if (found = expression.find("undo") != std::string::npos)
		{
			std::cout << "previous result(" << SimpleCalculator::undo() << ") \n";
		}
		else
		{
			SimpleCalculator::call_info();
			continue;
		}
	} while (true);
} //call_calc()
} ////namespace simplecalc