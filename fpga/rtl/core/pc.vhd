library IEEE;
use IEEE.std_logic_1164.all;

entity pc is
  port(
    clk        : in  std_logic;
    reset      : in  std_logic;
    en         : in  std_logic;
    next_pc    : in  std_logic_vector(31 downto 0);
    current_pc : out std_logic_vector(31 downto 0)
    );
end pc;

architecture env_pc of pc is
begin

  process(clk, reset)
  begin
    if reset = '1' then
      current_pc <= x"00000000";
    elsif rising_edge(clk) then
      if en = '1' then
        current_pc <= next_pc;
      end if;
    end if;
  end process;

end env_pc;
