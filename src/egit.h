#include "emacs-module.h"
#include "git2.h"
#include "uthash.h"
#include "git2.h"

#ifndef EGIT_H
#define EGIT_H

/**
 * Macro that defines a docstring for a function.
 * @param name The function name (without egit_ prefix).
 * @param args The argument list as visible from Emacs (without parens).
 * @param docstring The rest of the documentation.
 */
#define EGIT_DOC(name, args, docstring)                                 \
    const char *egit_##name##__doc = (docstring "\n\n(fn " args ")")

/**
 * Macro that declares a function and its docstring variable.
 * @param name The function name (without egit_ prefix)
 * @param ... The function arguments (without emacs_env)
 */
#define EGIT_DEFUN(name, ...)                                   \
    extern const char *egit_##name##__doc;                      \
    emacs_value egit_##name(emacs_env *env, __VA_ARGS__)

/**
 * Variant of EGIT_DEFUN for zero parameters.
 */
#define EGIT_DEFUN_0(name)                      \
    extern const char *egit_##name##__doc;      \
    emacs_value egit_##name(emacs_env *env)

/**
 * Assert that VAL is a function, signal an error and return otherwise.
 */
#define EGIT_ASSERT_FUNCTION(val)                                       \
    do { if (!em_assert(env, em_functionp, (val))) return em_nil; } while (0)

/**
 * Assert that VAL is a string, signal an error and return otherwise.
 */
#define EGIT_ASSERT_STRING(val)                                         \
    do { if (!em_assert(env, em_stringp, (val))) return em_nil; } while (0)

/**
 * Assert that VAL is a string or nil, signal an error and return otherwise.
 */
#define EGIT_ASSERT_STRING_OR_NIL(val) \
    do { if (EGIT_EXTRACT_BOOLEAN(val)) EGIT_ASSERT_STRING(val); } while (0)

/**
 * Assert that VAL is an integer, signal an error and return otherwise.
 */
#define EGIT_ASSERT_INTEGER(val)                                        \
    do { if (!em_assert(env, em_integerp, (val))) return em_nil; } while (0)

/**
 * Assert that VAL is an integer or nil, signal an error and return otherwise.
 */
#define EGIT_ASSERT_INTEGER_OR_NIL(val) \
    do { if (EGIT_EXTRACT_BOOLEAN(val)) EGIT_ASSERT_INTEGER(val); } while (0)

/**
 * Assert that VAL is a git blame, signal an error and return otherwise.
 */
#define EGIT_ASSERT_BLAME(val)                                      \
    do { if (!egit_assert_type(env, (val), EGIT_BLAME, em_libgit_blame_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a git commit, signal an error and return otherwise.
 */
#define EGIT_ASSERT_COMMIT(val)                                         \
    do { if (!egit_assert_type(env, (val), EGIT_COMMIT, em_libgit_commit_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a git config, signal an error and return otherwise.
 */
#define EGIT_ASSERT_CONFIG(val)                                         \
    do { if (!egit_assert_type(env, (val), EGIT_CONFIG, em_libgit_config_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a git index, signal an error and return otherwise.
 */
#define EGIT_ASSERT_INDEX(val)                                          \
    do { if (!egit_assert_type(env, (val), EGIT_INDEX, em_libgit_index_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a git index entry, signal an error and return otherwise.
 */
#define EGIT_ASSERT_INDEX_ENTRY(val)                                    \
    do { if (!egit_assert_type(env, (val), EGIT_INDEX_ENTRY, em_libgit_index_entry_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a git object, signal an error and return otherwise.
 */
#define EGIT_ASSERT_OBJECT(val)                                         \
    do { if (!egit_assert_object(env, (val))) return em_nil; } while (0)

/**
 * Assert that VAL is a git reference, signal an error and return otherwise.
 */
#define EGIT_ASSERT_REFERENCE(val)                                      \
    do { if (!egit_assert_type(env, (val), EGIT_REFERENCE, em_libgit_reference_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a git repository, signal an error and return otherwise.
 */
#define EGIT_ASSERT_REPOSITORY(val)                                     \
    do { if (!egit_assert_type(env, (val), EGIT_REPOSITORY, em_libgit_repository_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a signature, signal an error and return otherwise.
 */
#define EGIT_ASSERT_SIGNATURE(val)                                     \
    do { if (!egit_assert_type(env, (val), EGIT_SIGNATURE, em_libgit_signature_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a transaction, signal an error and return otherwise.
 */
#define EGIT_ASSERT_TRANSACTION(val)                                    \
    do { if (!egit_assert_type(env, (val), EGIT_TRANSACTION, em_libgit_transaction_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a transaction, signal an error and return otherwise.
 */
#define EGIT_ASSERT_TREE(val)                                           \
    do { if (!egit_assert_type(env, (val), EGIT_TREE, em_libgit_tree_p)) return em_nil; } while (0)

/**
 * Assert that VAL is a signature or nil, signal an error and return otherwise.
 */
#define EGIT_ASSERT_SIGNATURE_OR_NIL(val) \
    do { if (EGIT_EXTRACT_BOOLEAN(val)) EGIT_ASSERT_SIGNATURE(val); } while (0)

/**
 * Normalize an emacs_value string path. This macro may return.
 */
#define EGIT_NORMALIZE_PATH(val)\
    do {                                                        \
        (val) = em_expand_file_name(env, val);                  \
        if (env->non_local_exit_check(env)) return em_nil;      \
    } while (0)

/**
 * Extract a libgit git_??? struct from an emacs_value.
 * Caller is responsible for ensuring that this is a valid operation.
 */
#define EGIT_EXTRACT(val) (((egit_object*)env->get_user_ptr(env, (val)))->ptr)

/**
 * Extract a libgit git_??? struct from an emacs_value, or NULL.
 * Caller is responsible for ensuring that this is a valid operation.
 */

#define EGIT_EXTRACT_OR_NULL(val)                                \
  (EGIT_EXTRACT_BOOLEAN(val) ? EGIT_EXTRACT(val) : NULL);

/**
 * Extract a boolean from an emacs_value.
 */
#define EGIT_EXTRACT_BOOLEAN(val) (env->is_not_nil(env, (val)) ? 1 : 0)

/**
 * Extract a string from an emacs_value.
 * Caller is reponsible for ensuring that the emacs_value represents a string.
 */
#define EGIT_EXTRACT_STRING(val) em_get_string(env, (val));

/**
 * Extract an integer from an emacs_value.
 * Caller is reponsible for ensuring that the emacs_value represents an integer.
 */
#define EGIT_EXTRACT_INTEGER(val) env->extract_integer(env, (val))

/**
 * Extract an integer from an emacs_value with a default.
 * Caller is reponsible for ensuring that the emacs_value represents an integer.
 */
#define EGIT_EXTRACT_INTEGER_OR_DEFAULT(val, default)                   \
    (EGIT_EXTRACT_BOOLEAN(val) ? EGIT_EXTRACT_INTEGER(val) : (default))

/**
 * Extract a string from an emacs_value, or NULL.
 * Caller is reponsible for ensuring that the emacs_value represents a string or nil.
 */
#define EGIT_EXTRACT_STRING_OR_NULL(val)                                \
    (EGIT_EXTRACT_BOOLEAN(val) ? em_get_string(env, (val)) : NULL);

/**
 * Free a pointer if it is non-NULL.
 */
#define EGIT_FREE(val) do { if (val) free(val); } while (0)

/**
 * Extract a git_oid from an emacs_value.
 * Caller is responsible for ensuring that the emacs_value is a string.
 */
#define EGIT_EXTRACT_OID(val, tgt)                      \
    do {                                                \
        char *__str = em_get_string(env, (val));        \
        int __retval = git_oid_fromstrp(&(tgt), __str); \
        free(__str);                                    \
        EGIT_CHECK_ERROR(__retval);                     \
    } while (0)

/**
 * Extract a partial git_oid from an emacs_value and store its length.
 * Caller is responsible for ensuring that the emacs_value is a string.
 */
#define EGIT_EXTRACT_OID_PREFIX(val, tgt, tgt_len)      \
    do {                                                \
        char *__str = em_get_string(env, (val));        \
        tgt_len = strlen(__str);                        \
        int __retval = git_oid_fromstrp(&(tgt), __str); \
        free(__str);                                    \
        EGIT_CHECK_ERROR(__retval);                     \
    } while (0)

/**
 * If libgit2 signalled an error, pass the error on to Emacs and return.
 * @param val A libgit2 return value (negative value indicates error).
 */
#define EGIT_CHECK_ERROR(val)                                           \
    do { if (egit_dispatch_error(env, (val))) return em_nil; } while (0)

/**
 * Convert a git_buf to an Emacs string and return it, freeing the git_buf in the process.
 */
#define EGIT_RET_BUF_AS_STRING(buf)                                     \
    do {                                                                \
        emacs_value ret = env->make_string(env, (buf).ptr, (buf).size); \
        git_buf_free(&(buf));                                           \
        return ret;                                                     \
    } while (0)

/**
 * Initiate a loop over an Emacs list.
 * If any element is not a cons cell or nil, it WILL signal an error and return nil.
 * @param var Variable bound to each car.
 * @param listvar List to loop over.
 * @param name Unique name identifying the loop.
 */
#define EGIT_DOLIST(var, listvar, name)                             \
    emacs_value __cell##name = (listvar);                           \
    __loop##name:                                                   \
    if (!EGIT_EXTRACT_BOOLEAN(__cell##name)) goto __end##name;      \
    if (!em_assert(env, em_cons_p, __cell##name)) return em_nil;    \
    emacs_value (var) = em_car(env, __cell##name)

/**
 * Close a loop over an Emacs lisp.
 * @param name: Unique name identifying the loop.
 */
#define EGIT_DOLIST_END(name)                   \
    __cell##name = em_cdr(env, __cell##name);   \
    goto __loop##name;                          \
    __end##name:

/**
 * Enum used to distinguish between various types of git_??? structs.
 */
typedef enum {
    EGIT_UNKNOWN,
    EGIT_REPOSITORY,
    EGIT_REFERENCE,
    EGIT_COMMIT,
    EGIT_TREE,
    EGIT_BLOB,
    EGIT_TAG,
    EGIT_OBJECT,
    EGIT_SIGNATURE,
    EGIT_BLAME,
    EGIT_CONFIG,
    EGIT_TRANSACTION,
    EGIT_INDEX,
    EGIT_INDEX_ENTRY
} egit_type;

/**
 * Hashable wrapper structure for a git_??? struct.
 * These are used in an internal object hash-table to ensure that pointers aren't freed too early.
 * This is necessary since there may be many emacs_values that point to the same git_repository
 * (for example), so we can't just free the git_repository pointer in a finalizer called from Emacs.
 * Moreover, emacs_values that store e.g. git_object pointers must keep the git_repository alive.
 * To fix this we keep a hash table of pointers mapping to egit_objects, counting references and
 * freeing objects when they reach zero.
 *
 * User-pointers returned to Emacs should always wrap a struct of type egit_object.
 */
typedef struct {
    UT_hash_handle hh;          /**< For internal use by the hash table. */
    egit_type type;             /**< Type of object stored. */
    ptrdiff_t refcount;         /**< Reference count. */
    void *ptr;                  /**< Pointer to git_??? structure. */
} egit_object;

/**
 * Return the git object type stored by en Emacs value.
 * @param env The active Emacs environment.
 * @param _obj The value to check.
 * @return The object type, or EGIT_UNKNOWN if not known.
 */
egit_type egit_get_type(emacs_env *env, emacs_value _obj);

/**
 * Assert that an Emacs value represents a libgit2 struct of a given type, or signal an Emacs error.
 * To check for EGIT_OBJECT, use egit_assert_object instead.
 * @param env The active Emacs environment.
 * @param obj The value to check.
 * @param type The type to check for.
 * @param predicate Symbol to use in a wrong-type-argument error signal.
 * @return True iff the value has the right type.
 */
bool egit_assert_type(emacs_env *env, emacs_value obj, egit_type type, emacs_value predicate);

/**
 * Assert that an Emacs value represents a libgit2 git_object struct, or signal an Emacs error.
 * @param env The active Emacs environment.
 * @param obj The value to check.
 * @return True iff the value has the right type.
 */
bool egit_assert_object(emacs_env *env, emacs_value obj);

/**
 * Decrease the reference count of a git_repository struct.
 * This may cause the object to be freed.
 */
void egit_decref_repository(git_repository *wrapped);

/**
 * Decrease the reference count of a egit_object struct that wraps a git_repository.
 * This will fail if the wrapper wraps something of another type.
 * This may cause the object to be freed.
 * This function is suitable to use as a finalizer for Emacs user pointers.
 */
void egit_decref_repository_wrapper(void *wrapper);

/**
 * Wrap a git_repository structure in an emacs_value, adding it to the object store if necessary.
 * @param env The active Emacs environment.
 * @param ptr The pointer to store.
 * @return The Emacs value.
 */
emacs_value egit_wrap_repository(emacs_env *env, git_repository* ptr);

/**
 * Wrap a git_??? structure in an emacs_value.
 * NOTE: Use egit_wrap_repository instead of this one for repositories.
 * @param env The active Emacs environment.
 * @param obj The type of the object.
 * @param ptr The pointer to store.
 * @return The Emacs value.
 */
emacs_value egit_wrap(emacs_env *env, egit_type type, void* ptr);

/**
 * If libgit2 signaled an error, dispatch that error to Emacs.
 * @param env The active Emacs environment.
 * @param retval A libgit2 return value.
 * @return True iff an error was signalled.
 */
bool egit_dispatch_error(emacs_env *env, int retval);

/**
 * Define functions visible to Emacs.
 * This function only needs to be called once.
 * @param env The active Emacs environment.
 */
void egit_init(emacs_env *env);

#endif /* EGIT_H */
