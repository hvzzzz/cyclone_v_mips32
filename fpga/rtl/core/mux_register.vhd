library IEEE;
use IEEE.std_logic_1164.all;

entity mux_register is
  port(
    address_0 : in  std_logic_vector(4 downto 0);
    address_1 : in  std_logic_vector(4 downto 0);
    sel       : in  std_logic;
    output    : out std_logic_vector(4 downto 0)
    );
end mux_register;

architecture env_mux_register of mux_register is

begin
  process_main_ctrl : process(sel,address_0,address_1)
  begin
    if sel = '0' then
      output <= address_0;
    else
      output <= address_1;
    end if;
  end process process_main_ctrl;

end env_mux_register;
