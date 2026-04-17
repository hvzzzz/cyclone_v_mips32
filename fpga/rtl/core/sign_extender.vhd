library IEEE;
use IEEE.std_logic_1164.all;

entity sign_extender is
  port(
    petit : in  std_logic_vector(15 downto 0);
    extended : out  std_logic_vector(31 downto 0)
    );
end sign_extender;

architecture env_sign_extender of sign_extender is

begin
  process_sign_extender : process(petit)
  begin
    extended <= (31 downto 16 => petit(15)) & petit;
  end process process_sign_extender;

end env_sign_extender;
