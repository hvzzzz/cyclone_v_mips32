library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity shift_2 is
  port(
    input   : in  std_logic_vector(31 downto 0);
    shifted : out std_logic_vector(31 downto 0)
    );
end shift_2;

architecture env_shift2 of shift_2 is

begin
  process_mux_32 : process(input)
  begin
    shifted <= input(29 downto 0) & "00";
  end process process_mux_32;

end env_shift2;
