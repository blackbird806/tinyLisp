#include "tinyLisp.h"

#include <cctype>
#include <cstdio>
#include <numeric>

bool isPrimitivetype(CellType t)
{
	return t == CellType::Number || t == CellType::null || t == CellType::Bool || t == CellType::String;
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
		if (isblank(source[index]))
			index++;

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
			while (!isblank(source[index]) && source[index] != '(' && source[index] != ')')
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
		if (cell.list[0].value == "set")
		{
			return env.symbols[cell.list[1].value] = eval(cell.list[2], env);
		}
		else if (cell.list[0].value == "if")
		{
			return eval(eval(cell.list[1], env).bool_value ? cell.list[2] : (cell.list.size() > 2 ? cell.list[3] : Cell()), env);
		}
		else if (cell.list[0].value == "while")
		{
			while (eval(cell.list[1], env).bool_value)
			{
				if (cell.list.size() > 2)
					eval(cell.list[2], env);
			}
			return Cell();
		}
		else if (cell.list[0].value == "defun")
		{
			std::string const func_name = cell.list[1].value;
			Cell func_args = cell.list[2];
			Cell body = cell.list[3];

			Cell fun = [this, cell, env, func_name, func_args, body](std::vector<Cell> const& args) mutable -> Cell {
				// add function args in local env
				size_t i = 0;
				Cell func_sym = env.symbols[func_name];
				for (auto& arg : func_args.list)
					func_sym.local_env.symbols[arg.value] = args[i++];

				return eval(body, func_sym.local_env);
			};

			return env.symbols[cell.list[1].value] = fun;
		}
	}

	Cell proc = eval(cell.list.front(), env);
	std::vector<Cell> exprs;
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

	return Cell();
}

Cell Interpreter::evalS(std::string const& str)
{
	auto& tokens = lex(str);
	Cell last;
	while (!tokens.empty())
		last = eval(read_from(tokens), global_env);

	return last;
}

static void print_cell(Cell const& cell)
{
	if (cell.type == CellType::Number)
		printf("%f", cell.num_value);
	else if (cell.type == CellType::null)
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
		for (auto const& c : cell.list)
		{
			print_cell(c);
		}
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

	global_env.symbols[">"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
		{
			if (n < args[i].num_value)
				return Cell(false);
		}
		return Cell(true);
		});

	global_env.symbols["<"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
		{
			if (n > args[i].num_value)
				return Cell(false);
		}
		return Cell(true);
		});

	global_env.symbols["true"] = Cell(true);
	global_env.symbols["false"] = Cell(false);
	global_env.symbols["null"] = Cell();

	global_env.symbols["print"] = Cell([](std::vector<Cell> const& args) -> Cell {
		print_cells(args);
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