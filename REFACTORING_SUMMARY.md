# dtree C++ Standard Refactoring - Final Status

## Executive Summary

Two parallel refactoring efforts were undertaken:

1. **cpp17-refactor branch** (C++17, master-based) - ✅ **STABLE**
2. **cpp23-on-coro branch** (C++20/23, coro-based) - ⚠️ **TOOLCHAIN ISSUES**

---

## Branch Status

### cpp17-refactor (C++17)

**Status:** ✅ **WORKING** - 75% complete (6/8 tests passing)

**Base:** upstream/master

**Tests Passing:**
- ✅ out.test.out
- ✅ string.test.out
- ✅ container.test.out
- ✅ number.test.out
- ✅ interval.test.out
- ✅ expr.test.out

**Tests Failing:**
- ❌ sql.test.out (~20 errors remaining)
- ❌ store.test.out (depends on sql)

**Estimated Completion:** 16-20 hours

**Recommendation:** **USE THIS BRANCH** for C++17 projects

---

### cpp23-on-coro (C++20/23)

**Status:** ⚠️ **BLOCKED** - Toolchain issues

**Base:** upstream/coro

**Issue:** Clang 19 crashes when compiling coro branch code with C++20/23

**Root Cause:** 
- upstream/coro was written with early C++20 (experimental coroutine TS)
- Modern clang (19) has issues with the code patterns
- Both C++20 and C++23 cause compiler crashes

**Recommendation:** 
- **Do not use** until toolchain issues are resolved
- Requires either:
  - Older clang version (14-15)
  - Code updates for modern clang
  - upstream/coro branch fixes

---

## Key Accomplishments

### Documentation Created

1. **CPP17_MERGE_GUIDE.md** - How to merge between forks
2. **CPP17_EXPR_REFACTORING.md** - Expression tree C++17 patterns
3. **STORE_REFACTORING_STATUS.md** - store.h tracking
4. **CPP23_ON_CORO.md** - C++23 coro branch documentation
5. **REFACTORING_SUMMARY.md** - This file

### Refactoring Patterns Established

1. **Vector inheritance → composition**
   ```cpp
   // Before
   class X : public std::vector<T> { using base_t::push_back; };
   
   // After
   class X { std::vector<T> base_vec_; void push_back(const T& t) { base_vec_.push_back(t); } };
   ```

2. **Tuple inheritance → composition**
   ```cpp
   // Before
   class Y : public std::tuple<A,B> { auto a() { return std::get<0>(*this); } };
   
   // After
   class Y { A a_; B b_; auto a() { return a_; } };
   ```

3. **Nested class → external template**
   ```cpp
   // Before
   class operation { class ob : public operation { }; };  // Incomplete type
   
   // After
   template<typename V, typename V1> class ob;
   class operation { };
   template<typename V, typename V1> class ob : public operation<V, V1> { };
   ```

4. **base_t:: → this-> for dependent names**
   ```cpp
   // Before
   base_t::branches = b;  // Error in C++17
   
   // After
   this->branches = b;  // Correct
   ```

### Upstream Changes Merged

- CONCEPTS.md from questrel/dtree
- QUICKSTART.md from questrel/dtree

---

## Recommendations

### For Users

**Use cpp17-refactor branch** for:
- C++17 projects
- Maximum compiler compatibility
- Stable expression tree functionality

**Avoid cpp23-on-coro branch** until:
- Toolchain issues resolved
- All tests passing

### For Developers

**Continue cpp17-refactor:**
1. Fix remaining ~20 errors in sql.test.out
2. Get store.test.out working
3. Create PR to questrel/dtree master

**Fix cpp23-on-coro:**
1. Identify specific clang crash trigger
2. Test with older clang (14-15)
3. Update code for modern clang compatibility
4. OR wait for upstream/coro fixes

### For questrel/dtree Maintainers

**Consider:**
1. Adopting C++17 refactoring patterns from cpp17-refactor
2. Fixing coro branch for modern clang
3. Documenting minimum compiler requirements
4. Adding CI for multiple C++ standards

---

## File Changes Summary

| File | cpp17-refactor | cpp23-on-coro |
|------|---------------|---------------|
| qtl/tree.h | ✅ Complete | ⚠️ Partial |
| qtl/radix_map.h | ✅ Complete | ❌ Not started |
| qtl/store.h | ⚠️ 60% | ⚠️ 10% |
| qtl/out.h | ❌ Not started | ⚠️ Fixed TRACE |
| makefile | ✅ C++17 | ⚠️ C++20 (broken) |

---

## Next Steps

### Immediate (cpp17-refactor)

1. Fix `lex::string` operator+= ambiguity
2. Add `intvec::push_back()` method
3. Fix `path::requirements` forward declaration
4. Fix `optional<optree>::admits()` access
5. Get sql.test.out compiling

### Long-term (cpp23-on-coro)

1. Identify clang crash root cause
2. Test with clang 14-15
3. Update code for modern clang
4. Re-enable C++20/23 support
5. Add coroutine tests

---

## Repository Links

- **dmi-000/dtree**: https://github.com/dmi-000/dtree
- **questrel/dtree**: https://github.com/questrel/dtree
- **cpp17-refactor branch**: `git checkout cpp17-refactor`
- **cpp23-on-coro branch**: `git checkout cpp23-on-coro`

---

*Last updated: March 24, 2026*
*Status: cpp17-refactor STABLE, cpp23-on-coro BLOCKED*
