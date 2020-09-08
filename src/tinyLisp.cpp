#include "tinyLisp.h"

#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <numeric>
#include <fstream>

#define ENSURE(cond, fmt, ...) if (!(cond)) runtime_error(fmt, __VA_ARGS__);

using namespace lsp;

static bool isPrimitivetype(CellType t)
{
	return t == CellType::Int || t == CellType::Float || t == CellType::Null || t == CellType::Bool || t == CellType::String;
}

void lsp::runtime_error(const char* fmt, ...)
{
	printf("[lisp error] : ");
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
}

const char* lsp::to_string(CellType c)
{
	switch (c)
	{
	case CellType::Symbol:
		return "Symbol";
	case CellType::Float:
		return "Float";
	case CellType::Int:
		return "Int";
	case CellType::Bool:
		return "Bool";
	case CellType::String:
		return "String";
	case CellType::Null:
		return "Null";
	case CellType::Proc:
		return "Proc";
	case CellType::List:
		return "List";
	default:
		return "Unknown";
	}
}

std::queue<std::string> Interpreter::lex(std::string_view source)
{
	std::queue<std::string> tokens;
	size_t index = 0;
	while (index < source.size())
	{
		while (index < source.size() && isspace(source[index]))
			index++;

		if (index >= source.size())
			break;

		if (source[index] == ';')
		{
			while (index < source.size() && source[index++] != '\n') {}
			continue;
		}

		if (source[index] == '(' || source[index] == ')')
		{
			source[index] == '(' ? tokens.push("(") : tokens.push(")");
			index++;
		}
		else if (source[index] == '"')
		{
			size_t const save = index++;
			while (source[index++] != '"') {};
			tokens.push(std::string(source.substr(save, index - save)));
		}
		else
		{
			size_t const save = index;
			while (!isspace(source[index]) && source[index] != '(' && source[index] != ')')
				index++;
			tokens.push(std::string(source.substr(save, index - save)));
		}
	}

	return tokens;
}

Cell Interpreter::read_from(std::queue<std::string>& tokens)
{
	std::string tk = tokens.front();
	tokens.pop();
	if (tk == "(")
	{
		Cell c{ CellType::List };
		c.value = CellList_t();
		while (tokens.front() != ")")
			std::get<CellList_t>(c.value).push_back(read_from(tokens));

		tokens.pop();
		return c;
	}
	else if (isdigit(tk.front()) || (tk.front() == '-' && isdigit(tk[1])))
	{
		if (tk.find('.') != std::string::npos) // is float
		{
			Cell c{ CellType::Float };
			c.value = std::stod(tk);
			return c;
		}

		Cell c{ CellType::Int };
		c.value = std::stol(tk);
		return c;
	}
	else if (tk.front() == '"')
	{
		Cell c{ CellType::String };
		c.value = tk.substr(1, tk.size() - 2);
		return c;
	}
	else
	{
		Cell c{ CellType::Symbol };
		c.token_str = tk;
		return c;
	}
}

Cell Interpreter::eval(Cell const& cell, Environement& env)
{
	if (isPrimitivetype(cell.type))
		return cell;

	if (cell.type == CellType::Symbol)
	{
		if (env.symbols.find(cell.token_str) != env.symbols.end())
			return env.symbols[cell.token_str];
		else
			return global_env.symbols[cell.token_str];
	}

	auto& list_value = std::get<CellList_t>(cell.value);

	if (list_value.empty())
		return Cell();

	if (list_value[0].type == CellType::Symbol)
	{
		if (list_value[0].token_str == "import")
		{
			ENSURE(list_value[1].type == CellType::String, "a string literal must follow an import !");
			std::string file_name = std::get<std::string>(list_value[1].value);
			if (imported_files.count(file_name) == 0)
			{
				std::ifstream file(file_name);
				std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				evalS(source);
			}
			return Cell();
		}
		else if (list_value[0].token_str == "set")
		{
			return env.symbols[list_value[1].token_str] = eval(list_value[2], env);
		}
		else if (list_value[0].token_str == "setg")
		{
			return global_env.symbols[list_value[1].token_str] = eval(list_value[2], env);
		}
		else if (list_value[0].token_str == "if")
		{
			return eval(std::get<bool>(eval(list_value[1], env).value) ? list_value[2] : (list_value.size() > 3 ? list_value[3] : Cell()), env);
		}
		else if (list_value[0].token_str == "while")
		{
			std::vector<Cell> body = std::vector(list_value.begin() + 2, list_value.end());
			while (std::get<bool>(eval(list_value[1], env).value))
			{
				for (auto const& b : body)
					eval(b, env);
			}
			return Cell();
		}
		else if (list_value[0].token_str == "defun")
		{
			auto& cellList = std::get<CellList_t>(cell.value);
			std::string const func_name = cellList[1].token_str;
			Cell func_args = cellList[2];
			std::vector<Cell> body = std::vector(cellList.begin() + 3, cellList.end());
			Cell fun = { CellType::Proc };
			fun.local_env = Environement();
			fun.value = [this, cell, env, func_name, func_args, body](std::vector<Cell> const& args) mutable -> Cell {
				// add function args in local env
				size_t i = 0;
				Cell& func_sym = env.symbols[func_name];
				func_sym.local_env->symbols.reserve(func_sym.local_env->symbols.size() + std::get<CellList_t>(func_args.value).size());
				for (auto const& arg : std::get<CellList_t>(func_args.value))
					func_sym.local_env->symbols[std::get<std::string>(arg.value)] = args[i++];

				Cell last;
				for (auto const& b : body)
					last = eval(b, *func_sym.local_env);
				return last;
				};

			fun.value = func_name;
			return env.symbols[cellList[1].token_str] = fun;
		}

		if (list_value[0].token_str == "eval")
			return evalS(std::get<std::string>(list_value[1].value), env);

		Cell proc = eval(list_value[0], env);
		if (proc.type == CellType::Proc)
		{
			std::vector<Cell> exprs;
			exprs.reserve(list_value.size());
			bool skipFirst = true;
			for (auto& expr : list_value)
			{
				if (skipFirst)
				{
					skipFirst = false;
					continue;
				}
				exprs.push_back(eval(expr, env));
			}
			return std::get<CellProc_t>(proc.value)(exprs);
		}
		else
		{
			printf("error : symbol %s undefined\n", list_value[0].token_str.c_str());
			return Cell();
		}
	}
}

Cell Interpreter::evalS(std::string const& str)
{
	return evalS(str, global_env);
}

Cell Interpreter::evalS(std::string const& str, Environement& env)
{
	auto tokens = lex(str);
	Cell last;
	while (!tokens.empty())
		last = eval(read_from(tokens), env);
	return last;
}

Interpreter::Interpreter()
{
	{
		Cell retCell = { CellType::Proc };
		retCell.value = [](CellList_t const& args) {
			Cell c{ CellType::List };
			c.value = args;
			return c;
		};
		global_env.symbols["list"] = retCell;
	}
	{
		Cell retCell = { CellType::Proc };
		retCell.value = [](CellList_t const& args) {
			CellType sumType = CellType::Int; // int is the weaker type
			for (auto const& c : args) 
			{ 
				if (c.type == CellType::Float) 
				{ 
					sumType = CellType::Float; 
					break; 
				} 
			} 
			if (sumType == CellType::Float) 
			{ 
				CellFloat_t sum = 0.0; 
				for (auto const& c : args) 
				{ 
					ENSURE(c.type == CellType::Float || c.type == CellType::Int, "only numerical value can be sumed !"); 
					if (c.type == CellType::Float) 
						sum += std::get<CellFloat_t>(c.value); 
					else 
						sum += std::get<CellIntegral_t>(c.value); 
				} 
				Cell ret = { CellType::Float }; 
				ret.value = sum; 
				return ret; 
			} 
			{ // SumType == In
				CellIntegral_t sum = 0; 
				for (auto const& c : args) 
				{ 
					ENSURE(c.type == CellType::Float || c.type == CellType::Int, "only numerical value can be sumed !"); 
					if (c.type == CellType::Float) 
						sum += std::get<CellFloat_t>(c.value); 
					else 
						sum += std::get<CellIntegral_t>(c.value); 
				} 
				Cell ret = { CellType::Int }; 
				ret.value = sum; 
				return ret; 
			} 
		}; 
		global_env.symbols["+"] = retCell;
	}
	{
		Cell retCell = { CellType::Proc };
		retCell.value = [](CellList_t const& args) {
			CellType sumType = CellType::Int; // int is the weaker type
			for (auto const& c : args)
			{
				if (c.type == CellType::Float)
				{
					sumType = CellType::Float;
					break;
				}
			}
			if (sumType == CellType::Float)
			{
				CellFloat_t sum = 0.0;
				if (args[0].type == CellType::Float)
					sum = std::get<CellFloat_t>(args[0].value);
				else // int type
					sum = std::get<CellIntegral_t>(args[0].value);
				for (auto const& c : detail::Range(args.begin() + 1, args.end()))
				{
					ENSURE(c.type == CellType::Float || c.type == CellType::Int, "only numerical value can be sumed !");
					if (c.type == CellType::Float)
						sum -= std::get<CellFloat_t>(c.value);
					else
						sum -= std::get<CellIntegral_t>(c.value);
				}
				Cell ret = { CellType::Float };
				ret.value = sum;
				return ret;
			}
			{ // SumType == In
				CellIntegral_t sum = 0;
				if (args[0].type == CellType::Float)
					sum = std::get<CellFloat_t>(args[0].value);
				else // int type
					sum = std::get<CellIntegral_t>(args[0].value);
				for (auto const& c : detail::Range(args.begin() + 1, args.end()))
				{
					ENSURE(c.type == CellType::Float || c.type == CellType::Int, "only numerical value can be sumed !");
					if (c.type == CellType::Float)
						sum -= std::get<CellFloat_t>(c.value);
					else
						sum -= std::get<CellIntegral_t>(c.value);
				}
				Cell ret = { CellType::Int };
				ret.value = sum;
				return ret;
			}
		};
		global_env.symbols["-"] = retCell;
	}
	{
		Cell retCell = { CellType::Proc };
		retCell.value = [](CellList_t const& args) {
			CellType sumType = CellType::Int; // int is the weaker type
			for (auto const& c : args)
			{
				if (c.type == CellType::Float)
				{
					sumType = CellType::Float;
					break;
				}
			}
			if (sumType == CellType::Float)
			{
				CellFloat_t sum = 1.0;
				for (auto const& c : args)
				{
					ENSURE(c.type == CellType::Float || c.type == CellType::Int, "only numerical value can be multiplied !");
					if (c.type == CellType::Float)
						sum *= std::get<CellFloat_t>(c.value);
					else
						sum *= std::get<CellIntegral_t>(c.value);
				}
				Cell ret = { CellType::Float };
				ret.value = sum;
				return ret;
			}
			{ // SumType == In
				CellIntegral_t sum = 1;
				for (auto const& c : args)
				{
					ENSURE(c.type == CellType::Float || c.type == CellType::Int, "only numerical value can be multiplied !");
					if (c.type == CellType::Float)
						sum *= std::get<CellFloat_t>(c.value);
					else
						sum *= std::get<CellIntegral_t>(c.value);
				}
				Cell ret = { CellType::Int };
				ret.value = sum;
				return ret;
			}
		};
		global_env.symbols["*"] = retCell;
	}
	{
		Cell retCell = { CellType::Proc };
		retCell.value = [](CellList_t const& args) {
			ENSURE(args[0].type == CellType::Float || args[0].type == CellType::Int, "only numerical value can be divided !");
			
			CellFloat_t sum;
			if (args[0].type == CellType::Float)
				sum = std::get<CellFloat_t>(args[0].value);
			else // int type
				sum = std::get<CellIntegral_t>(args[0].value);

			for (auto const& c : detail::Range(args.begin() + 1, args.end()))
			{
				ENSURE(c.type == CellType::Float || c.type == CellType::Int, "only numerical value can be divided !");
				if (c.type == CellType::Float)
					sum /= std::get<CellFloat_t>(c.value);
				else
					sum /= std::get<CellIntegral_t>(c.value);
			}
			Cell ret = { CellType::Float };
			ret.value = sum;
			return ret;
		};
		global_env.symbols["/"] = retCell;
	}
	{
		Cell c = { CellType::Proc };
		c.value = [](CellList_t const& args) {
			for (auto const& a : args)
				printf("%s\n", to_string(a).c_str());
			return Cell();
		};
		global_env.symbols["println"] = c;
	}
}

Cell::Cell() : type(CellType::Null)
{
}

Cell::Cell(CellType t) : type(t)
{
}

std::string lsp::to_string(Cell const& cell)
{
	switch (cell.type)
	{
	case CellType::Float:
		return std::to_string(std::get<CellFloat_t>(cell.value));
	case CellType::Int:
		return std::to_string(std::get<CellIntegral_t>(cell.value));
	case CellType::Bool:
		return std::to_string(std::get<bool>(cell.value));
	case CellType::String:
		return std::get<std::string>(cell.value);
	case CellType::Null:
		return "Null";
	case CellType::Proc:
		return cell.token_str;
	case CellType::List:
	{
		std::string str("( ");
		size_t i = std::get<CellList_t>(cell.value).size();
		for (auto const& c : std::get<CellList_t>(cell.value))
		{
			str += to_string(c);
			if (i > 1) str += ", ";
			i--;
		}
		str += " )";
		return str;
	}
	default:
		return "Unknown";
	}
}