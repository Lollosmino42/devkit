# Update Logs

## Snapshot 8

+ Added new `DktIterator` interface to replace `DktIterable`. Foreach implementation is now much better.
+ Added `release` functions to structs to release ownership of memory when copying the struct.
+ `devkit.h` has now C++ compatibility.
+ `DktVector`, `DktMath` and `DktString` conversion to `DktView` is removed (with functions) as it is unreasonable.
+ Improved `DktString` compatibility with C-strings.
+ New struct `DktSet`

## Snapshot 7

+ New flag `DEVKIT_NO_FOREACH` to disable the 'enhanced for' feature if not needed
+ New flag `DEVKIT_STATIC` to include a static implementation of devkit
+ New struct `DktHeap`

## Snapshot 6

+ `devkit_math.h` header moved inside `devkit.h`, accessible through flags `DEVKIT_MATH` and `DEVKIT_MATH_IMPLEMENTATION`
+ Added `DktView` type which acts as a base type for collections in devkit (`DktList`, `DktArray`)
+ Updated some names
+ Added functions to convert other specific structs (like `DktString` and math structs) to a `DktView`

## What about older versions?

Update log did not exist before snapshot 6. 
There also was a time when devkit was not a single header library.
