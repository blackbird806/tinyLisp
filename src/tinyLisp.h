#pragma once
#include <string_view>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

enum class CellType
{
	Symbol,
	Number,
	Bool,
	String,
	List,
	Proc,
	null,
};

bool isPrimitivetype(CellType t);

struct Cell
{
	Cell(CellType = CellType::null);
	Cell(CellType, std::string);
	Cell(double);
	Cell(bool);
	Cell(std::vector<Cell> const&);
	Cell(std::function<Cell(std::vector<Cell> const&)>);

	CellType type;
	std::string value;
	double num_value;
	bool bool_value;
	std::function<Cell(std::vector<Cell> const&)> proc;
	std::vector<Cell> list;
};

class Interpreter
{

public:
	Interpreter();
	Cell eval(Cell const& cell);
	Cell evalS(std::string const&);

private:

	void set_globals();

	static std::queue<std::string> lex(std::string_view source);
	static Cell read(std::string const& str);
	static Cell read_from(std::queue<std::string>& tokens);

	std::unordered_map<std::string, Cell> symbols;
};