# C++23 Refactoring on coro Branch

## Overview

This branch applies C++23 compatibility patterns to the **upstream/coro** branch of questrel/dtree, which includes:
- C++ coroutines support (`<experimental/coroutine>`, cppcoro library)
- Recent store.h improvements
- Fuzzing test corpus cleanup

**Note:** The upstream/coro branch was written with early C++20. This branch upgrades to **final C++23** for:
- Stable coroutine TS
- Better template error messages
- Improved constexpr support
- Modern standard library features

## Base Branch

- **Parent**: `upstream/coro` (questrel/dtree coro branch)
- **Commit**: 15a64a1 "Add files via upload"
- **Standard**: C++23 (upgraded from early C++20)

## Key Differences from cpp17-refactor Branch

| Aspect | cpp17-refactor | cpp17-on-coro |
|--------|---------------|---------------|
| Base | upstream/master | upstream/coro |
| Coroutines | ❌ No | ✅ Yes |
| store.h version | Older | Newer with coro support |
| Fuzzing corpus | Full | Cleaned up |

## Refactoring Status

### Completed

- [x] `sample<T>` class - vector → composition
- [ ] `row` class - pending
- [ ] `rows` class - pending
- [ ] `path` class - pending
- [ ] `path::requirements` - pending
- [ ] `parts` class - pending
- [ ] `splits` class - pending
- [ ] `vertex` class - pending
- [ ] `kve` class - pending
- [ ] `stack` class - pending
- [ ] `lattice` class - pending

### Build Status

```
Test | Status
-----|--------
all  | Pending
```

## Merge Strategy

### To upstream/coro

1. Complete C++17 refactoring
2. Ensure all tests pass
3. Create PR to questrel/dtree coro branch
4. Document C++17 requirement

### To cpp17-refactor (master-based)

1. Cherry-pick relevant commits
2. Resolve conflicts with master-based changes
3. Maintain both branches in sync where possible

## Coroutines Integration

The coro branch adds:
```cpp
#include <experimental/coroutine>
#include <cppcoro/recursive_generator.hpp>
```

This enables:
- Lazy evaluation of store queries
- Efficient iteration over large result sets
- Better memory usage for recursive operations

## C++17 Patterns Applied

Same patterns as cpp17-refactor branch:
1. Vector/tuple inheritance → composition
2. Nested class → external template
3. `base_t::` → `this->` for dependent names

## Next Steps

1. Continue systematic refactoring of store.h classes
2. Test coroutines with C++17 changes
3. Verify fuzzing tests still pass
4. Document any coro-specific C++17 issues

---

*Created: March 24, 2026*
*Base: upstream/coro @ 15a64a1*
