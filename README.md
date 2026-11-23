# Thread-Safe Blocking Queue in C

**Status:** Private review copy of coursework implementation. Do not make public until course-sharing permissions and individual contributions are verified.

## Overview
A synchronized FIFO queue in C using `pthread_mutex_t` for critical sections and a POSIX semaphore to block consumers until data becomes available. Includes producer/consumer and queue behavior tests.

## Build and test (Linux)
```bash
make
./test_queue
```

## Verification to complete
- Confirm authorship and coursework publication rules.
- Review error paths, shutdown behavior and NULL item semantics.
- Run under ThreadSanitizer or Valgrind/Helgrind where supported.
- Document tests you actually pass; don't infer success from the code alone.

*McGill University coursework implementation.*
