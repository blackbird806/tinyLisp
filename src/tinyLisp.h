#pragma once
#include <string_view>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <optional>

enum class CellType
{
	Symbol,
	Number,
	Bool,
	String,
	List,
	Proc,
	Null,
};

struct Cell;
struct Environement
{
	std::unordered_map<std::string, Cell> symbols;
};

struct Cell
{
	Cell(CellType = CellType::Null);
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

	Environement local_env;
};

class Interpreter
{

public:
	Interpreter();
	Cell eval(Cell const& cell, Environement& env);
	Cell evalS(std::string const&, Environement& env);
	Cell evalS(std::string const&);

	Environement global_env;

private:
	void set_globals();

	static std::queue<std::string> lex(std::string_view source);
	static Cell read_from(std::queue<std::string>& tokens);
};