library IEEE;
use IEEE.std_logic_1164.all;

entity mux_32 is
  port(
    input_0 : in  std_logic_vector(31 downto 0);
    input_1 : in  std_logic_vector(31 downto 0);
    sel     : in  std_logic;
    output  : out std_logic_vector(31 downto 0)
    );
end mux_32;

architecture env_mux_32 of mux_32 is

begin
  process_mux_32 : process(sel, input_0, input_1)
  begin
    if sel = '0' then
      output <= input_0;
    else
      output <= input_1;
    end if;
  end process process_mux_32;

end env_mux_32;
