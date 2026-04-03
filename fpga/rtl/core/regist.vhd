library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity regist is
  port(
    clk       : in  std_logic;
    r1_addr_i : in  std_logic_vector(4 downto 0);
    r2_addr_i : in  std_logic_vector(4 downto 0);
    w_addr_i  : in  std_logic_vector(4 downto 0);
    w_data_i  : in  std_logic_vector(31 downto 0);
    w_en_i    : in  std_logic;
    r1_data_o : out std_logic_vector(31 downto 0);
    r2_data_o : out std_logic_vector(31 downto 0)
    );
end regist;

architecture env_reg of regist is
  type reg is array (31 downto 0) of std_logic_vector(31 downto 0);
  signal reg_file : reg := (others => (others => '0'));

begin

  process(r1_addr_i, reg_file)
  begin
    if r1_addr_i = "00000" then
      r1_data_o <= x"00000000";
    else
      r1_data_o <= reg_file(to_integer(unsigned(r1_addr_i)));
    end if;
  end process;

  process(r2_addr_i, reg_file)
  begin
    if r2_addr_i = "00000" then
      r2_data_o <= x"00000000";
    else
      r2_data_o <= reg_file(to_integer(unsigned(r2_addr_i)));
    end if;
  end process;

  process(clk)
  begin
    if rising_edge(clk) then
      if w_en_i = '1' and w_addr_i /= "00000" then
        reg_file(to_integer(unsigned(w_addr_i))) <= w_data_i;
      end if;
    end if;
  end process;

end env_reg;
