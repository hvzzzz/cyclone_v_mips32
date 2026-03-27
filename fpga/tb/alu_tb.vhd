library IEEE;
use IEEE.std_logic_1164.all;

entity alu_tb is
end alu_tb;

architecture alu_tb_arch of alu_tb is
  component alu
    port (
      in_a        : in  std_logic_vector(31 downto 0);
      in_b        : in  std_logic_vector(31 downto 0);
      alu_control : in  std_logic_vector(3 downto 0);
      zero        : out std_logic;
      alu_out     : out std_logic_vector(31 downto 0)
      );
  end component;

  signal in_a        : std_logic_vector (31 downto 0) := x"00000000";
  signal in_b        : std_logic_vector (31 downto 0) := x"00000000";
  signal alu_control : std_logic_vector(3 downto 0)   := "0000";
  signal zero        : std_logic;
  signal alu_out     : std_logic_vector(31 downto 0);

begin

  UUT : alu
    port map(
      in_a        => in_a,
      in_b        => in_b,
      alu_control => alu_control,
      zero        => zero,
      alu_out     => alu_out
      );
  alu_control <= "0001"      after 10 ns, "0010" after 20 ns, "0110" after 30 ns, "0111" after 40 ns;
  in_a        <= x"00000001" after 1 ns;
  in_b        <= x"00000010" after 1 ns;
end alu_tb_arch;
