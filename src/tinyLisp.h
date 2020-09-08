#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <variant>
#include <functional>
#include <optional>

namespace lsp {

	namespace detail
	{
		template<typename IT>
		struct Range
		{
			Range(IT&& begin, IT&& end) : b(std::forward<IT>(begin)), e(std::forward<IT>(end)) {};

			auto begin() const noexcept { return b; }
			auto end() const noexcept { return e; }

		private:
			IT b;
			IT e;
		};
	}

	void runtime_error(const char* fmt, ...);

	struct Cell;

	using CellList_t = std::vector<Cell>;
	//using CellProc_t = Cell(*)(CellList_t const&);
	using CellProc_t = std::function<Cell(CellList_t const&)>;
	using CellIntegral_t = long;
	using CellFloat_t = double;

	enum class CellType
	{
		Symbol,
		Int,
		Float,
		Bool,
		String,
		List,
		Proc,
		Null,
	};

	const char* to_string(CellType);
	struct Environement
	{
		std::unordered_map<std::string, Cell> symbols;
	};

	struct Cell
	{
		Cell();
		Cell(CellType t);
		Cell(Cell const&) = default;
		Cell(Cell&&) noexcept = default;
		~Cell() = default;

		Cell& operator=(Cell const&) = default;
		Cell& operator=(Cell&&) noexcept = default;

		CellType type;
		std::string token_str;
		
		std::variant<
			CellIntegral_t,
			CellFloat_t,
			bool,
			std::string,
			CellList_t,
			CellProc_t
		> value;
		std::optional<Environement> local_env;
	};

	std::string to_string(Cell const&);

	class Interpreter
	{

	public:
		Interpreter();
		Cell eval(Cell const& cell, Environement& env);
		Cell evalS(std::string const&, Environement& env);
		Cell evalS(std::string const&);

		Environement global_env;

	private:
		std::unordered_set<std::string> imported_files;

		static std::queue<std::string> lex(std::string_view source);
		static Cell read_from(std::queue<std::string>& tokens);
	};
}