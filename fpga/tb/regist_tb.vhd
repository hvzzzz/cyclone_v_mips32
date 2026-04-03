library IEEE;
use IEEE.std_logic_1164.all;

entity regist_tb is
end regist_tb;

architecture reg_tb_arch of regist_tb is
  component regist
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
  end component;
  constant period  : time                          := 20 ns;
  signal clk       : std_logic                     := '0';
  signal r1_addr_i : std_logic_vector(4 downto 0)  := (others => '0');
  signal r2_addr_i : std_logic_vector(4 downto 0)  := (others => '0');
  signal w_addr_i  : std_logic_vector(4 downto 0)  := (others => '0');
  signal w_data_i  : std_logic_vector(31 downto 0) := (others => '0');
  signal w_en_i    : std_logic                     := '0';
  signal r1_data_o : std_logic_vector(31 downto 0);
  signal r2_data_o : std_logic_vector(31 downto 0);

begin
  UUT : regist
    port map(
      clk       => clk,
      r1_addr_i => r1_addr_i,
      r2_addr_i => r2_addr_i,
      w_addr_i  => w_addr_i,
      w_data_i  => w_data_i,
      w_en_i    => w_en_i,
      r1_data_o => r1_data_o,
      r2_data_o => r2_data_o
      );
  clk       <= not clk     after period/2;
  w_en_i    <= '1'         after 5 ns, '0' after 200 ns;
  w_addr_i  <= "00001"     after 8 ns, "00010" after 25 ns, "00100" after 35 ns, "01000" after 45 ns, "10000" after 55 ns, "11111" after 65 ns, "00000" after 150 ns;
  w_data_i  <= x"0000000f" after 8 ns, x"000000f0" after 25 ns, x"00000f00" after 35 ns, x"0000f000" after 45 ns, x"000f0000" after 55 ns, x"00f00000" after 65 ns, x"ffffffff" after 150 ns;
  r1_addr_i <= "00001"     after 80 ns, "00010" after 90 ns, "00100" after 100 ns, "01000" after 110 ns, "10000" after 120 ns, "11111" after 130 ns, "00000" after 150 ns;
  r2_addr_i <= "00001"     after 80 ns, "00010" after 90 ns, "00100" after 100 ns, "01000" after 110 ns, "10000" after 120 ns, "11111" after 130 ns, "00000" after 150 ns;

end reg_tb_arch;
