#ifndef STLREPAIR_CONTRACTS__H_
#define STLREPAIR_CONTRACTS__H_

/**
 * These are "Design by Contract" macros. These could have all been implemented
 * directly in code as if-statements. However, I find it useful to visually
 * differentiate between pre-conditions/post-conditions/invariants vs. other
 * conditional logic.
 *
 * One day, these might be replaced with language-supported contracts.
 */
#define precondition_ret(X, Y) { if (!(X)) { return Y; }}
#define precondition_throw(X, Y) { if (!(X)) { throw Y; }}
#define invariant_ret(X, Y) { if (!(X)) { return Y; }}
#define invariant_throw(X, Y) { if (!(X)) { throw Y; }}

#endif
