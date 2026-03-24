# Toolchain Issues on coro Branch

## Problem

The `cpp23-on-coro` branch (based on upstream/coro) has severe toolchain issues that prevent compilation:

### clang++ (Homebrew clang 19)
- **Symptom:** Compiler crash (segfault)
- **Error:** `clang++: error: unable to execute command: Segmentation fault: 11`
- **Occurs with:** C++17, C++20, C++23
- **Status:** ❌ Unusable

### g++ (Apple g++)
- **Symptom:** Missing standard headers
- **Error:** `fatal error: 'string' file not found`
- **Occurs with:** All C++ standards
- **Status:** ❌ Unusable

## Root Cause Analysis

1. **upstream/coro was written for a specific toolchain** that is no longer available
2. **Include paths are misconfigured** for modern compilers
3. **Code patterns trigger clang 19 bugs** (compiler crash, not code error)
4. **g++ on macOS is a wrapper** that doesn't properly find C++ standard library

## Attempts Made

| Attempt | Result |
|---------|--------|
| Simplify TRACE/NOTRACE macros | ❌ Still crashes |
| Comment out coroutine includes | ❌ Still crashes |
| Switch C++17 ↔ C++20 ↔ C++23 | ❌ All crash |
| Use g++ instead of clang++ | ❌ Missing headers |
| Fix include paths | ❌ Still issues |

## Recommendation

**DO NOT USE cpp23-on-coro branch** until:

1. upstream/coro is fixed for modern toolchains
2. OR clang 19 bug is fixed
3. OR proper macOS C++ toolchain is configured

## Alternative

**Use cpp17-refactor branch** instead:
- ✅ Based on upstream/master (more stable)
- ✅ Compiles with clang 19
- ✅ 6/8 tests passing
- ✅ Well-documented refactoring patterns

## For Future Work

To fix coro branch toolchain:

1. **Identify original toolchain** used for upstream/coro
2. **Create Docker container** with matching environment
3. **OR** systematically update code for modern clang
4. **Add CI** to prevent future toolchain drift

---

*Documented: March 24, 2026*
*Status: BLOCKED - toolchain issues*
