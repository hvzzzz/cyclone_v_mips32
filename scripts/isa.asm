    add  $0, $0, $0 # $0 = 0 -> zero value
    add  $4, $0, $0 # $4 = 0 -> memory pointer

    addi $1, $0, 5  # $1 = $0 + 5 -> store 5 in register 1
    addi $2, $0, 60 # $2 = $0 + 60 -> store 60 in register 2
    add $3, $1, $2  # $3 = $1 + $2  = 65 -> register 3 = $1+$2
    sub $5, $2, $1  # $5 = $2 - $1  = 55 -> register 5 = $2-$1
    sw $3, 0($4)    # save register 3 in data memory at position 0
    addi $4, $4, 4  # $4 = 4 -> update memory pointer
    sw $5, 0($4)    # save register 5 in data memory at position 4

    slt $7, $3, $5  # $7 = 0 because $3 < $5 = false / 65 < 55
    slt $8, $5, $3  # $8 = 1 because $5 < $3 = true / 55 < 65
    addi $4, $4, 4  # $4 = 8 -> update memory pointer
    sw $7, 0($4)    # save register 7 in data memory at position 8
    addi $4, $4, 4  # $4 = 12 -> update memory pointer
    sw $8, 0($4)    # save register 8 in data memory at position 12
    addi $4, $4, 4  # $4 = 16 -> update memory pointer
    addi $4,$4, -16 # $4 = 0 -> update memory pointer

    lw $6, 0($4)    # $6 = 65 -> read from data memory to register 6
    addi $6,$6,30   # $6 = 65 + 30 -> add 30 to saved data
    sw $6, 0($4)    # store result

    beq  $0, $0, -1 # end
