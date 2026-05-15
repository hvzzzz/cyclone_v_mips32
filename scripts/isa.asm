    add  $0, $0, $0 # zero value
    add  $4, $0, $0 # memory pointer

    addi $1, $0, 5  # store 5 in register 1
    addi $2, $0, 60 # store 60 in register 2
    add $3, $1, $2  # register 3 = $1+$2
    sub $5, $2, $1  # register 5 = $2-$1
    sw $3, 0($4)    # save register 3 in data memory
    addi $4, $4, 4
    sw $5, 0($4)    # save register 5 in data memory

    slt $7, $3, $5
    slt $8, $5, $3
    sw $7, 0($4)
    addi $4, $4, 4
    sw $8, 0($4)
    addi $4, $4, 4
    addi $4,$4, -12

    lw $6, 0($4)    # read from data memory to register 6
    addi $6,$6,30   # add 30 to saved data
    sw $6, 0($4)    # store result

    beq  $0, $0, -1 # end
