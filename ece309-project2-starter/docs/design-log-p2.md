# Design Log — Project 2

(500–800 words total. See spec §5 for what each section must cover.)

## Growth factor and amortized cost
The Conversation class uses a dynamic array begining at capacity 0. When the first message is appended using append() this capacity becomes 1. For every subsequent message after the array fills the capacity doubles. So the second message doubles the capacity to 2, the third doubles to 4, the fifth to 8, ninth to 16, and so on and so forth. Each time copying the old elements into the new array before deleting the old array.

As the capacity grows larger and larger the less often this copying of cost O(n) (because you have n messages to copy) is needed so over a large number of appends the amortized cost is O(1).


## Rule of Five evidence
Within conversation.cpp the following are covered:
- Destructor: Uses delete[] to release the allocated memory
- Copy Constructor: Performs a deep copy into a newly allocated array
- Copy Assignment Operator: Checks for self-assignment and performs a deep copy into pre-existing object
- Move Constructor: Steals the pointer and gives it to a newly created object and resets the source
- Move Assignment Operator: CHecks for self-assignment and steals the pointer and gives it to a pre-existing object and resets the source
Therefore it fully abides by Rule of Five.


## Sentinel scanner: bounded pending_ proof
The SentinelScanner detects <|end_conversation|> but can arrive in chunks. The scanner combines the previous pending_ characters with the next chuck and searches for the sentinel. If it's not found, the scanner outputs everything except the last [S-1] characters where S is the length of the sentinel, in case the sentinel is split between chunks.


## What I would change differently
Start earlier for one, I've done nothing but this project for like the past five days. In my test 11 and 12 I'd also probably move those class outside main so I don't have to type them twice.