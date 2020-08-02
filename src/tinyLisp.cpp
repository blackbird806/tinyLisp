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

Cell Interpreter::read(std::string const& str)
{
	return read_from(lex(str));
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

Cell Interpreter::eval(Cell const& cell)
{
	if (isPrimitivetype(cell.type))
		return cell;

	if (cell.type == CellType::Symbol)
		return symbols[cell.value];

	if (cell.list.empty())
		return Cell();

	if (cell.list[0].type == CellType::Symbol)
	{
		if (cell.list[0].value == "set")
		{
			return symbols[cell.list[1].value] = eval(cell.list[2]);
		}
		else if (cell.list[0].value == "if")
		{
			return eval(eval(cell.list[1]).bool_value ? cell.list[2] : (cell.list.size() > 2 ? cell.list[3] : Cell()));
		}
		else if (cell.list[0].value == "while")
		{
			while (eval(cell.list[1]).bool_value)
			{
				if (cell.list.size() > 2)
					eval(cell.list[2]);
			}
			return Cell();
		}
	}

	Cell proc = eval(cell.list.front());
	std::vector<Cell> exprs;
	bool skipFirst = true;
	for (auto& expr : cell.list)
	{
		if (skipFirst)
		{
			skipFirst = false;
			continue;
		}
		exprs.push_back(eval(expr));
	}

	return proc.proc(exprs);

	return Cell();
}

Cell Interpreter::evalS(std::string const& str)
{
	return eval(read(str));
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

void Interpreter::set_globals()
{
	symbols["+"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double sum = 0.0f; 
		for (auto const& c : args)
			sum += c.num_value; 
		return Cell(sum); 
	});

	symbols["-"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
			n -= args[i].num_value;
		return Cell(n);
		});

	symbols["*"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double sum = 1.0f;
		for (auto const& c : args)
			sum *= c.num_value;
		return Cell(sum);
		});

	symbols["/"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
			n /= args[i].num_value;
		return Cell(n);
		});

	symbols[">"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
		{
			if (n < args[i].num_value)
				return Cell(false);
		}
		return Cell(true);
		});

	symbols["<"] = Cell([](std::vector<Cell> const& args) -> Cell {
		double n = args[0].num_value;
		for (size_t i = 1; i < args.size(); i++)
		{
			if (n > args[i].num_value)
				return Cell(false);
		}
		return Cell(true);
		});

	symbols["true"] = Cell(true);
	symbols["false"] = Cell(false);

	symbols["print"] = Cell([](std::vector<Cell> const& args) -> Cell {
		for (auto const& c : args)
			print_cell(c);
		return Cell();
		});
}

Interpreter::Interpreter()
{
	set_globals();
}