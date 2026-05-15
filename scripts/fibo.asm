      add  $1, $0, $0   # $1 = 0  (Current Fib)
      addi $2, $0, 1    # $2 = 1  (Next Fib)
      add  $3, $0, $0   # $3 = 0  (Memory Address Pointer)
      addi $6, $0, 0    # $6 = 0  (Initialization of the counter)
      addi $4, $0, 49   # (Loop Limit)

loop: beq  $6, $4, 7    # If Pointer == $4, jump forward 7 instructions to 'exit'
      sw   $1, 0($3)    # Store Current Fib to Data Memory
      add  $5, $1, $2   # Temp = Current + Next
      add  $1, $0, $2   # Current = Next
      add  $2, $0, $5   # Next = Temp
      addi $3, $3, 4    # Pointer = Pointer + 4
      addi $6, $6, 1    # Counter = Counter +1
      beq  $0, $0, -8   # Unconditional jump backward 8 instructions to 'loop'

exit: beq  $0, $0, -1   # Infinite trap
