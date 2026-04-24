library IEEE;
use IEEE.std_logic_1164.all;

entity mux_alu is
  port(
    data_from_register      : in  std_logic_vector(31 downto 0);
    data_from_sign_extender : in  std_logic_vector(31 downto 0);
    alu_src                 : in  std_logic;
    output                  : out std_logic_vector(31 downto 0)
    );
end mux_alu;

architecture env_mux_alu of mux_alu is

begin
  process_mux_alu : process(alu_src, data_from_register, data_from_sign_extender)
  begin
    if alu_src = '0' then
      output <= data_from_register;
    else
      output <= data_from_sign_extender;
    end if;
  end process process_mux_alu;

end env_mux_alu;
