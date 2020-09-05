#include "tinyLisp.h"

#include <cctype>
#include <cstdio>
#include <cmath>
#include <numeric>
#include <fstream>

// max 20 args with format fct
#define FMT_CELL_CASES \
case 1: printf(args[0].value.c_str()); break;\
case 2: printf(args[0].value.c_str(), to_string(args[1]).c_str()); break;\
case 3: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str()); break;\
case 4: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str()); break;\
case 5: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str()); break;\
case 6: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str()); break;\
case 7: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str()); break;\
case 8: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str()); break;\
case 9: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str()); break;\
case 10: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str()); break;\
case 11: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str()); break;\
case 12: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str(), to_string(args[11]).c_str()); break;\
case 13: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str(), to_string(args[11]).c_str(), to_string(args[12]).c_str()); break;\
case 14: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str(), to_string(args[11]).c_str(), to_string(args[12]).c_str(), to_string(args[13]).c_str()); break;\
case 15: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str(), to_string(args[11]).c_str(), to_string(args[12]).c_str(), to_string(args[13]).c_str(), to_string(args[14]).c_str()); break;\
case 16: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str(), to_string(args[11]).c_str(), to_string(args[12]).c_str(), to_string(args[13]).c_str(), to_string(args[14]).c_str(), to_string(args[15]).c_str()); break;\
case 17: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str(), to_string(args[11]).c_str(), to_string(args[12]).c_str(), to_string(args[13]).c_str(), to_string(args[14]).c_str(), to_string(args[15]).c_str(), to_string(args[16]).c_str()); break;\
case 18: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str(), to_string(args[11]).c_str(), to_string(args[12]).c_str(), to_string(args[13]).c_str(), to_string(args[14]).c_str(), to_string(args[15]).c_str(), to_string(args[16]).c_str(), to_string(args[17]).c_str()); break;\
case 19: printf(args[0].value.c_str(), to_string(args[1]).c_str(), to_string(args[2]).c_str(), to_string(args[3]).c_str(), to_string(args[4]).c_str(), to_string(args[5]).c_str(), to_string(args[6]).c_str(), to_string(args[7]).c_str(), to_string(args[8]).c_str(), to_string(args[9]).c_str(), to_string(args[10]).c_str(), to_string(args[11]).c_str(), to_string(args[12]).c_str(), to_string(args[13]).c_str(), to_string(args[14]).c_str(), to_string(args[15]).c_str(), to_string(args[16]).c_str(), to_string(args[17]).c_str(), to_string(args[18]).c_str()); break;\

using namespace lsp;

static bool isPrimitivetype(CellType t)
{
	return t == CellType::Number || t == CellType::Null || t == CellType::Bool || t == CellType::String;
}

const char* lsp::to_string(CellType c)
{
	switch (c)
	{
	case CellType::Symbol:
		return "Symbol";
	case CellType::Number:
		return "Number";
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

Cell::Cell(CellType t) : type(t)
{

}

Cell::Cell(CellType t, std::string str) : type(t), value(str)
{

}

Cell::Cell(double v) : type(CellType::Number), num_value(v)
{

}

Cell::Cell(bool v) : type(CellType::Bool), bool_value(v)
{

}

Cell::Cell(std::vector<Cell> const& l) : type(CellType::List), list(l)
{

}

Cell::Cell(std::function<Cell(std::vector<Cell> const&)> p) : type(CellType::Proc), proc(p)
{

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
		while (tokens.front() != ")")
			c.list.push_back(read_from(tokens));

		tokens.pop();
		return c;
	}
	else if (isdigit(tk.front()) || (tk.front() == '-' && isdigit(tk[1])))
	{
		Cell c{ std::stod(tk) };
		return c;
	}
	else if (tk.front() == '"')
	{
		Cell c{ CellType::String, tk.substr(1, tk.size() - 2)};
		return c;
	}
	else
	{
		Cell c{ CellType::Symbol };
		c.value = tk;
		return c;
	}
}

Cell Interpreter::eval(Cell const& cell, Environement& env)
{
	if (isPrimitivetype(cell.type))
		return cell;

	if (cell.type == CellType::Symbol)
	{
		if (env.symbols.find(cell.value) != env.symbols.end())
			return env.symbols[cell.value];
		else
			return global_env.symbols[cell.value];
	}

	if (cell.list.empty())
		return Cell();

	if (cell.list[0].type == CellType::Symbol)
	{
		if (cell.list[0].value == "import")
		{
			std::string file_name = cell.list[1].value;
			if (imported_files.count(file_name) == 0)
			{
				std::ifstream file(file_name);
				std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				evalS(source);
			}
			return Cell();
		}
		else if (cell.list[0].value == "set")
		{
			return env.symbols[cell.list[1].value] = eval(cell.list[2], env);
		}
		else if (cell.list[0].value == "setg")
		{
			return global_env.symbols[cell.list[1].value] = eval(cell.list[2], env);
		}
		else if (cell.list[0].value == "if")
		{
			return eval(eval(cell.list[1], env).bool_value ? cell.list[2] : (cell.list.size() > 3 ? cell.list[3] : Cell()), env);
		}
		else if (cell.list[0].value == "while")
		{
			std::vector<Cell> body = std::vector(cell.list.begin() + 2, cell.list.end());
			while (eval(cell.list[1], env).bool_value)
			{
				for (auto const& b : body)
					eval(b, env);
			}
			return Cell();
		}
		else if (cell.list[0].value == "defun")
		{
			std::string const func_name = cell.list[1].value;
			Cell func_args = cell.list[2];
			std::vector<Cell> body = std::vector(cell.list.begin() + 3, cell.list.end());

			Cell fun = Cell([this, cell, env, func_name, func_args, body](std::vector<Cell> const& args) mutable -> Cell {
				// add function args in local env
				size_t i = 0;
				Cell& func_sym = env.symbols[func_name];
				func_sym.local_env.symbols.reserve(func_sym.local_env.symbols.size() + func_args.list.size());
				for (auto const& arg : func_args.list)
					func_sym.local_env.symbols[arg.value] = args[i++];

				Cell last;
				for (auto const& b : body)
					last = eval(b, func_sym.local_env);
				return last;
				});
			fun.value = func_name;
			return env.symbols[cell.list[1].value] = fun;
		}
		else if (cell.list[0].value == "typeof")
		{
			std::vector<Cell> typeList;
			typeList.reserve(cell.list.size() - 1);

			for (auto arg = cell.list.begin() + 1; arg != cell.list.end(); ++arg)
			{
				std::string typeName = "Null";

				if (arg->type == CellType::Symbol)
					typeName = to_string(env.symbols[arg->value].type);
				else
					typeName = to_string(arg->type);

				typeList.emplace_back(CellType::String, typeName);
			}

			return Cell(typeList);
		}

		if (cell.list[0].value == "eval")
			return evalS(cell.list[1].value, env);

		Cell proc = eval(cell.list[0], env);
		if (proc.type == CellType::Proc)
		{
			std::vector<Cell> exprs;
			exprs.reserve(cell.list.size());
			bool skipFirst = true;
			for (auto& expr : cell.list)
			{
				if (skipFirst)
				{
					skipFirst = false;
					continue;
				}
				exprs.push_back(eval(expr, env));
			}
			return proc.proc(exprs);
		}
		else
		{
			printf("error : symbol %s undefined\n", cell.list[0].value.c_str());
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

void lsp::print_cell(Cell const& cell)
{
	if (cell.type == CellType::Number)
		printf("%f", cell.num_value);
	else if (cell.type == CellType::Null)
		printf("null");
	else if (cell.type == CellType::String)
		printf("%s", cell.value.c_str());
	else if (cell.type == CellType::Bool)
	{
		if (cell.bool_value)
			printf("true");
		else
			printf("false");
	}
	else if (cell.type == CellType::List)
	{
		printf("( ");
		size_t i = cell.list.size();
		for (auto const& c : cell.list)
		{
			print_cell(c);
			if (i > 1) printf(", ");
			i--;
		}
		printf(" )");
	}
	else if (cell.type == CellType::Proc)
	{
		printf("%s", cell.value.c_str());
	}
}

std::string lsp::to_string(Cell const& cell)
{
	switch (cell.type)
	{
	case CellType::Number:
		return std::to_string(cell.num_value);
	case CellType::Bool:
		return std::to_string(cell.bool_value);
	case CellType::String:
	case CellType::Proc:
		return cell.value;
	case CellType::List:
	{
		std::string str("( ");
		size_t i = cell.list.size();
		for (auto const& c : cell.list)
		{
			str += to_string(c);
			if (i > 1) printf(", ");
			i--;
		}
		str += " )";
		return str;
	}
	case CellType::Null:
		return "Null";
	default:
		return "Invalid";
	}
}

static void print_cells(std::vector<Cell> const& args)
{
	for (auto const& c : args)
		print_cell(c);
}

void Interpreter::set_globals()
{
	global_env.symbols["+"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double sum = 0.0f; 
		for (auto const& c : args)
			sum += c.num_value; 
		return Cell(sum); 
	});

	global_env.symbols["-"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
			n -= args[i].num_value;
		return Cell(n);
		});

	global_env.symbols["*"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double sum = 1.0f;
		for (auto const& c : args)
			sum *= c.num_value;
		return Cell(sum);
		});

	global_env.symbols["/"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
			n /= args[i].num_value;
		return Cell(n);
		});

	global_env.symbols["%"] = Cell([](std::vector<Cell> const& args) -> Cell {
		return Cell(fmod(args[0].num_value, args[1].num_value));
		});

	global_env.symbols[">"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
		{
			if (n <= args[i].num_value)
				return Cell(false);
		}
		return Cell(true);
		});

	global_env.symbols["="] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
		{
			if (n != args[i].num_value)
				return Cell(false);
		}
		return Cell(true);
		});

	global_env.symbols["<"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
		{
			if (n >= args[i].num_value)
				return Cell(false);
		}
		return Cell(true);
		});

	global_env.symbols["is_null"] = Cell([](std::vector<Cell> const& args) -> Cell {
		return args[0].type == CellType::Null;
		});

	global_env.symbols["not"] = Cell([](std::vector<Cell> const& args) -> Cell {
		return !args[0].bool_value;
		});

	global_env.symbols["true"] = Cell(true);
	global_env.symbols["false"] = Cell(false);
	global_env.symbols["null"] = Cell();

	global_env.symbols["list"] = Cell([](std::vector<Cell> const& args) -> Cell {
			Cell c{ CellType::List };
			c.list = args;
			return c;
		});

	global_env.symbols["return"] = Cell([](std::vector<Cell> const& args) -> Cell {
		return args[0];
		});

	global_env.symbols["append"] = Cell([](std::vector<Cell> const& args) -> Cell {
			Cell c{ CellType::List };
			c.list = args[0].list;
			for (size_t i = 1; i < args.size(); i++)
				c.list.push_back(args[i]);
			return c;
		});

	global_env.symbols["get"] = Cell([](std::vector<Cell> const& args) -> Cell {
		try {
			return Cell(args[0].list.at(round(args[1].num_value)));
		}
		catch (std::out_of_range&)
		{
			return Cell();
		}
		});

	global_env.symbols["length"] = Cell([](std::vector<Cell> const& args) -> Cell {
		return Cell((double)args[0].list.size());
		});

	global_env.symbols["print"] = Cell([](std::vector<Cell> const& args) -> Cell {
		print_cells(args);
		return Cell();
		});

	global_env.symbols["format"] = Cell([](std::vector<Cell> const& args) -> Cell {

		switch (args.size())
		{
			FMT_CELL_CASES
		}

		return Cell();
		});

	global_env.symbols["println"] = Cell([](std::vector<Cell> const& args) -> Cell {
		print_cells(args);
		puts("");
		return Cell();
		});
}

Interpreter::Interpreter()
{
	set_globals();
}