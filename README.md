# Typedef-Gen

Generate typedefs programatically from a grammar file.

## WARNING

HUGELY EXPONENTIAL PROGRAM. MAY FILL YOUR HARDDRIVE WITH TYPEDEFS IF YOU WRITE A
BAD GRAMMAR FILE OR USE LOTS OF LAYERS.

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
typedef vector<c> vc;
typedef vector<i> vi;
typedef vector<ll> vll;

...

typedef vector<vc> vvc;
typedef vector<vi> vvi;
typedef vector<vll> vvll;

...

typedef map<vc,vi> mvcvi;
typedef map<vi,vi> mvivi;
typedef map<vll,vi> mvllvi;
```

This, but for all the types outlined in your grammar file (see grammar file),
recursively.
