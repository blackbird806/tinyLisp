#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <string>

namespace lsp {

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

	const char* to_string(CellType);
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

	void print_cell(Cell const& c);
	std::string to_string(Cell const& c);

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
		std::unordered_set<std::string> imported_files;

		static std::queue<std::string> lex(std::string_view source);
		static Cell read_from(std::queue<std::string>& tokens);
	};
}