library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity i_memory is
  port(
    ins_addr_i    : in  std_logic_vector(31 downto 0);
    instruction_o : out std_logic_vector(31 downto 0)
    );
end i_memory;

architecture env_i_memory of i_memory is
  type reg is array (0 to 1023) of std_logic_vector(31 downto 0);
  signal reg_file : reg := (others => (others => '0'));

begin

  instruction_o <= reg_file(to_integer(unsigned(ins_addr_i(11 downto 2))));

end env_i_memory;
