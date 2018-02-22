# Typedef-Gen

Generate typedefs programatically from a grammar file.

## WARNING

HUGELY EXPONENTIAL OUTPUT SIZE GROWTH. MAY FILL YOUR HARDDRIVE WITH TYPEDEFS IF
YOU WRITE A BAD GRAMMAR FILE OR USE LOTS OF LAYERS.

## Requirements

* g++
* make
* boost

## To build

```
make
```

## To run

```
./alias [<layers> [<in> [<out>]]]

# eg.
./alias 2 grammar out
# or
./alias
```

## Sample output

```
typedef std::vector<char> vc;
typedef std::vector<int> vi;
typedef std::vector<long long> vll;
typedef std::map<char,char> mcc;
typedef std::map<int,char> mic;

...

typedef std::vector<vc> vvc;
typedef std::vector<vi> vvi;
typedef std::vector<vll> vvll;

...

typedef std::map<mci,vc> mmcivc;
typedef std::map<mii,vc> mmiivc;

...

and so on, all the way down to those master of types:

typedef std::tuple<tcllll,tllllll,tllllll> ttclllltlllllltllllll;
typedef std::tuple<tillll,tllllll,tllllll> ttilllltlllllltllllll;
typedef std::tuple<tllllll,tllllll,tllllll> ttlllllltlllllltllllll;
```

## Notes

Naming conflicts are possible, for example, with this grammar file:

```
#include <tuple>

long long -> ll
unsigned -> u
unsigned long long -> ull

std::tuple<2-3> t
```

### tl;dr

Rename `u` or `ull` so `ull` doesn't start with the former.

### Explanation

This is because `tullu` could mean `tuple<unsigned long long, unsigned>`, or
`tuple<unsigned, long long, unsigned>` (notice the extra comma). The solution to
this is not to abbreviations that start with the whole of another abbreviation
(in this exmaple `ull` starts with `u`) in grammars which generate
multiple-arity template arguments (tuple can have two or three parameters).
