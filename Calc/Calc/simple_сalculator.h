#pragma once
#include <iostream>
#include <cmath>
namespace simplecalc {
class SimpleCalculator
{
	static char calc_last_sign_;
	static double calc_last_arg_;
	static double calc_result_;
public:
	SimpleCalculator(char sign);
	~SimpleCalculator();
	//methods
	static double undo();
	static double get_res();
	static double get_last_arg();
	static char get_last_sign();
	static void set_res(double result);
	static void set_last_arg(double last_arg);
	static void set_last_sign(char last_sign);
	static void call_info();
	//functors
	double operator()(double operand_lh, double operand_rh);
	double operator()(double operand_rh);
};

void call_calc();
} //namespace simplecalc

