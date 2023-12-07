general

- fix (assert ... = true)
- prefix all library functions/types
- move MIN/MAX macros somewhere shareable
- simplify linkedlist tests by converting to arrays to run expectations

day5

- add util function for interval maps, to substract an interval from the dst side, and return the
  remaining interval map(s), using union akin to existing util for intervals
- use this recursively on remaining parts of interval maps to make sure all parts are checked
  against range maps
