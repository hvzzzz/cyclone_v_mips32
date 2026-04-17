library IEEE;
use IEEE.std_logic_1164.all;

entity pc_tb is
end pc_tb;

architecture pc_tb_arch of pc_tb is
  component pc
    port(
      clk        : in  std_logic;
      reset      : in  std_logic;
      en         : in  std_logic;
      next_pc    : in  std_logic_vector(31 downto 0);
      current_pc : out std_logic_vector(31 downto 0)
      );
  end component;
  constant period   : time                          := 20 ns;
  signal clk        : std_logic                     := '0';
  signal reset      : std_logic                     := '1';
  signal en         : std_logic                     := '1';
  signal next_pc    : std_logic_vector(31 downto 0) := (others => '0');
  signal current_pc : std_logic_vector(31 downto 0);
begin
  UUT : pc
    port map(
      clk        => clk,
      reset      => reset,
      en         => en,
      next_pc    => next_pc,
      current_pc => current_pc
      );
  clk     <= not clk     after period/2;
  next_pc <= x"00000001" after 10 ns, x"00000002" after 20 ns, x"00000003" after 30 ns, x"00000004" after 40 ns;
  reset   <= '0'         after 2 ns;
  en      <= '0'         after 35 ns;
end pc_tb_arch;
