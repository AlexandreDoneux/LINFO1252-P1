typedef volatile int spinlock_t; // defining a type that is acttually a volatile int. We could just use volatile int directly but this is more readable.

void lock(spinlock_t *lock);
void unlock(spinlock_t *lock);