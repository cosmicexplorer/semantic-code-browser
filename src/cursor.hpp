#ifndef CURSOR_HPP
#define CURSOR_HPP

// std includes
#include <list>
// external includes
#include <clang-c/Index.h>

namespace semantic_code_browser {
class EntityIndex;

/* cursor variants specification */
// this doesn't matter now, but it will when we introduce aliases, and
// especially when we introduce namespaces
enum Specifier { Type, Value };

enum ScopeSpecifier { Function };

class Cursor {
 protected:
  /* types of cursors used in multiplexing Cursor derived class in MakeCursor */
  static const CXCursorKind TypeDeclCursorKinds[];
  static const CXCursorKind ValDeclCursorKinds[];
  static const CXCursorKind TypeRefCursorKinds[];
  static const CXCursorKind ValRefCursorKinds[];
  static const CXCursorKind ScopeCursorKinds[];

  /* static helper methods */
  /*
    THIS DISPOSES OF THE CXSTRING AND WILL CAUSE MEMORY ERRORS IF YOU ATTEMPT
    TO USE THE CXSTRING AFTER PASSING IT INTO THIS FUNCTION
  */
  static std::string GetStringFromCXString(CXString);
  static std::string GetFileName(CXSourceLocation);
  static unsigned int GetOffset(CXSourceLocation);
  static bool IsDefinition(CXCursor);

  /* used in constructing names for anonymous classes and types */
  static size_t AnonymousCounter;
  static std::string GetName(CXCursor);
  static std::string GetUSR(CXCursor);

  /* constructor and members */
  Cursor(CXCursor);
  // for constructing anonymous types (generating unique id)
  Cursor(CXCursor, size_t);

  const CXCursor mCursor;
  const CXCursorKind mCursorKind;
  const CXSourceLocation mBegin;
  const CXSourceLocation mEnd;
  const std::string mFile;
  const unsigned int mOffset;
  const std::string mName;
  const std::string mUSR;

 public:
  virtual ~Cursor();

  /* mild form of introspection */
  virtual Specifier getSpecifier() const = 0;

  /* entry point to cursor class */
  static Cursor * MakeCursor(CXCursor);

  /* simple accessors */
  const CXCursor & get() const;
  const CXCursorKind & getKind() const;
  const CXSourceLocation & getBegin() const;
  const CXSourceLocation & getEnd() const;
  const std::string & getFile() const;
  const unsigned int & getOffset() const;
  const std::string & getName() const;
  const std::string & getUSR() const;
  bool isAnon() const;

  /* debugging */
  virtual std::string getDerivedType() const;
  static std::string ConvertSpecifier(Specifier s);
  std::string toString() const;

  /* more complex processing */
  /*
    One might naively assume that this calls clang_cursorEquals under the
    covers. In fact, it does not. This is meant to compare two cursors, even
    across translation units, which I'm not certain (?) clang_cursorEquals is
    able to do. The important point is that they refer to the same object, and
    are at the same location.
  */
  bool operator==(Cursor &) const;
  // da bomb
  static std::list<CXCursor> GetEnclosingScope(CXCursor);

  // delegates to appropriate add* function in EntityIndex
  // returns whether or not cursor DIDN'T already exist in index
  virtual bool accept(EntityIndex *) = 0;
};

/* inheritance hierarchy */
template <Specifier S>
class EntityCursor : public Cursor {
 protected:
  EntityCursor(CXCursor);

 public:
  virtual ~EntityCursor();

  virtual bool accept(EntityIndex *) = 0;

  virtual std::string getDerivedType() const;

  Specifier getSpecifier() const;
};

template <Specifier S>
class DeclCursor : public EntityCursor<S> {
 public:
  DeclCursor(CXCursor);
  ~DeclCursor();

  virtual bool accept(EntityIndex *);

  virtual std::string getDerivedType() const;
};

template <Specifier S>
class RefCursor : public EntityCursor<S> {
 public:
  RefCursor(CXCursor);
  ~RefCursor();

  virtual bool accept(EntityIndex *);

  virtual std::string getDerivedType() const;
};

template <Specifier S>
class DefnCursor : public DeclCursor<S> {
 public:
  DefnCursor(CXCursor);
  ~DefnCursor();

  virtual bool accept(EntityIndex *);

  virtual std::string getDerivedType() const;
};
}
#endif /* CURSOR_HPP */