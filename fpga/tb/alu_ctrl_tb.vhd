library IEEE;
use IEEE.std_logic_1164.all;

entity alu_ctrl_tb is
end alu_ctrl_tb;

architecture alu_ctrl_tb_arch of alu_ctrl_tb is
  component alu_ctrl
    port(
      alu_op      : in  std_logic_vector(1 downto 0);
      func_code   : in  std_logic_vector(5 downto 0);
      alu_control : out std_logic_vector(3 downto 0)
      );
  end component;

  signal alu_op      : std_logic_vector (1 downto 0) := "00";
  signal func_code   : std_logic_vector (5 downto 0) := "000000";
  signal alu_control : std_logic_vector(3 downto 0);

begin
  UUT : alu_ctrl
    port map(
      alu_op      => alu_op,
      func_code   => func_code,
      alu_control => alu_control
      );

  alu_op    <= "01"     after 10 ns, "10" after 20 ns;
  func_code <= "100000" after 20 ns, "100010" after 30 ns, "100100" after 40 ns, "100101" after 50 ns, "100111" after 60 ns, "101010" after 70 ns, "111111" after 80 ns;
end alu_ctrl_tb_arch;
