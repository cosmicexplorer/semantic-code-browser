#ifndef CURSOR_HPP
#define CURSOR_HPP

/* std includes */
#include <string>
#include <unordered_map>
#include <unordered_set>
/* external includes */
#include <clang-c/Index.h>

namespace std {
template <>
struct hash<CXCursorKind> {
  size_t operator()(const CXCursorKind & c) const {
    return std::hash<size_t>()(static_cast<size_t>(c));
  }
};
}

namespace semantic_code_browser {

namespace frontend {

struct ValidityError : public std::runtime_error {
  ValidityError(std::string s) : std::runtime_error(s) {
  }
};

namespace libclang_utils {

/* disposes of input CXString; don't attempt to use or dispose the input after
   calling this */
std::string GetStringAndDispose(CXString);
} /* libclang_utils */

/*
  this class is not type-safe; it is meant to be easily serializable and
  parseable through boost::spirit. a checker function is provided; use it when
  required to ensure invalid values don't sneak into the strings used for
  individual data members.
*/
struct cursor {
 private:
  /* creation of the cursor */
  static std::string setup_cursor_type(CXCursor);
  static std::string setup_entity_spec(CXCursor);
  static std::string setup_type(CXCursor);
  static std::string setup_name(CXCursor);
  /* named differently because they're meant to be used inside the
     constructor, after other elements have been setup already */
  std::tuple<std::string, unsigned int, unsigned int, unsigned int, std::string,
             unsigned int, unsigned int, unsigned int> setupLocations(CXCursor);
  std::string setupScope(CXCursor);

  /* validity checking */
  /*
  types should be formatted according to a regex, but that's kinda difficult due
  to the ambiguity available in type specifications in c/c++. right now it just
  returns false if blank.
  */
  bool isValidType(std::string);
  /* returns false if there are null characters */
  bool isValidFilename(std::string);
  /*
  scopes are formatted according to the following regex:

  (<filename>)?::(identifier::|identifier@)*

  where "filename" is a string identifying the file where the entity is located,
  if the entity has file-local (static) linkage. valid filenames are specified
  as above.

  "identifier" is the regex spelled out above. "::" denotes an enclosing
  namespace or class, while "@" denotes an enclosing function.
  */
  bool isValidScope(std::string);

  /*
  identifiers (in c and c++) are formatted according to the following regex:

  [a-zA-Z_][a-zA-Z_0-9]*
*/
  bool isValidIdentifier(std::string);

 public:
  /* publicly available data */
  static const std::regex ScopeRegex;
  static const std::string IdentifierRegexString;
  static const std::regex IdentifierRegex;
  static const std::unordered_set<std::string> CursorTypes;
  static const std::unordered_set<std::string> EntitySpecifiers;
  static const std::unordered_set<std::string> UntypedEntitySpecifiers;
  /* each scope cursor type is suffixed by a particular string (see ScopeRegex
     below). this contains the lookup table for those cursor types. */
  static const std::unordered_map<CXCursorKind, std::string> ScopeKinds;

  std::string cursorType;
  std::string entitySpec;
  std::string type;
  std::string name;
  /* index contents */
  std::string begin_file;
  /* libclang uses unsigned int for these; if it's good enough for them, it's
     good enough for us */
  unsigned int begin_offset;
  unsigned int begin_line;
  unsigned int begin_col;
  std::string end_file;
  unsigned int end_offset;
  unsigned int end_line;
  unsigned int end_col;
  std::string scope;

  cursor(CXCursor);
  /* copy constructing these things make bookkeeping hard */
  cursor(const cursor &) = delete;

  /*
   if any of the strings don't evaluate to a valid choice for that particular
   element, this returns false. users of this code should probably throw an
   exception, because it means something probably went screwy during parsing
   (or there's a bug in this code; hopefully not!)
  */
  bool isValid();

  /*
    serializes to a line of csv (strings are unquoted because tokens cannot
    have quotes in most languages)
  */
  std::string toString();
}; /* cursor */
} /* frontend */
} /* semantic_code_browser */
#endif /* CURSOR_HPP */
