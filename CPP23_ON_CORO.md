# C++23 Refactoring on coro Branch

## Overview

This branch applies C++23 compatibility patterns to the **upstream/coro** branch of questrel/dtree.

**Note:** The upstream/coro branch was written with early C++20. We're upgrading to **final C++23** for:
- Stable coroutine TS (not experimental)
- Better template error messages
- Improved constexpr support
- Modern standard library features

## Base Branch

- **Parent**: `upstream/coro` (questrel/dtree coro branch)
- **Commit**: 15a64a1 "Add files via upload"
- **Standard**: C++23 (upgraded from early C++20)

## Key Differences from cpp17-refactor Branch

| Aspect | cpp17-refactor | cpp23-on-coro |
|--------|---------------|---------------|
| Base | upstream/master | upstream/coro |
| Standard | C++17 | **C++23** |
| Coroutines | ❌ No | ✅ Yes (stable TS) |
| store.h version | Older | Newer with coro support |
| Fuzzing corpus | Full | Cleaned up |
| Compiler req | C++17 | C++23 (clang 16+) |

## Refactoring Status

### Completed

- [x] Makefile updated for C++23
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
- [ ] Coroutine integration test - pending

## C++23 Features Used

1. **Stable Coroutine TS**
   ```cpp
   #include <coroutine>  // Not experimental
   ```

2. **Deduction Guides**
   - Cleaner template instantiation

3. **Constexpr Improvements**
   - More compile-time evaluation

4. **Modern Standard Library**
   - `std::span`
   - `std::format` (optional)
   - Improved containers

## Merge Strategy

### To upstream/coro

1. Complete C++23 refactoring
2. Ensure all tests pass
3. Create PR to questrel/dtree coro branch
4. Document C++23 requirement

### To cpp17-refactor (master-based)

1. Cherry-pick relevant commits
2. Resolve conflicts with master-based changes
3. Maintain both branches in sync where possible

## Next Steps

1. Continue systematic refactoring of store.h classes
2. Test coroutines with C++23 changes
3. Verify fuzzing tests still pass
4. Document any C++23-specific issues
5. Update documentation for C++23 requirement

---

*Created: March 24, 2026*
*Updated: March 24, 2026 (C++23 upgrade)*
*Base: upstream/coro @ 15a64a1*
