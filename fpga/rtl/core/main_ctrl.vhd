library IEEE;
use IEEE.std_logic_1164.all;

entity main_ctrl is
  port(
    op_code   : in  std_logic_vector(5 downto 0);
    reg_dst   : out std_logic;
    alu_src   : out std_logic;
    mem_2_reg : out std_logic;
    reg_write : out std_logic;
    mem_read  : out std_logic;
    mem_write : out std_logic_vector(3 downto 0);
    branch    : out std_logic;
    alu_op    : out std_logic_vector(1 downto 0)
    );
end main_ctrl;

architecture env_main_ctrl of main_ctrl is

  signal output_vector : std_logic_vector(8 downto 0);
  signal bram_enable:std_logic_vector(3 downto 0) ;

begin
  process_main_ctrl : process(op_code)
  begin
    case op_code is
      when "000000" => output_vector <= "100100010"; -- R-type
      when "100011" => output_vector <= "011110000"; -- lw
      -- when "101011" => output_vector <= "-1-001000";
      -- when "000100" => output_vector <= "-0-000101";
      when "101011" => output_vector <= "010001000"; -- sw
      when "000100" => output_vector <= "000000101"; -- beq
      when "001000" => output_vector <= "010100000"; -- addi
      when others   => output_vector <= "000000000";
    end case;
  end process process_main_ctrl;

  process_bram_ctrl: process(output_vector(3))
  begin
    if output_vector(3)= '1' then
      bram_enable <="1111";
    else
      bram_enable <="0000";
    end if;
  end process process_bram_ctrl;

  reg_dst   <= output_vector(8);
  alu_src   <= output_vector(7);
  mem_2_reg <= output_vector(6);
  reg_write <= output_vector(5);
  mem_read  <= output_vector(4);
  -- mem_write <= output_vector(3);
  mem_write <= bram_enable;
  branch    <= output_vector(2);
  alu_op    <= output_vector(1 downto 0);
end env_main_ctrl;
