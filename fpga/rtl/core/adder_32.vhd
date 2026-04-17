library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity adder_32 is
  port(
    input_0 : in  std_logic_vector(31 downto 0);
    input_1 : in  std_logic_vector(31 downto 0);
    result  : out std_logic_vector(31 downto 0)
    );
end adder_32;

architecture env_adder_32 of adder_32 is

begin
  process_mux_32 : process(input_0, input_1)
  begin
    result <= std_logic_vector(signed(input_0) + signed(input_1));
  end process process_mux_32;

end env_adder_32;
