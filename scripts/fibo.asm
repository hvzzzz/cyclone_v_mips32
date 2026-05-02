add  $1, $0, $0         # $1 = 0  (Current Fib)
      addi $2, $0, 1    # $2 = 1  (Next Fib)
      add  $3, $0, $0   # $3 = 0  (Memory Address Pointer)
      addi $4, $0, 40   # $4 = 40 (Loop Limit: 10 numbers * 4 bytes)

loop: beq  $3, $4, 6    # If Pointer == 40, jump forward 6 instructions to 'exit'
      sw   $1, 0($3)    # Store Current Fib to Data Memory
      add  $5, $1, $2   # Temp = Current + Next
      add  $1, $0, $2   # Current = Next
      add  $2, $0, $5   # Next = Temp
      addi $3, $3, 4    # Pointer = Pointer + 4
      beq  $0, $0, -7   # Unconditional jump backward 7 instructions to 'loop'

exit: beq  $0, $0, -1   # Infinite trap
