import argparse
import re
import sys

# ISA Definitions
R_TYPE_FUNCT = {
    "add": 0b100000,  # 0x20
    "sub": 0b100010,  # 0x22
    "and": 0b100100,  # 0x24
    "or": 0b100101,  # 0x25
    "slt": 0b101010,  # 0x2a
}

I_TYPE_OPCODES = {
    "lw": 0b100011,  # 0x23
    "sw": 0b101011,  # 0x2b
    "addi": 0b001000,  # 0x08
    "beq": 0b000100,  # 0x04
}


def parse_register(reg_str):
    """Parses a register string (e.g., '$1', '1', '$31,') and returns the integer ID."""
    reg_clean = reg_str.replace("$", "").replace(",", "").strip()
    try:
        reg_num = int(reg_clean)
        if not (0 <= reg_num <= 31):
            raise ValueError
        return reg_num
    except ValueError:
        print(f"Error: Invalid register format '{reg_str}'. Expected $0 to $31.")
        sys.exit(1)


def parse_immediate(imm_str):
    """Parses an immediate value (hex or decimal)."""
    imm_clean = imm_str.replace(",", "").strip()
    try:
        if imm_clean.lower().startswith(("0x", "-0x")):
            return int(imm_clean, 16)
        return int(imm_clean)
    except ValueError:
        print(f"Error: Invalid immediate value '{imm_str}'.")
        sys.exit(1)


def assemble(asm_lines):
    labels = {}
    instructions = []

    # PASS 1: Label Resolution
    pc = 0
    for line_num, orig_line in enumerate(asm_lines, start=1):
        line = orig_line.split("#")[0].strip()
        if not line:
            continue

        label_match = re.match(r"^(\w+):(.*)", line)
        if label_match:
            label_name = label_match.group(1)
            labels[label_name] = pc
            line = label_match.group(2).strip()

        if line:
            instructions.append(
                {
                    "pc": pc,
                    "text": line,
                    "orig": orig_line.strip(),
                    "line_num": line_num,
                }
            )
            pc += 4

    output = []

    # PASS 2: Machine Code Generation
    for inst in instructions:
        pc = inst["pc"]
        text = inst["text"]
        orig = inst["orig"]
        line_num = inst["line_num"]

        tokens = text.replace(",", " ").split()
        op = tokens[0].lower()

        machine_code = 0

        try:
            if op in R_TYPE_FUNCT:
                rd = parse_register(tokens[1])
                rs = parse_register(tokens[2])
                rt = parse_register(tokens[3])

                opcode = 0b000000
                shamt = 0b00000
                funct = R_TYPE_FUNCT[op]

                machine_code = (
                    (opcode << 26)
                    | (rs << 21)
                    | (rt << 16)
                    | (rd << 11)
                    | (shamt << 6)
                    | funct
                )

            elif op == "addi":
                rt = parse_register(tokens[1])
                rs = parse_register(tokens[2])
                imm = parse_immediate(tokens[3]) & 0xFFFF
                opcode = I_TYPE_OPCODES[op]

                machine_code = (opcode << 26) | (rs << 21) | (rt << 16) | imm

            elif op in ["lw", "sw"]:
                rt = parse_register(tokens[1])
                mem_match = re.match(
                    r"(-?(?:0x[0-9a-fA-F]+|\d+))\(\$(\d+)\)", tokens[2]
                )
                if not mem_match:
                    print(
                        f"Error on line {line_num}: Invalid memory access syntax. Expected imm($rs)."
                    )
                    sys.exit(1)

                imm = parse_immediate(mem_match.group(1)) & 0xFFFF
                rs = int(mem_match.group(2))
                opcode = I_TYPE_OPCODES[op]

                machine_code = (opcode << 26) | (rs << 21) | (rt << 16) | imm

            elif op == "beq":
                rs = parse_register(tokens[1])
                rt = parse_register(tokens[2])
                target_str = tokens[3]

                # NEW LOGIC: Check if it's a label, otherwise try to parse as raw integer
                if target_str in labels:
                    target_address = labels[target_str]
                    offset = (target_address - (pc + 4)) // 4
                else:
                    try:
                        # Attempt to parse target_str directly as an immediate offset
                        offset = (
                            int(target_str, 16)
                            if "0x" in target_str.lower()
                            else int(target_str)
                        )
                    except ValueError:
                        print(
                            f"Error on line {line_num}: Undefined label or invalid offset '{target_str}'"
                        )
                        sys.exit(1)

                imm = offset & 0xFFFF
                opcode = I_TYPE_OPCODES[op]

                machine_code = (opcode << 26) | (rs << 21) | (rt << 16) | imm

            else:
                print(f"Error on line {line_num}: Unsupported instruction '{op}'")
                sys.exit(1)

            array_index = pc // 4
            clean_comment = " ".join(orig.split())
            output.append(f"{machine_code:08X} // {clean_comment}")
            # output.append(
            #     f"instr_mem_ptr[{array_index}] = 0x{machine_code:08X}; // {clean_comment}"
            # )

        except IndexError:
            print(f"Error on line {line_num}: Missing operands for instruction '{op}'")
            sys.exit(1)

    return output


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Custom 32-bit MIPS Assembler")
    parser.add_argument("input_file", help="Input .asm file to assemble")
    parser.add_argument(
        "-o", "--output", help="Optional output file (prints to console if omitted)"
    )
    args = parser.parse_args()

    try:
        with open(args.input_file, "r") as f:
            asm_lines = f.readlines()
    except FileNotFoundError:
        print(f"Error: Could not find file '{args.input_file}'")
        sys.exit(1)

    assembled_code = assemble(asm_lines)

    if args.output:
        with open(args.output, "w") as f:
            for line in assembled_code:
                f.write(line + "\n")
        print(f"Successfully assembled to {args.output}")
    else:
        for line in assembled_code:
            print(line)
